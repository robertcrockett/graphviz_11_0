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
#include <gtk/gtk.h>
#include <limits.h>
#include <stdbool.h>

#include <gvc/gvplugin_device.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

void
on_new1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    (void)user_data;

    GtkWindow *window1;
    GVJ_t *job;

    window1 = GTK_WINDOW(menuitem);
    job = g_object_get_data(G_OBJECT(window1), "job");

    job->callbacks->read(job, NULL, "dot");

    // should there be specific menus for (un)directed graphs etc?
    //  - I think the directed flag only affects layout and rendering
    //      so I plan to make it look like a graph attribute.
    //      Similarly "strict".
}

static void
ui_open_graph(GtkWindow *window1, gchar *filename)
{
    GVJ_t *job;
    GtkWidget *dialog;

    job = g_object_get_data(G_OBJECT(window1), "job");
    dialog = gtk_file_chooser_dialog_new(
		"Open graph", window1, GTK_FILE_CHOOSER_ACTION_OPEN,
		"Cancel", GTK_RESPONSE_CANCEL,
		"Open", GTK_RESPONSE_ACCEPT,
		NULL);
    if (filename)
	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog), filename);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    gtk_widget_destroy(dialog);
    if (filename) {
    	job->callbacks->read(job, filename, "dot");
//	if (!file) // we'll probably want to create a error dialog function
//	    fprintf(stderr, "Could not open file: %s\n", filename);
//	else
	    g_object_set_data_full(G_OBJECT(window1),
		    "activefilename", filename, (GDestroyNotify)g_free);
    }
}

void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    (void)user_data;

    GtkWindow *window1;
    gchar *filename;

    window1 = GTK_WINDOW(menuitem);
    filename = g_object_get_data(G_OBJECT(window1), "activefilename");
    ui_open_graph(window1, filename);
}

static void
ui_save_graph(GtkWindow *window1, gchar *filename)
{
    GtkWidget *dialog;

    GVJ_t *job = g_object_get_data(G_OBJECT(window1), "job");

    dialog = gtk_file_chooser_dialog_new(
		"Save graph as", window1, GTK_FILE_CHOOSER_ACTION_SAVE,
		"Cancel", GTK_RESPONSE_CANCEL,
		"Save", GTK_RESPONSE_ACCEPT,
		NULL);
    filename = g_object_get_data(G_OBJECT(window1), "activefilename");
    if (filename)
	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog), filename);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    gtk_widget_destroy(dialog);
    if (filename) {
	job->callbacks->render(job, "dot", filename);
	g_object_set_data_full(G_OBJECT(window1),
		"activefilename", filename, (GDestroyNotify)g_free);
    }
}

void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    (void)user_data;

    GtkWindow *window1;

    window1 = GTK_WINDOW(menuitem);
    gchar *filename = g_object_get_data(G_OBJECT(window1), "activefilename");
    ui_save_graph(window1, filename);
}


void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    (void)user_data;

    GtkWindow *window1;
       
    window1 = GTK_WINDOW(menuitem);
    ui_save_graph(window1, NULL);
}


void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    (void)user_data;
    gtk_widget_destroy(GTK_WIDGET(gtk_widget_get_toplevel(GTK_WIDGET(menuitem))));
    gtk_main_quit();
}


void
on_cut1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    (void)menuitem;
    (void)user_data;
    // I am thinking that we will annotate a node as to whether it is selected,
    // then retrieve a list of selected nodes for these operations
}


void
on_copy1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  (void)menuitem;
  (void)user_data;
}


void
on_paste1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  (void)menuitem;
  (void)user_data;
}


void
on_delete1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  (void)menuitem;
  (void)user_data;
}


void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    (void)user_data;

    static gchar *authors[] = {
		"John Ellson", 
		"Emden Gansner",
		"Stephen North",
		"special thanks to Michael Lawrence",
		NULL };
    GtkWindow *window = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(menuitem)));
    gtk_show_about_dialog(window,
		"name", "DotEdit",
		"program-name", "DotEdit",
		"version", "0.1",
		"copyright", "(C) 2011 AT&T Intellectual Procerty.",
		"license", "Eclipse Public License v1.0.",
		"website", "https://graphviz.org",
		"comments", "Visualize and edit graphs of nodes and edges",
		"authors", authors,
		NULL);
}

static void load_store_with_attrs(GtkListStore *model) {
        gtk_list_store_clear(model);
}


gboolean
on_drawingarea1_expose_event           (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
    (void)event;
    (void)user_data;

    cairo_t *cr;

    GVJ_t *job = g_object_get_data(G_OBJECT(widget),"job");
    cr = gdk_cairo_create(widget->window);

    job->callbacks->motion(job, job->pointer);

    job->context = cr;
    job->external_context = true;
    assert(widget->allocation.width >= 0);
    job->width = (unsigned)widget->allocation.width;
    assert(widget->allocation.height >= 0);
    job->height = (unsigned)widget->allocation.height;
    if (job->has_been_rendered) {
    	job->callbacks->refresh(job);
    }
    else {
	job->callbacks->refresh(job);
	
// FIXME - copy image to keyhole
//      the keyhole image is a fixed size and doesn;t need to be recomputed 
//      each time.   save a pixmap, then each time, show pixmap and overlay
//      with scaled view rectangle.

    }
    cairo_destroy(cr);

    load_store_with_attrs(GTK_LIST_STORE(g_object_get_data(G_OBJECT(widget), "attr_store")));

    return false;
}


