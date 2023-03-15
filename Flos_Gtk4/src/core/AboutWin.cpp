#include "AboutWin.h"

struct _AboutWin{
    GtkWindow parent_instance;
};

G_DEFINE_TYPE(AboutWin, about_win, GTK_TYPE_WINDOW)

static void about_win_init(AboutWin *self){
    gtk_widget_init_template(GTK_WIDGET(self));
}

static void about_win_class_init(AboutWinClass *klass){
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), 
                                                "/org/gtk/daleclack/cambalache/aboutwin.ui");
}

AboutWin *about_win_new(){
    return ABOUT_WIN(g_object_new(about_win_get_type(), NULL));
}
