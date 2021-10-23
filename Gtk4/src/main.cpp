#include "MainWin.h"

static void gtkmain(GtkApplication *app,gpointer user_data){
    MainWin * window;
    window = main_win_new(app);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc,char *argv[]){
    GtkApplication *app;
    int status;
    app=gtk_application_new("org.gtk.daleclack",G_APPLICATION_NON_UNIQUE);
    g_signal_connect(app,"activate",G_CALLBACK(gtkmain),NULL);
    status=g_application_run(G_APPLICATION(app),argc,argv);
    return status;
}
