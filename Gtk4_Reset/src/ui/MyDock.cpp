#include "MyDock.h"
#include "MyFinder.h"
#include "AppView.h"

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
    GtkWidget *dock_box, *main_box, *finder_box, *icons_box,
        *dock_left, *dock_right, *dock_bottom, *icons_sw, *main_overlay; // Dock, finder
    GtkWidget *main_pic, *finder;
    GtkWidget *btnlaunch, *launchpad_stack, *default_page, *apps_grid, // launchpad
        *launchpad_page, *apps_stack, *default_box, *addon_box,
        *apps_switcher, *apps_view, *appgrid_box, *appgrid_label;
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
    my_prefs_first_load(dock->prefs_win);
    my_prefs_start_scan(dock->prefs_win);
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
    my_prefs_first_load(dock->prefs_win);
    my_prefs_start_scan(dock->prefs_win);
    gtk_image_set_from_icon_name(GTK_IMAGE(dock->image_set), "my_prefs_running");
}

static gboolean prefs_win_closed(GtkWindow *window, MyDock *dock)
{
    // For the preferences window, hide it
    gtk_widget_set_visible(GTK_WIDGET(dock->prefs_win), FALSE);
    gtk_image_set_from_icon_name(GTK_IMAGE(dock->image_set), "my_prefs");
    return TRUE;
}

static void my_dock_get_widgets(MyDock *self)
{
    // Get widgets
    self->dock_box = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "dock_box"));
    self->main_box = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "main_box"));
    self->finder_box = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "finder_box"));
    self->dock_left = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "dock_left"));
    self->dock_right = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "dock_right"));
    self->dock_bottom = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "dock_bottom"));
    self->icons_sw = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "icons_sw"));
    self->icons_box = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "icons_box"));
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
    self->apps_grid = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "apps_grid"));
    self->apps_stack = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "apps_stack"));
    self->default_box = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "default_box"));
    self->addon_box = GTK_WIDGET(gtk_builder_get_object(self->dock_builder, "addon_box"));
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
}

