#include "MainWin.h"

struct _MainWin{
    GtkApplicationWindow parent;
    GtkWidget * background;
    GtkWidget * draw_area;
    GtkWidget * overlay;
    int width,height;
};

G_DEFINE_TYPE(MainWin,main_win,GTK_TYPE_APPLICATION_WINDOW)

static void change_background(GtkWindow * dialog,int response,MainWin *win){
    gtk_window_destroy(dialog);
}

static void background_dialog(GSimpleAction * action, GVariant * parmeter, gpointer win){
}

static GActionEntry entries = {
    
};

static void main_win_init(MainWin * win){
    //For Custom Window Size
    win->width=800;
    win->height=450;

    //Initailze Window
    gtk_window_set_icon_name(GTK_WINDOW(win),"My_GtkUI");
    gtk_window_set_default_size(GTK_WINDOW(win),win->width,win->height);
    gtk_window_set_title(GTK_WINDOW(win),"My GtkUI (Gtk4 Version)");

    //Add Overlay and background widget
    win->overlay = gtk_overlay_new();
    win->draw_area = gtk_drawing_area_new();
    win->background = gtk_picture_new();
    gtk_overlay_set_child(GTK_OVERLAY(win->overlay),win->background);
    gtk_widget_set_halign(win->draw_area,GTK_ALIGN_FILL);
    gtk_widget_set_valign(win->draw_area,GTK_ALIGN_FILL);
    gtk_overlay_add_overlay(GTK_OVERLAY(win->overlay),win->draw_area);
}

static void main_win_class_init(MainWinClass * klass){}

MainWin * main_win_new(GtkApplication * app){
    return (MainWin*)g_object_new(main_win_get_type(),"application",app,NULL);
}
