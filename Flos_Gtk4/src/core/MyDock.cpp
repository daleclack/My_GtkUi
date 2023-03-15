#include "MyDock.h"
#include "Start.h"
#include "src/file_app/FileWindow.h"

static FileWindow * main_window = NULL;
static GtkWidget * btnfile;
static gboolean file_app_running = FALSE;

static gboolean file_window_closed(GtkWindow * self, GtkButton * dock_file){
    GtkWidget * child = gtk_button_get_child(dock_file);
    gtk_image_set_from_icon_name(GTK_IMAGE(child),"file-manager");
    file_app_running = FALSE;
    gtk_window_destroy(self);
    return TRUE;
}

static void file_window_ctrl(FileWindow * window, GtkWindow * parent){
    GdkSurface * surface = gtk_native_get_surface(GTK_NATIVE(window));
    if(surface){
        GdkToplevelState state = gdk_toplevel_get_state(GDK_TOPLEVEL(surface));
        switch(state){
            case GDK_TOPLEVEL_STATE_MINIMIZED:
                gtk_window_set_transient_for(GTK_WINDOW(window),parent);
                gtk_window_unminimize(GTK_WINDOW(window));
                break;
            default:
                gtk_window_set_transient_for(GTK_WINDOW(window),NULL);
                gtk_window_minimize(GTK_WINDOW(window));
        }
    }
}

void btnfile_clicked(GtkButton * button, GtkWindow * parent){
    //If the file app is not running, create a window
    GtkWidget * child = gtk_button_get_child(GTK_BUTTON(btnfile));
    const char * icon_name = gtk_image_get_icon_name(GTK_IMAGE(child));

    if(!file_app_running){
        //Create the window
        main_window = file_window_new(parent);
        gtk_image_set_from_icon_name(GTK_IMAGE(child),"file-manager_running");

        //Link Signals
        g_signal_connect(main_window,"close-request",G_CALLBACK(file_window_closed),btnfile);

        //Show the window
        file_app_running = TRUE;
        gtk_window_present(GTK_WINDOW(main_window));
    }else{
        file_window_ctrl(main_window,parent);
    }
}

void btnhome_clicked(GtkButton * button, GtkWindow * parent){
    btnfile_clicked(button,parent);
    home_clicked(NULL,main_window);
}

void add_dock(MainWin * win){
    GtkWidget * dock_box, * main_box;

    //Create a box for layout
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL,5);

    //Get Widget
    GtkBuilder * builder = gtk_builder_new_from_resource("/org/gtk/daleclack/dock.ui");
    dock_box = (GtkWidget*)gtk_builder_get_object(builder,"main_dock");

    //Add a start
    add_start(win, GTK_BOX(dock_box));

    //Put the dock on the overlay
    gtk_box_append(GTK_BOX(main_box),dock_box);
    gtk_widget_set_halign(main_box,GTK_ALIGN_CENTER);
    gtk_widget_set_valign(main_box,GTK_ALIGN_END);
    gtk_overlay_add_overlay(main_win_get_overlay(win),main_box);

    //Add a separator
    GtkWidget * space = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(main_box),space);

    //Get the file button
    btnfile = (GtkWidget*)gtk_builder_get_object(builder,"btnfile");
    g_signal_connect(btnfile,"clicked",G_CALLBACK(btnfile_clicked),win);

    //Set Style
    gtk_style_context_add_provider((dock_box),
                                   main_win_get_style(win), G_MAXINT);
    

    //Apply Style for all child in dock
    GtkWidget * child;
    for(child = gtk_widget_get_first_child(dock_box);
        child != NULL;
        child = gtk_widget_get_next_sibling(child))
    {
        gtk_style_context_add_provider((child),
                                       main_win_get_style(win), G_MAXINT);
    }
}
