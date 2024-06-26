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

#include <stdbool.h>
#include <stdint.h>

#include <gvc/gvplugin_device.h>

#ifdef HAVE_GTK
#include <gtk/gtk.h>

#include <cairo.h>
#ifdef CAIRO_HAS_XLIB_SURFACE
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "interface.h"
#include "support.h"

// note that we do not own the newly entered string - must copy
static void
attr_value_edited_cb(GtkCellRendererText *renderer, gchar *pathStr, gchar *newText, gpointer data)
{
	(void)renderer;

	GtkTreeModel *model = GTK_TREE_MODEL(data);
	GtkTreeIter iter;
	gchar *old_attr;
	
	GtkTreePath *treepath = gtk_tree_path_new_from_string(pathStr);
	
	// need to free old attr value in job and allocate new attr value - how?
	
	// free old attr value in model
	gtk_tree_model_get_iter(model, &iter, treepath);
	gtk_tree_model_get(model, &iter, 1, &old_attr, -1);
	g_free(old_attr);
	
	// set new attr value in model
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, 1, g_strdup(newText), -1);
	
	gtk_tree_path_free(treepath);
}

static void gtk_initialize(GVJ_t *firstjob)
{
    Display *dpy;
    const char *display_name = NULL;
    int scr;

    gtk_set_locale ();
    gtk_init (NULL, NULL);

    dpy = XOpenDisplay(display_name);
    if (dpy == NULL) {
        fprintf(stderr, "Failed to open XLIB display: %s\n",
                XDisplayName(NULL));
        return;
    }
    scr = DefaultScreen(dpy);
    firstjob->device_dpi.x = DisplayWidth(dpy, scr) * 25.4 / DisplayWidthMM(dpy, scr);
    firstjob->device_dpi.y = DisplayHeight(dpy, scr) * 25.4 / DisplayHeightMM(dpy, scr);
    firstjob->device_sets_dpi = true;
}

static void gtk_finalize(GVJ_t *firstjob)
{
    GVJ_t *job;
    GtkWidget *window1, *drawingarea1, *drawingarea2, *treeview2;
    GtkListStore *attr_store;
    GtkCellRenderer *value_renderer;

    for (job = firstjob; job; job = job->next_active) {
	window1 = create_window1 ();

	g_object_set_data(G_OBJECT(window1), "job", job);

	drawingarea1 = lookup_widget (window1, "drawingarea1");      /* main graph view */
	g_object_set_data(G_OBJECT(drawingarea1), "job", job);

	drawingarea2 = lookup_widget (window1, "drawingarea2");      /* keyholeview */
	g_object_set_data(G_OBJECT(drawingarea2), "job", job);

	treeview2 = lookup_widget (window1, "treeview2"); /* attribute/value view */
	g_object_set_data(G_OBJECT(treeview2), "job", job);
	
	attr_store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
	
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview2), -1, "Name", 
		gtk_cell_renderer_text_new(), "text", 0, NULL);
	
	value_renderer = gtk_cell_renderer_text_new();
	g_signal_connect(G_OBJECT(value_renderer), "edited", G_CALLBACK(attr_value_edited_cb), attr_store);
	g_object_set(G_OBJECT(value_renderer), "editable", TRUE, "wrap-mode", PANGO_WRAP_WORD, 
		"wrap-width", 100, NULL);
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview2), -1, "Value", value_renderer, 
		"text", 1, NULL);
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeview2), GTK_TREE_MODEL(attr_store));
	g_object_set_data(G_OBJECT(drawingarea1), "attr_store", attr_store);
	
	gtk_widget_show (window1);
    }

    gtk_main();
}

static gvdevice_features_t device_features_gtk = {
    GVDEVICE_DOES_TRUECOLOR
	| GVDEVICE_EVENTS,      /* flags */
    {0.,0.},                    /* default margin - points */
    {0.,0.},                    /* default page width, height - points */
    {96.,96.},                  /* dpi */
};

static gvdevice_engine_t device_engine_gtk = {
    gtk_initialize,
    NULL,			/* gtk_format */
    gtk_finalize,
};
#endif
#endif

gvplugin_installed_t gvdevice_types_gtk[] = {
#ifdef HAVE_GTK
#ifdef CAIRO_HAS_XLIB_SURFACE
    {0, "gtk:cairo", 0, &device_engine_gtk, &device_features_gtk},
#endif
#endif
    {0, NULL, 0, NULL, NULL}
};
