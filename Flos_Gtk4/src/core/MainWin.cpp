#include "MainWin.h"
#include "MyDock.h"
#include <ctime>

struct _MainWin
{
    GtkApplicationWindow parent_instance;
    GtkWidget *overlay;
    GtkWidget *background;
    GtkStyleProvider *provider;
    GtkWidget *main_grid;
};

G_DEFINE_TYPE(MainWin, main_win, GTK_TYPE_APPLICATION_WINDOW)

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

static void main_win_init(MainWin *win)
{
    // Initalize window
    gtk_window_set_title(GTK_WINDOW(win), "My GtkUI Flos Version");
    gtk_window_set_icon_name(GTK_WINDOW(win), "My_GtkUI");

    // Create widgets
    win->overlay = gtk_overlay_new();
    win->background = gtk_picture_new();
    gtk_widget_set_size_request(win->overlay, 1024, 576);

    // Add Background
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_resource("/org/gtk/daleclack/flos.png", NULL);
    GdkPixbuf *sized = gdk_pixbuf_scale_simple(pixbuf, 1024, 576, GDK_INTERP_BILINEAR);
    gtk_picture_set_pixbuf(GTK_PICTURE(win->background), sized);
    g_object_unref(pixbuf);
    g_object_unref(sized);

    // Add MenuBar
    GtkBuilder *builder = gtk_builder_new_from_resource("/org/gtk/daleclack/menubar.xml");
    GMenuModel *model = G_MENU_MODEL(gtk_builder_get_object(builder, "model"));
    GtkWidget *menubar = gtk_popover_menu_bar_new_from_model(model);
    // gtk_button_set_child(GTK_BUTTON(back_button),menubar);

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
    g_signal_connect(home_button,"clicked",G_CALLBACK(btnhome_clicked),win);

    // Attach the button to grid
    gtk_grid_attach(GTK_GRID(win->main_grid), home_button, 1, 1, 1, 1);

    // Add menubar and grid to the overlay
    GtkWidget *desktop_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_append(GTK_BOX(desktop_box), menubar);
    gtk_box_append(GTK_BOX(desktop_box), win->main_grid);
    gtk_widget_set_valign(desktop_box, GTK_ALIGN_START);
    gtk_widget_set_halign(desktop_box, GTK_ALIGN_FILL);
    gtk_overlay_add_overlay(GTK_OVERLAY(win->overlay), desktop_box);

    // Apply Style for menubar and the button
    win->provider = GTK_STYLE_PROVIDER(gtk_css_provider_new());
    gtk_css_provider_load_from_resource(GTK_CSS_PROVIDER(win->provider), "/org/gtk/daleclack/style_dark.css");
    gtk_style_context_add_provider(gtk_widget_get_style_context(menubar), win->provider, G_MAXINT);
    gtk_style_context_add_provider(gtk_widget_get_style_context(home_button), win->provider, G_MAXINT);

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
