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
    GtkWidget *padabout, *padaud, *paddraw, *padfile, *padgedit,    // Launchpad icons
        *padgame, *padimage, *padnote, *padedit, *padvlc, *padvlc_win32,
        *padrun, *padset, *padgame24, *padcalc, *padmine;
    PadPage current_page;
    GtkBuilder *menu_builder;
    GMenuModel *menu_model;
    GtkGesture *gesture;
    GtkWidget *context_menu; // Context menu
};

G_DEFINE_TYPE(MyDock, my_dock, GTK_TYPE_BOX)

void hide_launchpad(MyDock *dock)
{
    // Hide the launchpad
    gtk_stack_set_visible_child(GTK_STACK(dock->launchpad_stack),
                                dock->default_page);
    dock->current_page = MainPage;
}

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
    self->padaud = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "padaud"));
    self->padabout = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "padabout"));
    self->padcalc = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "padcalc"));
    self->paddraw = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "paddraw"));
    self->padedit = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "padedit"));
    self->padfile = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "padfile"));
    self->padgame24 = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "padgame24"));
    self->padgame = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "padgame"));
    self->padgedit = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "padgedit"));
    self->padimage = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "padimage"));
    self->padmine = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "padmine"));
    self->padnote = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "padnote"));
    self->padrun = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "padrun"));
    self->padset = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "padset"));
    self->padvlc = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "padvlc"));
    self->padvlc_win32 = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "padvlc_win32"));

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
    GdkTexture *texture = gdk_texture_new_for_pixbuf(sized);
    self->main_pic = gtk_picture_new_for_paintable(GDK_PAINTABLE(texture));
    g_object_unref(pixbuf);
    g_object_unref(sized);

    // Add finder
    self->finder = my_finder_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(self->finder_box), self->finder);

    // Link Signals
    g_signal_connect(self->btnlaunch, "clicked", G_CALLBACK(btnlaunch_clicked), self);

    // Create Css Provider for styling 
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(provider, "/org/gtk/daleclack/style.css");

    // Add Style for finder
    my_finder_add_style(MY_FINDER(self->finder), provider);

    // Add Style to launchpad page
    gtk_widget_add_css_class(self->launchpad_page, "dock_style");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(self->launchpad_page),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // The widget in the launchpad page should use default style
    GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(self->launchpad_page), 0, 0);
    gtk_widget_add_css_class(child, "default_style");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(child),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

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

GtkWidget *my_dock_get_background(MyDock *dock)
{
    return dock->main_pic;
}

GtkWidget *my_dock_new()
{
    return GTK_WIDGET(g_object_new(my_dock_get_type(), NULL));
}
