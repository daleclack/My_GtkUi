#include "MainWin.h"
#include "MyDock.h"
#include "src/toml.hpp"
#include "AboutWin.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>

struct _MainWin
{
    GtkApplicationWindow parent_instance;
    GtkWidget *overlay;
    GtkWidget *background;
    GtkStyleProvider *provider;
    GtkWidget *main_grid;
    GtkWidget *context_menu;
    GtkGesture *gesture_click;
    bool dark_mode;
};

G_DEFINE_TYPE(MainWin, main_win, GTK_TYPE_APPLICATION_WINDOW)

static void quit_activated(GSimpleAction *action,
                           GVariant *parmeter,
                           gpointer data)
{
    // Quit the UI interface
    gtk_window_destroy(GTK_WINDOW(data));
}

static void about_activated(GSimpleAction *action,
                            GVariant *parmeter,
                            gpointer data)
{
    AboutWin *win = about_win_new();
    gtk_window_present(GTK_WINDOW(win));
}

static gboolean label_timer(gpointer data)
{
    // Get Local Time
    time_t t;
    t = time(NULL);
    struct tm *local;
    local = localtime(&t);

    // Set Label time
    char buf[57];
    sprintf(buf, "%02d:%02d %d/%d/%d", local->tm_hour,
            local->tm_min, local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
    gtk_label_set_label(GTK_LABEL(data), buf);

    return TRUE;
}

// These are not working well now :(
// static void check_dark_toggled(GtkCheckButton *button, gpointer data)
// {
//     // Create toml data
//     static constexpr std::string_view toml_str = R"(
//         [interface]
//         dark_mode = false
//     )";
//     toml::table tbl;

//     try
//     {
//         // Assign value for the key
//         gboolean dark_mode = gtk_check_button_get_active(button);
//         tbl = toml::parse(toml_str);
//         auto tbl1 = tbl["interface"].as_table();
//         tbl1->insert_or_assign("dark_mode", (bool)dark_mode);

//         // Save content to a file
//         std::fstream outfile;
//         outfile.open("config.toml", std::ios_base::out);
//         if (outfile.is_open())
//         {
//             outfile << tbl;
//         }
//         outfile.close();
//     }
//     catch (const toml::parse_error &err)
//     {
//         // If error occurs, output the error
//         std::cout << err.what() << std::endl;
//     }
// }

// static bool get_dark_mode()
// {
//     bool dark_mode = false;
//     // Open config file
//     std::fstream infile;
//     infile.open("config.toml", std::ios_base::in);
//     if (infile.is_open())
//     {
//         try
//         {
//             // Get value from toml data
//             toml::table tbl = toml::parse(infile);
//             dark_mode = tbl["interface"]["dark_mode"].as_boolean();
//         }
//         catch (const toml::parse_error &err)
//         {
//             toml::table tbl = toml::parse(infile);
//         }
//     }
//     infile.close();
//     return dark_mode;
// }

static void gesture_pressed(GtkGestureSingle *gesture, 
                            int n_press, 
                            double x, 
                            double y, 
                            MainWin *win)
{   // for right-click process
    GdkRectangle rect = {(int)x, (int)y, 1, 1};     // Set popover position
    gtk_popover_set_pointing_to(GTK_POPOVER(win->context_menu), &rect);
    gtk_popover_popup(GTK_POPOVER(win->context_menu));  // Show popover
}

static void main_win_init(MainWin *win)
{
    // Initalize window
    gtk_window_set_title(GTK_WINDOW(win), "My GtkUI Flos Version");
    gtk_window_set_icon_name(GTK_WINDOW(win), "My_GtkUI");
    // win->dark_mode = get_dark_mode();

    // Create widgets
    win->overlay = gtk_overlay_new();
    win->background = gtk_picture_new();
    gtk_widget_set_size_request(win->overlay, 1024, 576);

    // Add Background
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_resource("/org/gtk/daleclack/flos.png", NULL);
    GdkPixbuf *sized = gdk_pixbuf_scale_simple(pixbuf, 1024, 576, GDK_INTERP_BILINEAR);
    GdkTexture *texture = gdk_texture_new_for_pixbuf(sized);
    gtk_picture_set_paintable(GTK_PICTURE(win->background), GDK_PAINTABLE(texture));
    // gtk_picture_set_pixbuf(GTK_PICTURE(win->background), sized); Deprecated with gtk4.12
    g_object_unref(pixbuf);
    g_object_unref(sized);

    // Add MenuBar
    GtkBuilder *builder = gtk_builder_new_from_resource("/org/gtk/daleclack/menubar.xml");
    GMenuModel *model = G_MENU_MODEL(gtk_builder_get_object(builder, "model"));
    GtkWidget *menubar = gtk_popover_menu_bar_new_from_model(model);
    // gtk_popover_set_position(GTK_POPOVER(menubar), GTK_POS_TOP);
    // gtk_button_set_child(GTK_BUTTON(back_button),menubar);

    // Add actions
    static GActionEntry action_entries[] = {
        {"quit", quit_activated, NULL, NULL, NULL},
        {"about", about_activated, NULL, NULL, NULL}};
    g_action_map_add_action_entries(G_ACTION_MAP(win), action_entries, G_N_ELEMENTS(action_entries), win);

    // Add Check Button for dark mode
    GtkWidget *check_dark = gtk_check_button_new_with_label("Dark Mode");
    gtk_check_button_set_active(GTK_CHECK_BUTTON(check_dark), win->dark_mode);
    gtk_popover_menu_bar_add_child(GTK_POPOVER_MENU_BAR(menubar), check_dark, "check_dark");
    // g_signal_connect(check_dark, "toggled", G_CALLBACK(check_dark_toggled), NULL);

    // Add a grid
    win->main_grid = gtk_grid_new();

    // Create a "Home" Button
    GtkWidget *home_button = gtk_button_new();
    GtkWidget *home_image = gtk_image_new_from_icon_name("user-home");
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *button_label = gtk_label_new("dale");

    gtk_image_set_pixel_size(GTK_IMAGE(home_image), 48);
    gtk_box_append(GTK_BOX(button_box), home_image);
    gtk_box_append(GTK_BOX(button_box), button_label);
    gtk_button_set_child(GTK_BUTTON(home_button), button_box);

    gtk_button_set_has_frame(GTK_BUTTON(home_button), FALSE);
    gtk_widget_set_margin_top(home_button, 15);
    gtk_widget_set_margin_start(home_button, 25);
    g_signal_connect(home_button, "clicked", G_CALLBACK(btnhome_clicked), win);

    // Attach the button to grid
    gtk_grid_attach(GTK_GRID(win->main_grid), home_button, 1, 1, 1, 1);

    // Add menubar and grid to the overlay
    GtkWidget *desktop_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_append(GTK_BOX(desktop_box), menubar);
    gtk_box_append(GTK_BOX(desktop_box), win->main_grid);
    gtk_widget_set_valign(desktop_box, GTK_ALIGN_START);
    gtk_widget_set_halign(desktop_box, GTK_ALIGN_FILL);
    gtk_overlay_add_overlay(GTK_OVERLAY(win->overlay), desktop_box);

    // Add a gesture for right click
    win->gesture_click = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(win->gesture_click), GDK_BUTTON_SECONDARY);
    g_signal_connect(win->gesture_click, "pressed", G_CALLBACK(gesture_pressed), win);
    gtk_widget_add_controller(win->overlay, GTK_EVENT_CONTROLLER(win->gesture_click));

    // Create context menu
    GtkBuilder *menu_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/more_menu.xml");
    GMenuModel *menu_model = G_MENU_MODEL(gtk_builder_get_object(menu_builder, "more_menu"));
    win->context_menu = gtk_popover_menu_new_from_model(menu_model);
    gtk_popover_set_has_arrow(GTK_POPOVER(win->context_menu), FALSE);
    gtk_widget_set_parent(win->context_menu, win->overlay);

    // Apply Style for menubar and the button
    gtk_widget_add_css_class(GTK_WIDGET(menubar), "main_style");
    gtk_widget_add_css_class(home_button, "btn_style");
    // gtk_widget_add_css_class(GTK_WIDGET(home_button), "main_style");
    win->provider = GTK_STYLE_PROVIDER(gtk_css_provider_new());
    // if (win->dark_mode)
    // {
        gtk_css_provider_load_from_resource(GTK_CSS_PROVIDER(win->provider), "/org/gtk/daleclack/style_dark.css");
    // }
    // else
    // {
    //     gtk_css_provider_load_from_resource(GTK_CSS_PROVIDER(win->provider), "/org/gtk/daleclack/style.css");
    // }
    // gtk_widget_set_opacity(menubar, 0.7);
    gtk_style_context_add_provider((menubar), win->provider, G_MAXINT);
    gtk_style_context_add_provider((home_button), win->provider, G_MAXINT);

    // Add Label for time
    GtkWidget *time_label = gtk_label_new("12:21 2022/1/9");
    g_timeout_add(1000, label_timer, time_label);

    gtk_widget_set_halign(time_label, GTK_ALIGN_END);
    gtk_widget_set_valign(time_label, GTK_ALIGN_START);

    // Set Position of the label
    gtk_widget_set_margin_bottom(time_label, 3);
    gtk_widget_set_margin_start(time_label, 3);
    gtk_widget_set_margin_end(time_label, 10);
    gtk_widget_set_margin_top(time_label, 3);
    gtk_overlay_add_overlay(GTK_OVERLAY(win->overlay), time_label);

    // Add a dock
    add_dock(win);

    // Add widgets
    gtk_overlay_set_child(GTK_OVERLAY(win->overlay), win->background);
    gtk_window_set_child(GTK_WINDOW(win), win->overlay);
}

static void main_win_class_init(MainWinClass *klass) {}

MainWin *main_win_new(GtkApplication *app)
{
    return (MainWin *)g_object_new(main_win_get_type(), "application", app, NULL);
}

GtkOverlay *main_win_get_overlay(MainWin *win)
{
    // Get the Overlay of the window
    return GTK_OVERLAY(win->overlay);
}

GtkStyleProvider *main_win_get_style(MainWin *win)
{
    // Get Style Provider
    return win->provider;
}

bool main_win_get_dark_mode(MainWin *win)
{
    // Get whether use dark mode
    return win->dark_mode;
}
