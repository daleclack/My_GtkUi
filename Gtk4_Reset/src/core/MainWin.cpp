#include "MainWin.h"

struct _MainWin{
    GtkApplicationWindow parent_class;
};

G_DEFINE_TYPE(MainWin, main_win, GTK_TYPE_APPLICATION_WINDOW)

static void main_win_init(MainWin *win){}

static void main_win_class_init(MainWinClass *win_class){}

MainWin *main_win_new(GtkApplication *app){
    return MAIN_WIN(g_object_new(main_win_get_type(), "application", app, NULL));
}