static void my_dock_init(MyDock *self)
{
    // Builder for the main dock
    self->dock_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/mydock.ui");

    my_dock_get_widgets(self);

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
    self->main_pic = gtk_picture_new();
    // gtk_widget_set_size_request(self->main_pic, 1024, 576);
    // GdkPixbuf *pixbuf = gdk_pixbuf_new_from_resource("/org/gtk/daleclack/final_approach.png", NULL);
    // GdkPixbuf *sized = gdk_pixbuf_scale_simple(pixbuf, 1024, 576, GDK_INTERP_BILINEAR);
    // GdkTexture *texture = gdk_texture_new_for_pixbuf(sized);
    // self->main_pic = gtk_picture_new_for_paintable(GDK_PAINTABLE(texture));
    // g_object_unref(pixbuf);
    // g_object_unref(sized);

    // Create prefs window
    self->prefs_win = my_prefs_new();
    my_prefs_set_background(self->prefs_win, self->main_pic);

    // Add finder
    self->finder = my_finder_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(self->finder_box), self->finder);

    // Pack widgets for dock

    // Dock position and mode
    DockPos dock_pos = my_prefs_get_dock_pos(self->prefs_win);
    switch (dock_pos)
    {
    case DockPos::Pos_Left:
        gtk_widget_set_vexpand(self->icons_sw, TRUE);
        gtk_widget_set_valign(self->icons_sw, GTK_ALIGN_FILL);
        gtk_widget_set_valign(self->dock_box, GTK_ALIGN_FILL);
        gtk_box_append(GTK_BOX(self->dock_left), self->dock_box);
        break;
    case DockPos::Pos_Right:
        gtk_widget_set_vexpand(self->icons_sw, TRUE);
        gtk_widget_set_valign(self->icons_sw, GTK_ALIGN_FILL);
        gtk_widget_set_valign(self->dock_box, GTK_ALIGN_FILL);
        gtk_box_append(GTK_BOX(self->dock_right), self->dock_box);
        break;
    case DockPos::Pos_Bottom:
        gtk_orientable_set_orientation(GTK_ORIENTABLE(self->dock_box),
                                       GTK_ORIENTATION_HORIZONTAL);
        gtk_orientable_set_orientation(GTK_ORIENTABLE(self->icons_box),
                                       GTK_ORIENTATION_HORIZONTAL);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(self->icons_sw),
                                       GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
        gtk_widget_set_hexpand(self->icons_sw, TRUE);
        gtk_widget_set_halign(self->icons_sw, GTK_ALIGN_FILL);
        gtk_widget_set_halign(self->dock_box, GTK_ALIGN_FILL);
        gtk_box_append(GTK_BOX(self->dock_bottom), self->dock_box);
        break;
    }

    // The Main widget, for background
    gtk_overlay_set_child(GTK_OVERLAY(self->main_overlay), self->main_pic);
    gtk_overlay_add_overlay(GTK_OVERLAY(self->main_overlay), self->main_box);
    gtk_box_append(GTK_BOX(self), self->main_overlay);

    // Link Signals
    g_signal_connect(self->btnlaunch, "clicked", G_CALLBACK(btnlaunch_clicked), self);
    g_signal_connect(self->btnset, "clicked", G_CALLBACK(btnset_clicked), self);
    g_signal_connect(self->padset, "clicked", G_CALLBACK(padset_clicked), self);
    g_signal_connect(self->prefs_win, "close-request", G_CALLBACK(prefs_win_closed), self);

    // Create Css Provider for styling
    GtkCssProvider *provider = gtk_css_provider_new();
    if (get_dark_mode(self))
    {
        gtk_css_provider_load_from_resource(provider, "/org/gtk/daleclack/style_dark.css");
    }
    else
    {
        gtk_css_provider_load_from_resource(provider, "/org/gtk/daleclack/style.css");
    }
    // Add Style for finder
    my_finder_add_style(MY_FINDER(self->finder), provider);

    // Add Apps grid

    // To make the default view layout same as the addon apps view
    self->appgrid_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    self->appgrid_label = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(self->appgrid_box), self->appgrid_label);
    gtk_box_append(GTK_BOX(self->appgrid_box), self->apps_grid);
    gtk_box_append(GTK_BOX(self->default_box), self->appgrid_box);
    gtk_widget_set_halign(self->default_box, GTK_ALIGN_CENTER);
    gtk_grid_set_row_spacing(GTK_GRID(self->apps_grid), 20);

    // Add Addon apps view
    self->apps_view = app_view_new();
    gtk_box_append(GTK_BOX(self->addon_box), self->apps_view);
    gtk_widget_set_halign(self->addon_box, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(self->apps_view, GTK_ALIGN_CENTER);

    // Add a switcher for the apps shown
    GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(self->launchpad_page), 0, 0);
    self->apps_switcher = gtk_stack_switcher_new();
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(self->apps_switcher),
                                 GTK_STACK(self->apps_stack));
    gtk_widget_set_halign(self->apps_switcher, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(self->apps_switcher, GTK_ALIGN_END);
    gtk_widget_set_halign(self->apps_grid, GTK_ALIGN_FILL);
    gtk_widget_set_valign(self->apps_grid, GTK_ALIGN_FILL);
    gtk_grid_attach(GTK_GRID(child), self->apps_switcher, 1, 2, 1, 1);

    // Add Style to launchpad page
    gtk_widget_add_css_class(self->launchpad_page, "dock_style");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(self->launchpad_page),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // The widget in the launchpad page should use default style
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

    // The widget in the dock should use default style
    gtk_widget_add_css_class(self->apps_switcher, "default_style");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(self->apps_switcher),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // The widget in the dock should use default style
    gtk_widget_add_css_class(self->apps_view, "default_style");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(self->apps_view),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // // The style for context menu
    // gtk_widget_add_css_class(self->context_menu, "default_style");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(self->context_menu),
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
