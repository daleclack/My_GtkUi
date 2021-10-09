#include <cstdlib>
#include <thread>
#include "panel2.h"
#include "win1.h"
#include "game.h"
#include "TextEditor.h"
#include "drawing.h"
#include "FileWindow.h"

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

void btnfiles_clicked(GtkWidget *widget,GtkWindow *parent){
    FileWindow * window1 = file_window_new();
    gtk_window_set_transient_for(GTK_WINDOW(window1),parent);
    gtk_widget_show_all(GTK_WIDGET(window1));
}

void add_leftpanel(GtkBuilder *builder,GtkFixed *fixed){
    //Get Left panel
    GtkBuilder *panel2=gtk_builder_new_from_resource("/gtk42/leftpanel.ui");
    GObject *panel=gtk_builder_get_object(panel2,"left_panel");
    //Set start button and image
    GObject *btn_image=gtk_builder_get_object(panel2,"btn_image");
    GdkPixbuf *pixbuf=gdk_pixbuf_new_from_resource("/gtk42/icon.png",NULL);
    GdkPixbuf *sized=gdk_pixbuf_scale_simple(pixbuf,40,40,GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(GTK_IMAGE(btn_image),sized);
    //Get popover window
    GObject *popover=gtk_builder_get_object(panel2,"popover1");
    //Button image
    GtkWidget *img_vlc=gtk_image_new_from_resource("/gtk42/vlc.png");
    GtkWidget *img_aud=gtk_image_new_from_resource("/gtk42/audacious.png");
    GtkWidget *img_gedit=gtk_image_new_from_resource("/gtk42/gedit.png");
    //Audacious bin exec
    GObject *btn_audacious=gtk_builder_get_object(panel2,"btnaud");
    gtk_button_set_image(GTK_BUTTON(btn_audacious),img_aud);
    g_signal_connect(btn_audacious,"clicked",G_CALLBACK(btnaud_clicked),NULL);
    g_signal_connect_swapped(btn_audacious,"clicked",G_CALLBACK(gtk_widget_hide),popover);
    //vlc exec button for linux
    GObject *btnvlc=gtk_builder_get_object(panel2,"btnvlc");
    GObject *img_vlc1=gtk_builder_get_object(panel2,"image1");
    gtk_image_set_from_resource(GTK_IMAGE(img_vlc1),"/gtk42/vlc.png");
    g_signal_connect(btnvlc,"clicked",G_CALLBACK(btnvlc_clicked),NULL);
    g_signal_connect_swapped(btnvlc,"clicked",G_CALLBACK(gtk_widget_hide),popover);
    //Start Gedit Text Editor
    GObject *btngedit=gtk_builder_get_object(panel2,"btngedit");
    GObject *img_gedit1=gtk_builder_get_object(panel2,"image2");
    gtk_image_set_from_resource(GTK_IMAGE(img_gedit1),"/gtk42/gedit.png");
    g_signal_connect(btngedit,"clicked",G_CALLBACK(btngedit_clicked),NULL);
    g_signal_connect_swapped(btngedit,"clicked",G_CALLBACK(gtk_widget_hide),popover);
    //Notepad on windows
    GObject *btnnote=gtk_builder_get_object(panel2,"btn_note");
    gtk_button_set_image(GTK_BUTTON(btnnote),img_gedit);
    g_signal_connect(btnnote,"clicked",G_CALLBACK(btnnote_clicked),NULL);
    g_signal_connect_swapped(btnnote,"clicked",G_CALLBACK(gtk_widget_hide),popover);
    //VLC on windows
    GObject *btn_vlc=gtk_builder_get_object(panel2,"btn_vlc");
    gtk_button_set_image(GTK_BUTTON(btn_vlc),img_vlc);
    g_signal_connect(btn_vlc,"clicked",G_CALLBACK(btnvlc_win32),NULL);
    g_signal_connect_swapped(btn_vlc,"clicked",G_CALLBACK(gtk_widget_hide),popover);
    //Get main window
    GObject *window=gtk_builder_get_object(builder,"window");
    //Gtk31 application
    GObject *btngame=gtk_builder_get_object(panel2,"btngame");
    g_signal_connect(btngame,"clicked",G_CALLBACK(gamemain),window);
    g_signal_connect_swapped(btngame,"clicked",G_CALLBACK(gtk_widget_hide),popover);
    //Text Editor(Gtk86)
    GObject *btnedit=gtk_builder_get_object(panel2,"btneditor");
    g_signal_connect(btnedit,"clicked",G_CALLBACK(text_editor),window);
    g_signal_connect_swapped(btnedit,"clicked",G_CALLBACK(gtk_widget_hide),popover);
    //Drawing application
    GObject *btndraw=gtk_builder_get_object(panel2,"btndraw");
    g_signal_connect(btndraw,"clicked",G_CALLBACK(drawing_main),window);
    g_signal_connect_swapped(btndraw,"clicked",G_CALLBACK(gtk_widget_hide),popover);
    //About window
    GObject *btn_about=gtk_builder_get_object(panel2,"btnabout");
    g_signal_connect(btn_about,"clicked",G_CALLBACK(win1_init),window);
    g_signal_connect_swapped(btn_about,"clicked",G_CALLBACK(gtk_widget_hide),popover);
    //File Manager
    GObject *btnfiles=gtk_builder_get_object(panel2,"btnfiles");
    g_signal_connect(btnfiles,"clicked",G_CALLBACK(btnfiles_clicked),window);
    g_signal_connect_swapped(btnfiles,"clicked",G_CALLBACK(gtk_widget_hide),popover);
    gtk_fixed_put(fixed,GTK_WIDGET(panel),0,25);
    g_object_unref(pixbuf);
    g_object_unref(sized);
}
