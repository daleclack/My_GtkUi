#include "MainWin.h"
#include "MyStack.h"

struct _MainWin{
    GtkApplicationWindow parent_class;
};

G_DEFINE_TYPE(MainWin, main_win, GTK_TYPE_APPLICATION_WINDOW)

static void main_win_init(MainWin *win){
    // Initalize window
    // gtk_window_set_default_size(GTK_WINDOW(win), 1024, 576);
    gtk_window_set_icon_name(GTK_WINDOW(win), "My_GtkUI");
    gtk_window_set_title(GTK_WINDOW(win), "My Gtk UI Gtk4 Version");

    // Add Main Stack
    create_main_stack(GTK_WINDOW(win));
}

static void main_win_class_init(MainWinClass *win_class){}

MainWin *main_win_new(GtkApplication *app){
    return MAIN_WIN(g_object_new(main_win_get_type(), "application", app, NULL));
}
