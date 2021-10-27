#include "GameWin.h"

struct _GameWin
{
    GtkWindow parent;
    GtkWidget * btn_1, * btn_2, * btn_3, * btn_4;
    GtkWidget * btn_go, * btn_exit;
    GtkWidget * label_status;
    int flag;
};

G_DEFINE_TYPE(GameWin,game_win,GTK_TYPE_WINDOW)


// GObject *button[4];
// GObject *label;
// const char *btn_index[4]={"0","1","2","3"};
// int flag;

// static void btn_click(GtkWidget *widget,gpointer data){
//     //Get image
//     GdkPixbuf *pixbuf=gdk_pixbuf_new_from_resource("/gtk31/icon.jpg",NULL);
//     GdkPixbuf *sized=gdk_pixbuf_scale_simple(pixbuf,50,50,GDK_INTERP_BILINEAR);
//     GtkWidget *image=gtk_image_new_from_pixbuf(sized);
//     //Get button index
//     char str[2];
//     strcpy(str,(char*)data);
//     //Set a image to the flag button and judge
//     gtk_button_set_child((GtkButton*)button[flag],image);
//     if(flag==str[0]-48){
//         gtk_label_set_label(GTK_LABEL(label),"Good luck!");
//     }else{
//         gtk_label_set_label(GTK_LABEL(label),"Flag missed.");
//     }
//     for(int i=0;i<4;i++){
//         gtk_widget_set_sensitive(GTK_WIDGET(button[i]),FALSE);
//     }
// }

// static void buttons_init(GtkBuilder *builder){
//     //Initalize buttons
//     for(int i=0;i<4;i++){
//         char str[6];
//         sprintf(str,"btn_%d",i+1);
//         button[i]=gtk_builder_get_object(builder,str);
//         g_signal_connect(button[i],"clicked",G_CALLBACK(btn_click),(gpointer)btn_index[i]);
//         //gtk_button_set_always_show_image(GTK_BUTTON(button[i]),TRUE);
//     }
// }

static void btngo_click(GtkWidget * widget,GameWin * win){
    //Reinitalize the buttons and the flag
    gtk_label_set_label(GTK_LABEL(win->label_status),"Select a button");
    srand((unsigned)time(NULL));
    int x=rand();
    win->flag=x%4;
}

// static void gtkmain(GtkApplication *app,gpointer user_data){
//     //Get objects from the Gtkbuilder and initalize the application
//     GtkBuilder *builder=gtk_builder_new();
//     GObject *window;
//     GdkPixbuf *pixbuf=gdk_pixbuf_new_from_resource("/gtk31/icon.jpg",NULL);
//     gtk_builder_add_from_resource(builder,"/gtk31/window.ui",NULL);
//     window=gtk_builder_get_object(builder,"window");
//     gtk_application_add_window(app,GTK_WINDOW(window));
//     gtk_window_set_icon_name(GTK_WINDOW(window),"org.gtk.daleclack");
//     //Get buttons from GtkBuilder
//     buttons_init(builder);
//     GObject *btn_go=gtk_builder_get_object(builder,"btn_go");
//     g_signal_connect(btn_go,"clicked",G_CALLBACK(btngo_click),(gpointer)builder);
//     GObject *btn_exit=gtk_builder_get_object(builder,"btn_exit");
//     g_signal_connect_swapped(btn_exit,"clicked",G_CALLBACK(gtk_window_destroy),window);
//     label=gtk_builder_get_object(builder,"label");
//     //Initalize the flag
//     srand((unsigned)time(NULL));
//     int x=rand();
//     flag=x%4;

//     gtk_widget_show(GTK_WIDGET(window));
//     g_object_unref(pixbuf);
// }

static gboolean game_win_close(GtkWindow * win){
    gtk_widget_hide(GTK_WIDGET(win));
    return TRUE;
}

static void game_win_init(GameWin * game_win){
    gtk_widget_init_template(GTK_WIDGET(game_win));
    gtk_window_set_icon_name(GTK_WINDOW(game_win),"game");
}

static void game_win_class_init(GameWinClass * klass){
    GTK_WINDOW_CLASS(klass)->close_request=game_win_close;
    //Set template for the game window
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass),
                                                "/org/gtk/daleclack/game1.ui");
    //Get Childe Widgets
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),GameWin,btn_1);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),GameWin,btn_2);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),GameWin,btn_3);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),GameWin,btn_4);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),GameWin,btn_go);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),GameWin,btn_exit);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass),GameWin,label_status);
}

GameWin * game_win_new(GtkWindow * win){
    return (GameWin*)g_object_new(game_win_get_type(),"transient-for",win,NULL);
}
