#include "MyDock.h"
#include "MyFinder.h"

void add_dock(GtkWidget *main_page1)
{
    // Builder for the main dock
    GtkBuilder *dock_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/mydock.ui");

    // Get widgets
    GtkWidget *dock_box = GTK_WIDGET(gtk_builder_get_object(dock_builder, "dock_box"));
    GtkWidget *main_box = GTK_WIDGET(gtk_builder_get_object(dock_builder, "main_box"));
    GtkWidget *finder_box = GTK_WIDGET(gtk_builder_get_object(dock_builder, "finder_box"));
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

    // Add finder
    GtkWidget *finder = my_finder_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(finder_box), finder);

    // Pack widgets

    // Dock position and mode, will be changable soon
    gtk_widget_set_vexpand(icons_sw, TRUE);
    gtk_widget_set_valign(icons_sw, GTK_ALIGN_FILL);
    gtk_widget_set_valign(dock_box, GTK_ALIGN_FILL);
    gtk_box_append(GTK_BOX(dock_left), dock_box);

    // Others
    gtk_overlay_set_child(GTK_OVERLAY(main_overlay), main_pic);
    gtk_overlay_add_overlay(GTK_OVERLAY(main_overlay), main_box);
    gtk_box_append(GTK_BOX(main_page1), main_overlay);
}
