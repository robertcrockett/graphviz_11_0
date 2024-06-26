/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include "config.h"

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <gvc/gvplugin_render.h>
#include <cgraph/agxbuf.h>
#include <cgraph/alloc.h>
#include <common/utils.h>
#include <gvc/gvplugin_textlayout.h>

#include <pango/pangocairo.h>
#include "gvgetfontlist.h"
#ifdef HAVE_PANGO_FC_FONT_LOCK_FACE
#include <pango/pangofc-font.h>
#endif

static void pango_free_layout (void *layout)
{
    g_object_unref(layout);
}

static char* pango_psfontResolve (PostscriptAlias* pa)
{
    static char buf[1024];
    strcpy(buf, pa->family);
    strcat(buf, ",");
    if (pa->weight) {
        strcat(buf, " ");
        strcat(buf, pa->weight);
    }
    if (pa->stretch) {
        strcat(buf, " ");
        strcat(buf, pa->stretch);
    }
    if (pa->style) {
        strcat(buf, " ");
        strcat(buf, pa->style);
    }
    return buf;
}

#define FONT_DPI 96.

#define ENABLE_PANGO_MARKUP

// wrapper to handle difference in calling conventions between `agxbput` and
// `xml_escape`’s `cb`
static int agxbput_int(void *buffer, const char *s) {
  size_t len = agxbput(buffer, s);
  assert(len <= INT_MAX);
  return (int)len;
}

