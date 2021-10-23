#include <cstdio>
#include "winconf.h"

static void conf_response(GtkWidget *widget,int response,GtkBuilder *builder_conf){
    //Get width and height config and put the numbers into winsize.conf file
    GObject *width_spin,*height_spin;
    width_spin=gtk_builder_get_object(builder_conf,"width_spin");
    height_spin=gtk_builder_get_object(builder_conf,"height_spin");
    // GObject *entry_width,*entry_height;
    // entry_width=gtk_builder_get_object(builder_conf,"entry_width");
    // entry_height=gtk_builder_get_object(builder_conf,"entry_height");
    if(response==GTK_RESPONSE_OK){
        // const gchar *width,*height;
        // width=gtk_entry_get_text(GTK_ENTRY(entry_width));
        // height=gtk_entry_get_text(GTK_ENTRY(entry_height));
        int width,height;
        width=gtk_spin_button_get_value_as_int((GtkSpinButton*)width_spin);
        height=gtk_spin_button_get_value_as_int((GtkSpinButton*)height_spin);
        freopen("winsize.conf","w",stdout);
        g_print("%d %d",width,height);
        fclose(stdout);
    }
    gtk_widget_destroy(widget);
}

static void set_default(GtkWidget *widget,GtkBuilder *builder_conf){
    //Discard changes and set to default config
    GObject *width_spin=gtk_builder_get_object(builder_conf,"width_spin");
    gtk_spin_button_set_value((GtkSpinButton*)width_spin,640);
    GObject *height_spin=gtk_builder_get_object(builder_conf,"height_spin");
    gtk_spin_button_set_value((GtkSpinButton*)height_spin,480);
    // GObject *entry_width=gtk_builder_get_object(builder_conf,"entry_width");
    // GObject *entry_height=gtk_builder_get_object(builder_conf,"entry_height");
    // gtk_entry_set_text(GTK_ENTRY(entry_width),"640");
    // gtk_entry_set_text(GTK_ENTRY(entry_height),"360");
}

static void get_winsize(GtkWidget *widget,GtkBuilder *builder_conf){
    //Get current window size
    int width=640,height=360;
    //char width1[7],height1[7];
    GtkWindow *window;
    //Get dialog window and entry
    GObject *dialog=gtk_builder_get_object(builder_conf,"dialog");
    GObject *width_spin=gtk_builder_get_object(builder_conf,"width_spin");
    //gtk_entry_set_text(GTK_ENTRY(entry_width),"640");
    GObject *height_spin=gtk_builder_get_object(builder_conf,"height_spin");
    // GObject *entry_width=gtk_builder_get_object(builder_conf,"entry_width");
    // GObject *entry_height=gtk_builder_get_object(builder_conf,"entry_height");
    //Get main window
    window=gtk_window_get_transient_for(GTK_WINDOW(dialog));
    gtk_window_get_size(GTK_WINDOW(window),&width,&height);
    gtk_spin_button_set_value((GtkSpinButton*)width_spin,width);
    gtk_spin_button_set_value((GtkSpinButton*)height_spin,height);
    //g_print("%d %d\n",width,height);
    // sprintf(width1,"%d",width);
    // sprintf(height1,"%d",height);
    // gtk_entry_set_text(GTK_ENTRY(entry_width),width1);
    // gtk_entry_set_text(GTK_ENTRY(entry_height),height1);
}

void conf_dialog(GtkWidget *widget,GtkBuilder *builder){
    //Window config dialog
    GtkBuilder *conf_size=gtk_builder_new_from_resource("/gtk42/win_size.ui");
    GObject *dialog=gtk_builder_get_object(conf_size,"dialog");
    GObject *window=gtk_builder_get_object(builder,"window");
    gtk_window_set_transient_for(GTK_WINDOW(dialog),GTK_WINDOW(window));
    g_signal_connect(dialog,"response",G_CALLBACK(conf_response),conf_size);
    //Set default config
    GObject *width_spin=gtk_builder_get_object(conf_size,"width_spin");
    gtk_spin_button_set_value((GtkSpinButton*)width_spin,640);
    //gtk_entry_set_text(GTK_ENTRY(entry_width),"640");
    GObject *height_spin=gtk_builder_get_object(conf_size,"height_spin");
    gtk_spin_button_set_value((GtkSpinButton*)height_spin,480);
    // GObject *entry_height=gtk_builder_get_object(conf_size,"entry_height");
    // gtk_entry_set_text(GTK_ENTRY(entry_height),"360");
    //Get window size
    GObject *btnGet=gtk_builder_get_object(conf_size,"btnGet");
    g_signal_connect(btnGet,"clicked",G_CALLBACK(get_winsize),conf_size);
    //Reset to default
    GObject *btn_default=gtk_builder_get_object(conf_size,"btn_default");
    g_signal_connect(btn_default,"clicked",G_CALLBACK(set_default),conf_size);
    gtk_widget_show_all(GTK_WIDGET(dialog));
}

void get_config(int *width,int *height){
    freopen("winsize.conf","r",stdin);
    scanf("%d",width);
    scanf("%d",height);
    fclose(stdin);
}
