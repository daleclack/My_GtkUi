#include "MainWin.h"
#include "MainStack.h"
#include "winconf.h"
// #include "winpe.xpm"
// #include "img7.xpm"
#include "image_types.h"

#define winpe "/org/gtk/daleclack/winpe.png"
#define img7 "/org/gtk/daleclack/img7.png"
#define final_approach "/org/gtk/daleclack/final_approach.png"

enum class BackMode
{
    DEFAULT_1,
    DEFAULT_2,
    DEFAULT_3,
    CUSTOM
};

struct _MainWin
{
    GtkApplicationWindow parent;
    GtkWidget *background;
    GtkWidget *overlay;
    GtkGesture *gesture;
    GtkWidget *popover;
    GtkWidget *stack;
    int width, height;
    BackMode back_mode;
};

G_DEFINE_TYPE(MainWin, main_win, GTK_TYPE_APPLICATION_WINDOW)

static void file_dialog_response(GObject *dialog, GAsyncResult *result, gpointer data)
{
    GFile *file;
    MainWin *win = MAIN_WIN(data);

    // Open the file by dialog
    file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(dialog), result, NULL);
    if (file != NULL)
    {
        char *filename = g_file_get_path(file);

        // Set Image from the file
        GdkPixbuf *pixbuf, *sized;
        pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
        sized = gdk_pixbuf_scale_simple(pixbuf, win->width, win->height, GDK_INTERP_BILINEAR);
        GdkTexture *texture = gdk_texture_new_for_pixbuf(sized);
        gtk_picture_set_paintable(GTK_PICTURE(win->background), GDK_PAINTABLE(texture));
        // gtk_picture_set_pixbuf(GTK_PICTURE(win->background), sized); // Warning#2

        // Change Mode and free memory
        win->back_mode = BackMode::CUSTOM;
        g_object_unref(pixbuf);
        g_object_unref(sized);
        g_object_unref(file);
        g_free(filename);
    }
}

static void background_dialog(GSimpleAction *action, GVariant *parmeter, gpointer data)
{
    GtkFileDialog *dialog;
    dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Open Image");

    GListStore *filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
    GtkFileFilter *filter_image = gtk_file_filter_new();
    gtk_file_filter_set_name(filter_image, "Image Files");

    // For Systems supported mime type and not supported, use different way
    if (mime_type_supported())
    {
        gtk_file_filter_add_mime_type(filter_image, "image/*");
    }
    else
    {
        for (int i = 0; supported_globs[i] != NULL; i++)
        {
            const char *glob = supported_globs[i];
            gtk_file_filter_add_pattern(filter_image, glob);
        }
    }
    g_list_store_append(filters, filter_image);

    GtkFileFilter *filter_any = gtk_file_filter_new();
    gtk_file_filter_set_name(filter_any, "Any Files");
    g_list_store_append(filters, filter_any);

    gtk_file_dialog_open(dialog, GTK_WINDOW(data), NULL, file_dialog_response, data);
}

static void logout_activated(GSimpleAction *action, GVariant *parmeter, gpointer data)
{
    MainWin *win = MAIN_WIN(data);
    gtk_stack_set_visible_child_name(GTK_STACK(win->stack), "login_page");
}

static void quit_activated(GSimpleAction *action, GVariant *parmeter, gpointer data)
{
    gtk_window_destroy(GTK_WINDOW(data));
}
static void winsize_activated(GSimpleAction *action, GVariant *parmeter, gpointer data)
{
    MainWin *win = MAIN_WIN(data);
    ConfDlg *dialog = conf_dlg_new(GTK_WINDOW(win));
    gtk_window_present(GTK_WINDOW(dialog));
}

static void set_default_background(gpointer data,
                                   const char *background_name,
                                   BackMode back_mode1)
{
    // Set the background by pixbuf
    MainWin *win = MAIN_WIN(data);
    GdkPixbuf *pixbuf, *sized;
    // pixbuf = gdk_pixbuf_new_from_xpm_data(winpe);
    pixbuf = gdk_pixbuf_new_from_resource(background_name, NULL);
    sized = gdk_pixbuf_scale_simple(pixbuf, win->width, win->height, GDK_INTERP_BILINEAR);
    GdkTexture *texture = gdk_texture_new_for_pixbuf(sized);
    gtk_picture_set_paintable(GTK_PICTURE(win->background), GDK_PAINTABLE(texture));

    // Change Mode and free memory
    win->back_mode = back_mode1;
    g_object_unref(pixbuf);
    g_object_unref(sized);
}

static void default_background1(GSimpleAction *action, GVariant *parmeter, gpointer data)
{
    // Set default background 1
    set_default_background(data, winpe, BackMode::DEFAULT_1);
}

static void default_background2(GSimpleAction *action, GVariant *parmeter, gpointer data)
{
    // Set default background 2
    set_default_background(data, img7, BackMode::DEFAULT_2);
}

static void default_background3(GSimpleAction *action, GVariant *parmeter, gpointer data)
{
    // Set default background 3
    set_default_background(data, final_approach, BackMode::DEFAULT_3);
}

