#include "MyDock.h"
#include "MyFinder.h"

struct _MyDock
{
    GtkBox parent_instance;
    GtkBuilder *dock_builder;
    GtkWidget *dock_box, *main_box, *finder_box,
        *dock_left, *icons_sw, *main_overlay;
    GtkWidget *main_pic, *finder;
    GtkWidget *btnlaunch, *launchpad_stack,
        *default_page, *launchpad_page;
};

G_DEFINE_TYPE(MyDock, my_dock, GTK_TYPE_BOX)

static void my_dock_init(MyDock *self)
{
    // Builder for the main dock
    self->dock_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/mydock.ui");

    // Get widgets
    self->dock_box = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "dock_box"));
    self->main_box = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "main_box"));
    self->finder_box = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "finder_box"));
    self->dock_left = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "dock_left"));
    self->icons_sw = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "icons_sw"));
    self->btnlaunch = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "btnlaunch"));

    // Add background widget
    self->main_overlay = gtk_overlay_new();

    // Set default background
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_resource("/org/gtk/daleclack/final_approach.png", NULL);
    GdkPixbuf *sized = gdk_pixbuf_scale_simple(pixbuf, 1024, 576, GDK_INTERP_BILINEAR);
    self->main_pic = gtk_picture_new_for_pixbuf(sized);
    g_object_unref(pixbuf);
    g_object_unref(sized);

    // Add finder
    self->finder = my_finder_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(self->finder_box), self->finder);

    // Pack widgets

    // Dock position and mode, will be changable soon
    gtk_widget_set_vexpand(self->icons_sw, TRUE);
    gtk_widget_set_valign(self->icons_sw, GTK_ALIGN_FILL);
    gtk_widget_set_valign(self->dock_box, GTK_ALIGN_FILL);
    gtk_box_append(GTK_BOX(self->dock_left), self->dock_box);

    // Others
    gtk_overlay_set_child(GTK_OVERLAY(self->main_overlay), self->main_pic);
    gtk_overlay_add_overlay(GTK_OVERLAY(self->main_overlay), self->main_box);
    gtk_box_append(GTK_BOX(self), self->main_overlay);
}

static void my_dock_class_init(MyDockClass *klass)
{
}

GtkWidget *my_dock_new()
{
    return GTK_WIDGET(g_object_new(my_dock_get_type(), NULL));
}
