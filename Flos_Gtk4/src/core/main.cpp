#include "MainWin.h"

static void gtkmain(GtkApplication * app,gpointer user_data){
    //Create a window and show
    MainWin * window;
    window = main_win_new(app);
    //gtk_widget_show(GTK_WIDGET(window));
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char ** argv){
    //Create a new application and run
    GtkApplication * app;
    app = gtk_application_new("org.gtk.daleclack",G_APPLICATION_NON_UNIQUE);
    g_signal_connect(app,"activate",G_CALLBACK(gtkmain),NULL);
    return g_application_run(G_APPLICATION(app),argc,argv);
}