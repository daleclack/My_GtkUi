#include "MainWin.h"
#include "MyStack.h"
#include "MyDock.h"
#include "MyPrefs.h"
#include "winpe.xpm"

static GtkWidget *dock;

static void btnlogin_clicked(GtkWidget *widget, GtkStack *stack)
{
    gtk_stack_set_visible_child_name(stack, "main_page1");
}

static void back_activated(GSimpleAction *actions,
                           GVariant *parmeter,
                           gpointer win)
{
    // Show preferences window
    MyPrefs *prefs = main_win_get_prefs(MAIN_WIN(win));
    my_prefs_first_load(prefs);
    my_prefs_start_scan(prefs);
    gtk_window_present(GTK_WINDOW(prefs));
}

static void logout_activated(GSimpleAction *action,
                             GVariant *parmeter,
                             gpointer win)
{
    // Logout
    GtkWidget *stack = gtk_window_get_child(GTK_WINDOW(win));
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "login_page1");
}

static void quit_activated(GSimpleAction *action,
                           GVariant *parmeter,
                           gpointer win)
{
    // Destroy the main window
    gtk_window_destroy(GTK_WINDOW(win));
}

static void about_activated(GSimpleAction *action,
                            GVariant *parmeter,
                            gpointer win)
{
    // Authors information
    const char *authors[] = {
        "Dale Clack",
        "GCR_CMake on github https://github.com/Makman2/GCR_CMake",
        NULL};

    // Version information
    char *version;
    version = g_strdup_printf("7.1\nRunning Against: Gtk %d.%d.%d",
                              gtk_get_major_version(),
                              gtk_get_minor_version(),
                              gtk_get_micro_version());

    // Get Year information
    time_t t;
    t = time(NULL);
    struct tm *local;
    local = localtime(&t);

    // Copyright
    char *copyright;
    copyright = g_strdup_printf("© 2019—%04d The Xe Project", local->tm_year + 1900);

    hide_launchpad(MY_DOCK(dock));
    // Show the about dialog
    gtk_show_about_dialog(GTK_WINDOW(win),
                          "program-name", "My_GtkUI",
                          "version", version,
                          "copyright", copyright,
                          "comments", "A program that simulates desktop, inspired by lomiri desktop",
                          "authors", authors,
                          "license-type", GTK_LICENSE_GPL_3_0,
                          "logo-icon-name", "My_GtkUI",
                          "title", "About My GtkUI Mac Version",
                          NULL, nullptr);

    // Free Memory
    g_free(version);
    g_free(copyright);
}

void create_main_stack(GtkWindow *win)
{
    static GActionEntry entries[] = {
        {"logout", logout_activated, NULL, NULL, NULL},
        {"quit", quit_activated, NULL, NULL, NULL},
        {"back", back_activated, NULL, NULL, NULL},
        {"about", about_activated, NULL, NULL, NULL}};

    // Create a builder
    GtkBuilder *builder = gtk_builder_new_from_resource("/org/gtk/daleclack/stack.ui");

    // Get Widgets
    GtkWidget *stack = GTK_WIDGET(gtk_builder_get_object(builder, "main_stack"));
    GtkWidget *login_page = GTK_WIDGET(gtk_builder_get_object(builder, "login_page"));
    GtkWidget *login_overlay = GTK_WIDGET(gtk_builder_get_object(builder, "login_overlay"));
    GtkWidget *main_page = GTK_WIDGET(gtk_builder_get_object(builder, "main_page"));
    GtkWidget *btnlogin = GTK_WIDGET(gtk_builder_get_object(builder, "btnlogin"));

    // Add background for login page
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_xpm_data(winpe);
    GdkPixbuf *sized = gdk_pixbuf_scale_simple(pixbuf, 1024, 576, GDK_INTERP_BILINEAR);
    GdkTexture *texture = gdk_texture_new_for_pixbuf(sized);
    GtkWidget *login_pic = gtk_picture_new_for_paintable(GDK_PAINTABLE(texture));
    g_object_unref(pixbuf);
    g_object_unref(sized);
    gtk_overlay_set_child(GTK_OVERLAY(login_overlay), login_pic);

    // Add login buttons
    gtk_overlay_add_overlay(GTK_OVERLAY(login_overlay), login_page);

    // Add actions
    g_action_map_add_action_entries(G_ACTION_MAP(win), entries,
                                    G_N_ELEMENTS(entries), win);

    // Add dock and finder
    dock = my_dock_new(win);
    gtk_widget_set_hexpand(dock, TRUE);
    gtk_widget_set_vexpand(dock, TRUE);
    gtk_box_append(GTK_BOX(main_page), dock);

    // Link Signals
    g_signal_connect(btnlogin, "clicked", G_CALLBACK(btnlogin_clicked), stack);

    // Add Preferences window
    MyPrefs *prefs = my_dock_get_prefs(MY_DOCK(dock));
    main_win_set_prefs(MAIN_WIN(win), prefs);

    gtk_window_set_child(win, stack);
}