static void refresh_activated(GSimpleAction *action, GVariant *parmeter, gpointer data)
{
    MainWin *win = MAIN_WIN(data);
    // Redraw background image
    switch (win->back_mode)
    {
    case BackMode::DEFAULT_1:
        default_background1(action, parmeter, data);
        break;
    case BackMode::DEFAULT_2:
        default_background2(action, parmeter, data);
        break;
    case BackMode::DEFAULT_3:
        default_background3(action, parmeter, data);
        break;
    case BackMode::CUSTOM:
        break;
    }
}

static void about_activated(GSimpleAction *action, GVariant *parmeter, gpointer data)
{
    GtkWindow *win = GTK_WINDOW(data);
    const char *authors[] = {
        "Dale Clack",
        "GCR_CMake on github https://github.com/Makman2/GCR_CMake",
        NULL};
    char *version;
    version = g_strdup_printf("6.4\nRunning Against GTK %d.%d.%d",
                              gtk_get_major_version(),
                              gtk_get_minor_version(),
                              gtk_get_micro_version());
    GtkWidget *dialog;
    dialog = gtk_about_dialog_new();
    gtk_show_about_dialog(win,
                          "program-name", "My GtkUI Classic",
                          "version", version,
                          "copyright", "© 2019—2024 The Xe Project",
                          "comments", "A program that simulates desktop (Gtk4 Version)",
                          "authors", authors,
                          "license-type", GTK_LICENSE_GPL_3_0,
                          "logo-icon-name", "My_GtkUI",
                          "title", "About My GtkUI (Gtk4 Version)",
                          NULL, nullptr);
    g_free(version);
}

void btnabout_clicked(GtkWidget *widget, gpointer data)
{
    about_activated(NULL, NULL, data);
}

static void gesture_pressed(GtkGestureClick *self, int n_press, double x, double y, MainWin *win)
{
    // When third button is pressed, show popover menu
    GdkRectangle rect;
    rect.x = x;
    rect.y = y;
    rect.height = 1;
    rect.width = 1;
    gtk_popover_set_pointing_to(GTK_POPOVER(win->popover), &rect);
    gtk_popover_popup(GTK_POPOVER(win->popover));
}

GtkWidget *main_win_get_background(MainWin *win)
{
    // Get Background widget, for window size config.
    return win->background;
}

int main_win_get_height(MainWin *win)
{
    // Get the Height of Main Win
    return win->height;
}

int main_win_get_width(MainWin *win)
{
    // Get the width of Main Win
    return win->width;
}

static void main_win_init(MainWin *win)
{
    // For Custom Window Size
    win->width = 640;
    win->height = 360;
    get_config(&(win->width), &(win->height));
    // g_print("%d %d",win->width,win->height);

    // Initailze Window
    gtk_window_set_icon_name(GTK_WINDOW(win), "My_GtkUI");
    gtk_window_set_title(GTK_WINDOW(win), "My GtkUI Classic");

    // Add Actions
    static GActionEntry entries[] = {
        {"back", background_dialog, NULL, NULL, NULL},
        {"default1", default_background1, NULL, NULL, NULL},
        {"default2", default_background2, NULL, NULL, NULL},
        {"default3", default_background3, NULL, NULL, NULL},
        {"refresh", refresh_activated, NULL, NULL, NULL},
        {"about", about_activated, NULL, NULL, NULL},
        {"logout", logout_activated, NULL, NULL, NULL},
        {"quit", quit_activated, NULL, NULL, NULL},
        {"size", winsize_activated, NULL, NULL, NULL}};
    g_action_map_add_action_entries(G_ACTION_MAP(win), entries, G_N_ELEMENTS(entries), win);

    // Add Overlay and background widget
    win->overlay = gtk_overlay_new();
    win->background = gtk_picture_new();
    gtk_widget_set_size_request(win->background, win->width, win->height);
    gtk_overlay_set_child(GTK_OVERLAY(win->overlay), win->background);
    default_background1(NULL, NULL, win);

    // Add Menu
    GtkBuilder *menu_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/appmenu.xml");
    GMenuModel *model = G_MENU_MODEL(gtk_builder_get_object(menu_builder, "app-menu"));
    win->popover = gtk_popover_menu_new_from_model(model);
    gtk_popover_set_has_arrow(GTK_POPOVER(win->popover), FALSE);
    gtk_widget_set_parent(win->popover, win->overlay);

    // Add Gesture to show menu
    win->gesture = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(win->gesture), GDK_BUTTON_SECONDARY);
    g_signal_connect(win->gesture, "pressed", G_CALLBACK(gesture_pressed), win);
    gtk_widget_add_controller(win->overlay, GTK_EVENT_CONTROLLER(win->gesture));

    // Add Main Page
    win->stack = create_main_stack(win, model);
    gtk_widget_set_halign(GTK_WIDGET(win->stack), GTK_ALIGN_FILL);
    gtk_widget_set_valign(GTK_WIDGET(win->stack), GTK_ALIGN_FILL);
    gtk_overlay_add_overlay(GTK_OVERLAY(win->overlay), GTK_WIDGET(win->stack));

    gtk_window_set_child(GTK_WINDOW(win), win->overlay);
    g_object_unref(menu_builder);
}

static void main_win_class_init(MainWinClass *klass) {}

MainWin *main_win_new(GtkApplication *app)
{
    return (MainWin *)g_object_new(main_win_get_type(), "application", app, NULL);
}
