#include "MainWin.h"

struct _MainWin{
    GtkApplicationWindow parent_instance;
    GtkWidget * overlay;
    GtkWidget * background;
};

G_DEFINE_TYPE(MainWin,main_win,GTK_TYPE_APPLICATION_WINDOW)

static void main_win_init(MainWin * win){
    //Initalize window
    gtk_window_set_title(GTK_WINDOW(win),"My GtkUI Flos Version");
    gtk_window_set_default_size(GTK_WINDOW(win),800,450);
    gtk_window_set_icon_name(GTK_WINDOW(win),"org.gtk.daleclack");

    //Create widgets
    win->overlay = gtk_overlay_new();
    win->background = gtk_picture_new();

    //Add Background
    GdkPixbuf * pixbuf = gdk_pixbuf_new_from_resource("/org/gtk/daleclack/flos.png",NULL);
    GdkPixbuf * sized = gdk_pixbuf_scale_simple(pixbuf,800,450,GDK_INTERP_BILINEAR);
    gtk_picture_set_pixbuf(GTK_PICTURE(win->background),sized);
    g_object_unref(pixbuf);
    g_object_unref(sized);

    //Add MenuBar
    GtkBuilder * builder = gtk_builder_new_from_resource("/org/gtk/daleclack/menubar.xml");
    GMenuModel * model = G_MENU_MODEL(gtk_builder_get_object(builder,"model"));
    GtkWidget * menubar = gtk_popover_menu_bar_new_from_model(model);
    //gtk_button_set_child(GTK_BUTTON(back_button),menubar);
    gtk_widget_set_valign(menubar,GTK_ALIGN_START);
    gtk_widget_set_halign(menubar,GTK_ALIGN_FILL);
    gtk_overlay_add_overlay(GTK_OVERLAY(win->overlay),menubar);

    //Apply Style for menubar
    GtkStyleProvider * provider = GTK_STYLE_PROVIDER(gtk_css_provider_new());
    gtk_css_provider_load_from_resource(GTK_CSS_PROVIDER(provider),"/org/gtk/daleclack/style.css");
    gtk_style_context_add_provider(gtk_widget_get_style_context(menubar),provider,G_MAXINT);
    
    //Add widgets
    gtk_overlay_set_child(GTK_OVERLAY(win->overlay),win->background);
    gtk_window_set_child(GTK_WINDOW(win),win->overlay);
}

static void main_win_class_init(MainWinClass * klass){}

MainWin * main_win_new(GtkApplication * app){
    return (MainWin*)g_object_new(main_win_get_type(),"application",app,NULL);
}