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
    GtkWindow *parent_win;
    GtkBuilder *dock_builder; // Main builder
    GtkWidget *dock_box, *main_box, *finder_box,
        *dock_left, *icons_sw, *main_overlay; // Dock, finder
    GtkWidget *main_pic, *finder;
    GtkWidget *btnlaunch, *launchpad_stack, // launchpad
        *default_page, *launchpad_page;
    GtkWidget *btnfiles, *btndraw, *btncalc, *btnedit, *btnimage, // Dock buttons
        *btnset, *btngame, *btngame24, *btnmine;
    GtkWidget *image_file, *image_draw, *image_calc, *image_game, // Image widget for dock buttons
        *image_edit, *image_viewer, *image_game24, *image_mine, *image_set;
    GtkWidget *padabout, *padaud, *paddraw, *padfile, *padgedit, // Launchpad icons
        *padgame, *padimage, *padnote, *padedit, *padvlc, *padvlc_win32,
        *padrun, *padset, *padgame24, *padcalc, *padmine;
    PadPage current_page;
    GtkBuilder *menu_builder;
    GMenuModel *menu_model;
    GtkGesture *gesture;
    GtkWidget *context_menu; // Context menu
    MyPrefs *prefs_win;      // Prefs window
};

G_DEFINE_TYPE(MyDock, my_dock, GTK_TYPE_BOX)

// Launchpad control
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

// The context menu control
static void pressed(GtkGesture *gesture, int n_press,
                    double x, double y, MyDock *dock)
{
    // Show the context menu at the mouse position
    GdkRectangle rectangle = {(int)x, (int)y, 1, 1};
    gtk_popover_set_pointing_to(GTK_POPOVER(dock->context_menu), &rectangle);
    gtk_popover_popup(GTK_POPOVER(dock->context_menu));
}

// Window control func
static void window_ctrl(GtkWindow *window, GtkWindow *parent)
{
    // Get GdkSurface for window state
    GdkSurface *surface = gtk_native_get_surface(GTK_NATIVE(window));
    if (surface)
    {
        // The state will available when the window open
        unsigned short state = gdk_toplevel_get_state(GDK_TOPLEVEL(surface));
        switch (state)
        {
        // Minimized
        case GDK_TOPLEVEL_STATE_MINIMIZED:
            gtk_window_set_transient_for(window, parent);
            gtk_window_unminimize(window);
            break;
        default:
            // The controlled window is on dock
            gtk_window_set_transient_for(window, NULL);
            gtk_window_minimize(window);
            break;
        }
    }
    else
    {
        // Create a window
        gtk_window_set_transient_for(window, parent);
        gtk_window_present(window);
    }
}

static void padset_clicked(GtkWidget *widget, MyDock *dock)
{
    // When the window visible, unminimize it
    if (gtk_widget_get_visible(GTK_WIDGET((dock->prefs_win))))
    {
        gtk_window_unminimize(GTK_WINDOW(dock->prefs_win));
    }
    else
    {
        // Show the window
        gtk_window_set_transient_for(GTK_WINDOW(dock->prefs_win), dock->parent_win);
        gtk_window_present(GTK_WINDOW(dock->prefs_win));
    }
    gtk_image_set_from_icon_name(GTK_IMAGE(dock->image_set), "my_prefs_running");
    btnlaunch_clicked(NULL, dock);
}

static void btnset_clicked(GtkWidget *widget, MyDock *dock)
{
    // When the window visible, control the window state
    if (gtk_widget_get_visible(GTK_WIDGET((dock->prefs_win))))
    {
        window_ctrl(GTK_WINDOW(dock->prefs_win), dock->parent_win);
    }
    else
    {
        // Show the window
        gtk_window_set_transient_for(GTK_WINDOW(dock->prefs_win), dock->parent_win);
        gtk_window_present(GTK_WINDOW(dock->prefs_win));
    }
    gtk_image_set_from_icon_name(GTK_IMAGE(dock->image_set), "my_prefs_running");
}

static gboolean prefs_win_closed(GtkWindow *window, MyDock *dock)
{
    // For the preferences window, hide it
    gtk_widget_set_visible(GTK_WIDGET(dock->prefs_win), FALSE);
    gtk_image_set_from_icon_name(GTK_IMAGE(dock->image_set), "my_prefs");
    return TRUE;
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
    self->btnfiles = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "btnfiles"));
    self->image_file = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "image_file"));
    self->btncalc = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "btncalc"));
    self->image_calc = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "image_calc"));
    self->btndraw = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "btndraw"));
    self->image_draw = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "image_draw"));
    self->btnedit = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "btnedit"));
    self->image_edit = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "image_edit"));
    self->btngame24 = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "btngame24"));
    self->image_game24 = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "image_game24"));
    self->btngame = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "btngame"));
    self->image_game = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "image_game"));
    self->btnimage = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "btnimage"));
    self->image_viewer = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "image_viewer"));
    self->btnmine = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "btnmine"));
    self->image_mine = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "image_mine"));
    self->btnset = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "btnset"));
    self->image_set = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "image_set"));
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

    // Pack widgets
    // Dock position and mode, will be changable soon
    gtk_widget_set_vexpand(self->icons_sw, TRUE);
    gtk_widget_set_valign(self->icons_sw, GTK_ALIGN_FILL);
    gtk_widget_set_valign(self->dock_box, GTK_ALIGN_FILL);
    gtk_box_append(GTK_BOX(self->dock_left), self->dock_box);

    // The Main widget, for background
    gtk_overlay_set_child(GTK_OVERLAY(self->main_overlay), self->main_pic);
    gtk_overlay_add_overlay(GTK_OVERLAY(self->main_overlay), self->main_box);
    gtk_box_append(GTK_BOX(self), self->main_overlay);

    // Create prefs window
    self->prefs_win = my_prefs_new(self->main_pic);

    // Link Signals
    g_signal_connect(self->btnlaunch, "clicked", G_CALLBACK(btnlaunch_clicked), self);
    g_signal_connect(self->btnset, "clicked", G_CALLBACK(btnset_clicked), self);
    g_signal_connect(self->padset, "clicked", G_CALLBACK(padset_clicked), self);
    g_signal_connect(self->prefs_win, "close-request", G_CALLBACK(prefs_win_closed), self);

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

    // Add Style to the Dock
    gtk_widget_add_css_class(self->dock_left, "dock_style");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(self->dock_left),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // The widget in the dock should use default style
    gtk_widget_add_css_class(self->dock_box, "default_style");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(self->dock_box),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

static void my_dock_class_init(MyDockClass *klass)
{
}

GtkWidget *my_dock_get_background(MyDock *dock)
{
    return dock->main_pic;
}

GtkWidget *my_dock_new(GtkWindow *parent)
{
    // Create the dock and set parent window
    GtkWidget *dock = GTK_WIDGET(g_object_new(my_dock_get_type(), NULL));
    MY_DOCK(dock)->parent_win = parent;
    return dock;
}

MyPrefs *my_dock_get_prefs(MyDock *dock)
{
    // Get the prefs window
    return dock->prefs_win;
}