gboolean
on_drawingarea1_motion_notify_event    (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
    (void)user_data;

    GVJ_t *job = g_object_get_data(G_OBJECT(widget),"job");
    job->pointer.x = event->x;
    job->pointer.y = event->y;
    gtk_widget_queue_draw(widget);

    return false;
}


gboolean
on_drawingarea2_motion_notify_event    (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
  (void)widget;
  (void)event;
  (void)user_data;

  return false;
}



gboolean
on_drawingarea2_expose_event           (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
    (void)event;
    (void)user_data;

    cairo_t *cr;
    double tmp;

    GVJ_t *job = g_object_get_data(G_OBJECT(widget),"job");
    cr = gdk_cairo_create(widget->window);

    job->callbacks->motion(job, job->pointer);

    job->context = cr;
    job->external_context = true;
    assert(widget->allocation.width >= 0);
    job->width = (unsigned)widget->allocation.width;
    assert(widget->allocation.height >= 0);
    job->height = (unsigned)widget->allocation.height;

    tmp = job->zoom;
    job->zoom = MIN(job->width * POINTS_PER_INCH / (job->bb.UR.x * job->dpi.x),
                    job->height * POINTS_PER_INCH / (job->bb.UR.y * job->dpi.y));
    job->callbacks->refresh(job);
    job->zoom = tmp;

    cairo_destroy(cr);

    return false;
}

gboolean
on_window1_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    (void)widget;
    (void)event;
    (void)user_data;

    gtk_main_quit();
    return false;
}


gboolean
on_drawingarea1_configure_event        (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
    (void)user_data;

    double zoom_to_fit;

/*FIXME - should allow for margins */
/*      - similar zoom_to_fit code exists in: */
/*      plugin/gtk/callbacks.c */
/*      plugin/xlib/gvdevice_xlib.c */
/*      lib/gvc/gvevent.c */

    GVJ_t *job = g_object_get_data(G_OBJECT(widget),"job");
    if (! job->has_been_rendered) {
	zoom_to_fit = MIN((double) event->width / (double) job->width,
			  (double) event->height / (double) job->height);
        if (zoom_to_fit < 1.0) /* don't make bigger */
	    job->zoom *= zoom_to_fit;
    }
    else if (job->fit_mode) {
	zoom_to_fit = MIN((double) event->width / (double) job->width,
			  (double) event->height / (double) job->height);
	job->zoom *= zoom_to_fit;
    }
    if ((event->width >= 0 && (unsigned)event->width > job->width) ||
        (event->height >= 0 && (unsigned)event->height > job->height))
	job->has_grown = true;
    assert(event->width >= 0);
    job->width = (unsigned)event->width;
    assert(event->height >= 0);
    job->height = (unsigned)event->height;
    job->needs_refresh = true;

    return false;
}


gboolean
on_drawingarea1_button_press_event     (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    (void)user_data;

    pointf pointer;

    GVJ_t *job = g_object_get_data(G_OBJECT(widget),"job");
    pointer.x = event->x;
    pointer.y = event->y;
    assert(event->button <= INT_MAX);
    job->callbacks->button_press(job, (int)event->button, pointer);
    
    load_store_with_attrs(GTK_LIST_STORE(g_object_get_data(G_OBJECT(widget), "attr_store")));
    return false;
}


gboolean
on_drawingarea1_button_release_event   (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    (void)user_data;

    pointf pointer;

    GVJ_t *job = g_object_get_data(G_OBJECT(widget),"job");
    pointer.x = event->x;
    pointer.y = event->y;
    assert(event->button <= INT_MAX);
    job->callbacks->button_release(job, (int)event->button, pointer);

    return false;
}


gboolean
on_drawingarea1_scroll_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    (void)user_data;

    pointf pointer;

    GVJ_t *job = g_object_get_data(G_OBJECT(widget),"job");
    pointer.x = ((GdkEventScroll *)event)->x;
    pointer.y = ((GdkEventScroll *)event)->y;
    switch (((GdkEventScroll *)event)->direction) {
	case GDK_SCROLL_UP:
	    job->callbacks->button_press(job, 4, pointer);
	    break;
	case GDK_SCROLL_DOWN:
	    job->callbacks->button_press(job, 5, pointer);
	    break;
	default: // do nothing
	    break;
    }
    gtk_widget_queue_draw(widget);

    return false;
}

gboolean
on_button1_button_press_event          (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  (void)widget;
  (void)event;
  (void)user_data;


fprintf(stderr, "will delete selected object\n");

  return false;
}

