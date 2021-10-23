#include "MainWin.h"

struct _MainWin{
    GtkApplicationWindow parent;
    GtkWidget * background;
    GtkWidget * overlay;
    GtkGesture * gesture;
    GtkWidget * popover;
    int width,height;
};

G_DEFINE_TYPE(MainWin,main_win,GTK_TYPE_APPLICATION_WINDOW)

static void change_background(GtkWindow * dialog,int response,MainWin *win){
    gtk_window_destroy(dialog);
}

static void background_dialog(GSimpleAction * action, GVariant * parmeter, MainWin *win){
}

static void default_background1(GSimpleAction * action, GVariant * parmeter, MainWin *win){}

static void default_background2(GSimpleAction * action, GVariant * parmeter, MainWin *win){}

static void gesture_pressed(GtkGestureClick *self,int n_press,double x,double y,MainWin *win){
    GdkRectangle rect;
    rect.x=x;
    rect.y=y;
    rect.height=1;
    rect.width=1;
    gtk_popover_set_pointing_to(GTK_POPOVER(win->popover),&rect);
    gtk_popover_popup(GTK_POPOVER(win->popover));
}

static GActionEntry entries[] = {
    {"back",background_dialog,NULL,NULL,NULL}
};

static void main_win_init(MainWin * win){
    //For Custom Window Size
    win->width=800;
    win->height=450;

    //Initailze Window
    gtk_window_set_icon_name(GTK_WINDOW(win),"My_GtkUI");
    gtk_window_set_default_size(GTK_WINDOW(win),win->width,win->height);
    gtk_window_set_title(GTK_WINDOW(win),"My GtkUI (Gtk4 Version)");

    //Add Actions
    g_action_map_add_action_entries(G_ACTION_MAP(win),entries,G_N_ELEMENTS(entries),win);

    //Add Overlay and background widget
    win->overlay = gtk_overlay_new();
    win->background = gtk_picture_new();
    gtk_overlay_set_child(GTK_OVERLAY(win->overlay),win->background);

    //Add Menu
    GtkBuilder * menu_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/appmenu.xml");
    GMenuModel * model = G_MENU_MODEL(gtk_builder_get_object(menu_builder,"app-menu"));
    win->popover = gtk_popover_menu_new_from_model(model);
    gtk_popover_set_has_arrow(GTK_POPOVER(win->popover),FALSE);
    gtk_widget_set_parent(win->popover,win->overlay);

    //Add Gesture to show menu
    win->gesture = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(win->gesture),GDK_BUTTON_SECONDARY);
    g_signal_connect(win->gesture,"pressed",G_CALLBACK(gesture_pressed),win);
    gtk_widget_add_controller(win->overlay,GTK_EVENT_CONTROLLER(win->gesture));

    gtk_window_set_child(GTK_WINDOW(win),win->overlay);
}

static void main_win_class_init(MainWinClass * klass){}

MainWin * main_win_new(GtkApplication * app){
    return (MainWin*)g_object_new(main_win_get_type(),"application",app,NULL);
}
