#include "MyDock.h"
#include "MyFinder.h"

enum PadPage
{
    MainPage,
    LaunchPage
};

struct _MyDock
{
    GtkBox parent_instance;
    GtkBuilder *dock_builder; // Main builder
    GtkWidget *dock_box, *main_box, *finder_box,
        *dock_left, *icons_sw, *main_overlay; // Dock, finder
    GtkWidget *main_pic, *finder;
    GtkWidget *btnlaunch, *launchpad_stack, // launchpad
        *default_page, *launchpad_page;
    PadPage current_page;
};

G_DEFINE_TYPE(MyDock, my_dock, GTK_TYPE_BOX)

static void btnlaunch_clicked(GtkWidget *widget, MyDock *dock)
{
    // Check is launchpad page is shown and switch pages
    if (dock->current_page == MainPage)
    {
        gtk_stack_set_visible_child(GTK_STACK(dock->launchpad_stack),
                                    dock->launchpad_page);
        dock->current_page = LaunchPage;
    }
    else
    {
        gtk_stack_set_visible_child(GTK_STACK(dock->launchpad_stack),
                                    dock->default_page);
        dock->current_page = MainPage;
    }
}

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
    self->launchpad_stack = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "launchpad_stack"));
    self->default_page = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "default_page"));
    self->launchpad_page = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "launchpad_page"));

    // Launchpad default
    self->current_page = MainPage;

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

    // Link Signals
    g_signal_connect(self->btnlaunch, "clicked", G_CALLBACK(btnlaunch_clicked), self);

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
