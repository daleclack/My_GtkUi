#include "MyDock.h"

void add_dock(GtkWidget *main_page1)
{
    // Builder for the main dock
    GtkBuilder *dock_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/mydock.ui");

    // Get widgets
    GtkWidget *dock_box = GTK_WIDGET(gtk_builder_get_object(dock_builder, "dock_box"));
    GtkWidget *main_box = GTK_WIDGET(gtk_builder_get_object(dock_builder, "main_box"));
    GtkWidget *dock_left = GTK_WIDGET(gtk_builder_get_object(dock_builder, "dock_left"));
    GtkWidget *icons_sw = GTK_WIDGET(gtk_builder_get_object(dock_builder, "icons_sw"));

    // Add background widget
    GtkWidget *main_overlay = gtk_overlay_new();

    // Set default background
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_resource("/org/gtk/daleclack/final_approach.png", NULL);
    GdkPixbuf *sized = gdk_pixbuf_scale_simple(pixbuf, 1024, 576, GDK_INTERP_BILINEAR);
    GtkWidget *main_pic = gtk_picture_new_for_pixbuf(sized);
    g_object_unref(pixbuf);
    g_object_unref(sized);

    // Pack widgets
    gtk_widget_set_size_request(icons_sw, -1, 300);
    gtk_box_append(GTK_BOX(dock_left), dock_box);   // Dock position, will be changable soon
    gtk_overlay_set_child(GTK_OVERLAY(main_overlay), main_pic);
    gtk_overlay_add_overlay(GTK_OVERLAY(main_overlay), main_box);
    gtk_box_append(GTK_BOX(main_page1), main_overlay);
}
