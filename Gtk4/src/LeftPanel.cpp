#include <cstdlib>
#include <thread>
#include "LeftPanel.h"
#include "MainWin.h"
#include "GameWin.h"
// #include "TextEditor.h"
// #include "drawing.h"
#include "FileWindow.h"

struct _LeftPanel{
    GtkBox parent;
    GtkWindow * parent_win;
    GtkWidget * popover1;
    GtkWidget * btnstart;
    //Button for linux applications
    GtkWidget * btnaud, * btngedit, * btnvlc;
    //Button for win32 applications
    GtkWidget * btn_vlc, * btn_note;
    //Integrated applications
    GtkWidget * btnabout, * btnfiles, * btndraw, * btngame, * btnrun, * btneditor;
    //Panel Buttons
    GtkWidget * panel_file, * panel_game;
    //Panel Images
    GtkWidget * file_image, * game_image;
    //Application Windows
    GameWin * game1;
    FileWindow * file_win;
    //Status
    gboolean game_running,file_running;
};

G_DEFINE_TYPE(LeftPanel,left_panel,GTK_TYPE_BOX)

//Just simply launch applications
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

//Window Control Function

static void window_ctrl(LeftPanel * self,GtkWindow * ctrled_win){
    GdkSurface * surface = gtk_native_get_surface(GTK_NATIVE(ctrled_win));
    if(surface){
        GdkToplevelState state = gdk_toplevel_get_state(GDK_TOPLEVEL(surface));
        switch(state){
            case GDK_TOPLEVEL_STATE_MINIMIZED:
                gtk_window_set_transient_for(ctrled_win,self->parent_win);
                gtk_window_unminimize(ctrled_win);
                break;
            default:
                gtk_window_set_transient_for(ctrled_win,NULL);
                gtk_window_minimize(ctrled_win);
        }
    } 
}

//Functions for game app window

static gboolean game_window_closed(GtkWindow * window,LeftPanel * parent_panel){
    parent_panel->game_running = FALSE;
    gtk_image_set_from_icon_name(GTK_IMAGE(parent_panel->game_image),"game");
    gtk_window_destroy(window);
    return TRUE;
}

static void btngame_clicked(GtkWidget * widget,LeftPanel * parent_panel){
    if(!parent_panel->game_running){
        //Create a window
        parent_panel->game1 = game_win_new(parent_panel->parent_win);

        //Connect to the close signal for window
        g_signal_connect(parent_panel->game1,"close-request",G_CALLBACK(game_window_closed),parent_panel);

        //Change Panel Status
        gtk_image_set_from_icon_name(GTK_IMAGE(parent_panel->game_image),"game_running");
        parent_panel->game_running = TRUE;

        //Show Window
        gtk_window_present(GTK_WINDOW(parent_panel->game1));
    }else{
        //The Game Window is running, control the window
        window_ctrl(parent_panel,GTK_WINDOW(parent_panel->game1));
    }
}

//Functions for game app window

static gboolean file_window_closed(GtkWindow * window,LeftPanel * parent_panel){
    parent_panel->file_running = FALSE;
    gtk_image_set_from_icon_name(GTK_IMAGE(parent_panel->file_image),"file-app");
    gtk_window_destroy(window);
    return TRUE;
}

static void btnfile_clicked(GtkWidget * widget,LeftPanel * parent_panel){
    if(!parent_panel->file_running){
        //Create a window
        parent_panel->file_win = file_window_new(parent_panel->parent_win);

        //Connect to the close signal for window
        g_signal_connect(parent_panel->file_win,"close-request",G_CALLBACK(file_window_closed),parent_panel);

        //Change Panel Status
        gtk_image_set_from_icon_name(GTK_IMAGE(parent_panel->file_image),"file-app-running");
        parent_panel->file_running = TRUE;

        //Show Window
        gtk_window_present(GTK_WINDOW(parent_panel->file_win));
    }else{
        //The Files Window is running, control the window
        window_ctrl(parent_panel,GTK_WINDOW(parent_panel->file_win));
    }
}

static void left_panel_init(LeftPanel * panel){
    gtk_widget_init_template(GTK_WIDGET(panel));

    //Set Image for start button

    gtk_menu_button_set_label(GTK_MENU_BUTTON(panel->btnstart),"Start");

    //Connect Signals
    //Audacious media player
    g_signal_connect(panel->btnaud,"clicked",G_CALLBACK(btnaud_clicked),NULL);
    g_signal_connect_swapped(panel->btnaud,"clicked",G_CALLBACK(gtk_popover_popdown),panel->popover1);
    //Vlc Media Player
    g_signal_connect(panel->btnvlc,"clicked",G_CALLBACK(btnvlc_clicked),NULL);
    g_signal_connect_swapped(panel->btnvlc,"clicked",G_CALLBACK(gtk_popover_popdown),panel->popover1);
    //Gedit Text Editor
    g_signal_connect(panel->btngedit,"clicked",G_CALLBACK(btngedit_clicked),NULL);
    g_signal_connect_swapped(panel->btngedit,"clicked",G_CALLBACK(gtk_popover_popdown),panel->popover1);
    //Notepad in windows
    g_signal_connect(panel->btn_note,"clicked",G_CALLBACK(btnnote_clicked),NULL);
    g_signal_connect_swapped(panel->btn_note,"clicked",G_CALLBACK(gtk_popover_popdown),panel->popover1);
    //VLC in windows
    g_signal_connect(panel->btn_vlc,"clicked",G_CALLBACK(btnvlc_win32),NULL);
    g_signal_connect_swapped(panel->btn_vlc,"clicked",G_CALLBACK(gtk_popover_popdown),panel->popover1);
    //"About" window
    g_signal_connect(panel->btnabout,"clicked",G_CALLBACK(btnabout_clicked),panel->parent_win);
    g_signal_connect_swapped(panel->btnabout,"clicked",G_CALLBACK(gtk_popover_popdown),panel->popover1);
    //File Manager
    g_signal_connect(panel->btnfiles,"clicked",G_CALLBACK(btnfile_clicked),panel->parent_win);
    g_signal_connect_swapped(panel->btnfiles,"clicked",G_CALLBACK(gtk_popover_popdown),panel->popover1);
    //A guess game
    g_signal_connect(panel->btngame,"clicked",G_CALLBACK(btngame_clicked),panel);
    g_signal_connect_swapped(panel->btngame,"clicked",G_CALLBACK(gtk_popover_popdown),panel->popover1);
    //Panel Buttons
    g_signal_connect(panel->panel_game,"clicked",G_CALLBACK(btngame_clicked),panel);
    g_signal_connect(panel->panel_file,"clicked",G_CALLBACK(btnfile_clicked),panel);
}

static void left_panel_class_init(LeftPanelClass * klass){
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass),
                                                "/org/gtk/daleclack/leftpanel.ui");
    
    //Get Child widgets
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
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,panel_file);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,file_image);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,panel_game);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),LeftPanel,game_image);
}

LeftPanel * left_panel_new(){
    return (LeftPanel*)g_object_new(left_panel_get_type(),NULL);
}
