#include "MyPrefs.h"

struct _MyPrefs{
    GtkWindow parent_instance;
    GtkBuilder *prefs_builder;
    GtkWidget *background;
};

G_DEFINE_TYPE(MyPrefs, my_prefs, GTK_TYPE_WINDOW)

gboolean my_prefs_closed(GtkWindow *window)
{
    gtk_widget_set_visible(GTK_WIDGET(window), FALSE);
    return TRUE;
}

static void my_prefs_init(MyPrefs *self){}

static void my_prefs_class_init(MyPrefsClass *klass)
{
    GTK_WINDOW_CLASS(klass)->close_request = my_prefs_closed;
}

MyPrefs *my_prefs_new(GtkWidget *back)
{
    MyPrefs *prefs_win = MY_PREFS(g_object_new(my_prefs_get_type(), NULL));
    MY_PREFS(prefs_win)->background = back;
    return prefs_win;
}