static bool pango_textlayout(textspan_t * span, char **fontpath)
{
    static char buf[1024];  /* returned in fontpath, only good until next call */
    static PangoFontMap *fontmap;
    static PangoContext *context;
    static PangoFontDescription *desc;
    static char *fontname;
    static double fontsize;
    static gv_font_map* gv_fmap;
    char *fnt, *psfnt = NULL;
    PangoLayout *layout;
    PangoRectangle logical_rect;
    cairo_font_options_t* options;
    PangoFont *font;
#ifdef ENABLE_PANGO_MARKUP
    PangoAttrList *attrs;
    GError *error = NULL;
    int flags;
#endif
    char *text;
    double textlayout_scale;
    PostscriptAlias *pA;

    if (!context) {
	fontmap = pango_cairo_font_map_new();
	gv_fmap = get_font_mapping(fontmap);
	context = pango_font_map_create_context (fontmap);
	options=cairo_font_options_create();
	cairo_font_options_set_antialias(options,CAIRO_ANTIALIAS_GRAY);
	cairo_font_options_set_hint_style(options,CAIRO_HINT_STYLE_FULL);
	cairo_font_options_set_hint_metrics(options,CAIRO_HINT_METRICS_ON);
	cairo_font_options_set_subpixel_order(options,CAIRO_SUBPIXEL_ORDER_BGR);
	pango_cairo_context_set_font_options(context, options);
	pango_cairo_context_set_resolution(context, FONT_DPI);
	cairo_font_options_destroy(options);
	g_object_unref(fontmap);
    }

    if (!fontname || strcmp(fontname, span->font->name) != 0 || fontsize != span->font->size) {

	/* check if the conversion to Pango units below will overflow */
	if ((double)(G_MAXINT / PANGO_SCALE) < span->font->size) {
	    return false;
	}

	free(fontname);
	fontname = gv_strdup(span->font->name);
	fontsize = span->font->size;
	pango_font_description_free (desc);

	pA = span->font->postscript_alias;
	if (pA) {
	    psfnt = fnt = gv_fmap[pA->xfig_code].gv_font;
	    if(!psfnt)
		psfnt = fnt = pango_psfontResolve (pA);
	}
	else
	    fnt = fontname;

	desc = pango_font_description_from_string(fnt);
        /* all text layout is done at a scale of FONT_DPI (nominaly 96.) */
        pango_font_description_set_size (desc, (gint)(fontsize * PANGO_SCALE));

        if (fontpath && (font = pango_font_map_load_font(fontmap, context, desc))) {  /* -v support */
	    const char *fontclass;

	    fontclass = G_OBJECT_CLASS_NAME(G_OBJECT_GET_CLASS(font));

	    buf[0] = '\0';
	    if (psfnt) {
		strcat(buf, "(ps:pango  ");
		strcat(buf, psfnt);
		strcat(buf, ") ");
	    }
	    strcat(buf, "(");
	    strcat(buf, fontclass);
	    strcat(buf, ") ");
#ifdef HAVE_PANGO_FC_FONT_LOCK_FACE
	    if (strcmp(fontclass, "PangoCairoFcFont") == 0) {
	        FT_Face face;
	        PangoFcFont *fcfont;
	        FT_Stream stream;
	        FT_StreamDesc streamdesc;
	        fcfont = PANGO_FC_FONT(font);
	        face = pango_fc_font_lock_face(fcfont);
	        if (face) {
		    strcat(buf, "\"");
		    strcat(buf, face->family_name);
		    strcat(buf, ", ");
		    strcat(buf, face->style_name);
		    strcat(buf, "\" ");

		    stream = face->stream;
		    if (stream) {
			streamdesc = stream->pathname;
			if (streamdesc.pointer)
			    strcat(buf, (char*)streamdesc.pointer);
		        else
			    strcat(buf, "*no pathname available*");
		    }
		    else
			strcat(buf, "*no stream available*");
		}
	        pango_fc_font_unlock_face(fcfont);
	    }
	    else
#endif
	    {
    		PangoFontDescription *tdesc;
		char *tfont;

	        tdesc = pango_font_describe(font);
	        tfont = pango_font_description_to_string(tdesc);
	        strcat(buf, "\"");
	        strcat(buf, tfont);
	        strcat(buf, "\" ");
	        g_free(tfont);
	    }
            *fontpath = buf;
        }
    }

#ifdef ENABLE_PANGO_MARKUP
    if ((span->font) && (flags = span->font->flags)) {
	agxbuf xb = {0};

	agxbput(&xb,"<span");

	if (flags & HTML_BF)
	    agxbput(&xb," weight=\"bold\"");
	if (flags & HTML_IF)
	    agxbput(&xb," style=\"italic\"");
	if (flags & HTML_UL)
	    agxbput(&xb," underline=\"single\"");
	if (flags & HTML_S)
	    agxbput(&xb," strikethrough=\"true\"");
	agxbput (&xb,">");

	if (flags & HTML_SUP)
	    agxbput(&xb,"<sup>");
	if (flags & HTML_SUB)
	    agxbput(&xb,"<sub>");

	const xml_flags_t xml_flags = {.raw = 1, .dash = 1, .nbsp = 1};
	xml_escape(span->str, xml_flags, agxbput_int, &xb);

	if (flags & HTML_SUB)
	    agxbput(&xb,"</sub>");
	if (flags & HTML_SUP)
	    agxbput(&xb,"</sup>");

	agxbput (&xb,"</span>");
	if (!pango_parse_markup (agxbuse(&xb), -1, 0, &attrs, &text, NULL, &error)) {
	    fprintf (stderr, "Error - pango_parse_markup: %s\n", error->message);
	    text = span->str;
	    attrs = NULL;
	}
	agxbfree (&xb);
    }
    else {
	text = span->str;
	attrs = NULL;
    }
#else
    text = span->str;
#endif

    layout = pango_layout_new (context);
    span->layout = layout;    /* layout free with textspan - see labels.c */
    span->free_layout = pango_free_layout;    /* function for freeing pango layout */

    pango_layout_set_text (layout, text, -1);
    pango_layout_set_font_description (layout, desc);
#ifdef ENABLE_PANGO_MARKUP
    if (attrs)
	pango_layout_set_attributes (layout, attrs);
#endif

    pango_layout_get_extents (layout, NULL, &logical_rect);

    /* if pango doesn't like the font then it sets width=0 but height = garbage */
    if (logical_rect.width == 0)
	logical_rect.height = 0;

    textlayout_scale = POINTS_PER_INCH / (FONT_DPI * PANGO_SCALE);
    span->size.x = logical_rect.width * textlayout_scale;
    span->size.y = logical_rect.height * textlayout_scale;

    /* The y offset from baseline to 0,0 of the bitmap representation */
    span->yoffset_layout = pango_layout_get_baseline (layout) * textlayout_scale;

    /* The distance below midline for y centering of text strings */
    span->yoffset_centerline = 0.05 * span->font->size;

    return logical_rect.width != 0 || strcmp(text, "") == 0;
}

static gvtextlayout_engine_t pango_textlayout_engine = {
    pango_textlayout,
};

gvplugin_installed_t gvtextlayout_pango_types[] = {
    {0, "textlayout", 10, &pango_textlayout_engine, NULL},
    {0, NULL, 0, NULL, NULL}
};
