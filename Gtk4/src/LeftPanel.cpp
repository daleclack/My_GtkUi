#include <cstdlib>
#include <thread>
#include "LeftPanel.h"
#include "MainWin.h"
// #include "game.h"
// #include "TextEditor.h"
// #include "drawing.h"
// #include "FileWindow.h"

struct _LeftPanel{
    GtkBox parent;
    GtkWindow * parent_win;
    GtkWidget * popover1;
    GtkWidget * btnstart;
    GtkWidget * btnaud;
    GtkWidget * btngedit;
    GtkWidget * btnvlc;
    GtkWidget * btn_vlc;
    GtkWidget * btn_note;
    GtkWidget * btnabout;
    GtkWidget * btnfiles;
    GtkWidget * btndraw;
    GtkWidget * btngame;
    GtkWidget * btnrun;
    GtkWidget * btneditor;
};

G_DEFINE_TYPE(LeftPanel,left_panel,GTK_TYPE_BOX)

static void btnvlc_clicked(GtkWidget *widget,gpointer data){
    std::thread first(system,"vlc");
    first.detach();
}

static void btngedit_clicked(GtkWidget *widget,gpointer data){
    std::thread second(system,"gedit");
    second.detach();
}

static void btnaud_clicked(GtkWidget *widget,gpointer data){
    std::thread third(system,"audacious");
    third.detach();
}

static void btnnote_clicked(GtkWidget *widget,gpointer data){
    std::thread fourth(system,"start notepad");
    fourth.detach();
}

static void btnvlc_win32(GtkWidget *widget,gpointer data){
    std::thread fifth(system,"start ..\\vlc\\vlc.exe");
    fifth.detach();
}

void left_panel_set_parent(LeftPanel * self,GtkWindow * parent_win1){
    self->parent_win = parent_win1;
}

static void left_panel_init(LeftPanel * panel){
    gtk_widget_init_template(GTK_WIDGET(panel));

    //Set Image for start button
    gtk_menu_button_set_label(GTK_MENU_BUTTON(panel->btnstart),"Start");

    //Connect Signals
    g_signal_connect(panel->btnaud,"clicked",G_CALLBACK(btnaud_clicked),NULL);
    g_signal_connect_swapped(panel->btnaud,"clicked",G_CALLBACK(gtk_popover_popdown),panel->popover1);
    g_signal_connect(panel->btnvlc,"clicked",G_CALLBACK(btnvlc_clicked),NULL);
    g_signal_connect_swapped(panel->btnvlc,"clicked",G_CALLBACK(gtk_popover_popdown),panel->popover1);
    g_signal_connect(panel->btngedit,"clicked",G_CALLBACK(btngedit_clicked),NULL);
    g_signal_connect_swapped(panel->btngedit,"clicked",G_CALLBACK(gtk_popover_popdown),panel->popover1);
    g_signal_connect(panel->btn_note,"clicked",G_CALLBACK(btnnote_clicked),NULL);
    g_signal_connect_swapped(panel->btn_note,"clicked",G_CALLBACK(gtk_popover_popdown),panel->popover1);
    g_signal_connect(panel->btn_vlc,"clicked",G_CALLBACK(btnvlc_win32),NULL);
    g_signal_connect_swapped(panel->btn_vlc,"clicked",G_CALLBACK(gtk_popover_popdown),panel->popover1);
    g_signal_connect(panel->btnabout,"clicked",G_CALLBACK(btnabout_clicked),panel->parent_win);
    g_signal_connect_swapped(panel->btnabout,"clicked",G_CALLBACK(gtk_popover_popdown),panel->popover1);
}

static void left_panel_class_init(LeftPanelClass * klass){
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass),
                                                "/org/gtk/daleclack/leftpanel.ui");
    
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,popover1);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,btnstart);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,btnaud);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,btngedit);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,btnvlc);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,btn_vlc);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,btn_note);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,btnabout);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,btndraw);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,btnfiles);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,btngame);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,btnrun);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,btneditor);
}

LeftPanel * left_panel_new(){
    return (LeftPanel*)g_object_new(left_panel_get_type(),NULL);
}

// void btnfiles_clicked(GtkWidget *widget,GtkWindow *parent){
//     FileWindow * window1 = file_window_new();
//     gtk_window_set_transient_for(GTK_WINDOW(window1),parent);
//     gtk_widget_show_all(GTK_WIDGET(window1));
// }

// void add_leftpanel(GtkBuilder *builder,GtkFixed *fixed){
//     //Gtk31 application
//     GObject *btngame=gtk_builder_get_object(panel2,"btngame");
//     g_signal_connect(btngame,"clicked",G_CALLBACK(gamemain),window);
//     g_signal_connect_swapped(btngame,"clicked",G_CALLBACK(gtk_widget_hide),popover);
//     //Text Editor(Gtk86)
//     GObject *btnedit=gtk_builder_get_object(panel2,"btneditor");
//     g_signal_connect(btnedit,"clicked",G_CALLBACK(text_editor),window);
//     g_signal_connect_swapped(btnedit,"clicked",G_CALLBACK(gtk_widget_hide),popover);
//     //Drawing application
//     GObject *btndraw=gtk_builder_get_object(panel2,"btndraw");
//     g_signal_connect(btndraw,"clicked",G_CALLBACK(drawing_main),window);
//     g_signal_connect_swapped(btndraw,"clicked",G_CALLBACK(gtk_widget_hide),popover);
//     //About window
//     GObject *btn_about=gtk_builder_get_object(panel2,"btnabout");
//     g_signal_connect(btn_about,"clicked",G_CALLBACK(win1_init),window);
//     g_signal_connect_swapped(btn_about,"clicked",G_CALLBACK(gtk_widget_hide),popover);
//     //File Manager
//     GObject *btnfiles=gtk_builder_get_object(panel2,"btnfiles");
//     g_signal_connect(btnfiles,"clicked",G_CALLBACK(btnfiles_clicked),window);
//     g_signal_connect_swapped(btnfiles,"clicked",G_CALLBACK(gtk_widget_hide),popover);
//     gtk_fixed_put(fixed,GTK_WIDGET(panel),0,25);
//     g_object_unref(pixbuf);
//     g_object_unref(sized);
// }
