#include "MainWin.h"

struct _MainWin{
    GtkApplicationWindow parent;
};

G_DEFINE_TYPE(MainWin,main_win,GTK_TYPE_APPLICATION_WINDOW)

static void main_win_init(MainWin * win){
    gtk_window_set_icon_name(GTK_WINDOW(win),"My_GtkUI");
}

static void main_win_class_init(MainWinClass * klass){}

MainWin * main_win_new(GtkApplication * app){
    return (MainWin*)g_object_new(main_win_get_type(),"application",app,NULL);
}
