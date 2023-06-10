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
    GtkBuilder *menu_builder;
    GMenuModel *menu_model;
    GtkGesture *gesture;
    GtkWidget *context_menu; // Context menu
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

static void pressed(GtkGesture *gesture, int n_press,
                    double x, double y, MyDock *dock)
{
    // Show the context menu at the mouse position
    GdkRectangle rectangle = {(int)x, (int)y, 1, 1};
    gtk_popover_set_pointing_to(GTK_POPOVER(dock->context_menu), &rectangle);
    gtk_popover_popup(GTK_POPOVER(dock->context_menu));
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

    // Add Gesture to control the menu
    self->gesture = gtk_gesture_click_new();
    gtk_widget_add_controller(self->main_overlay, GTK_EVENT_CONTROLLER(self->gesture));
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(self->gesture), GDK_BUTTON_SECONDARY);
    g_signal_connect(self->gesture, "pressed", G_CALLBACK(pressed), self);

    // Add context menu
    self->menu_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/mainmenu.xml");
    self->menu_model = G_MENU_MODEL(gtk_builder_get_object(self->menu_builder, "model"));
    self->context_menu = gtk_popover_menu_new_from_model(self->menu_model);
    gtk_popover_set_has_arrow(GTK_POPOVER(self->context_menu), FALSE);
    gtk_widget_set_parent(self->context_menu, self->main_overlay);

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
