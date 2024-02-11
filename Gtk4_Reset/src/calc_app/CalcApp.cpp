#include "CalcApp.h"
#include "calc.h"

struct _CalcApp
{
    GtkWindow parent_instance;
};

G_DEFINE_TYPE(CalcApp, calc_app, GTK_TYPE_WINDOW)

static void calc_app_dispose(GObject *object)
{
    gtk_widget_dispose_template(GTK_WIDGET(object),
                                calc_app_get_type());
    G_OBJECT_CLASS(calc_app_parent_class)->dispose(object);
}

static void calc_app_init(CalcApp *self)
{
    gtk_widget_init_template(GTK_WIDGET(self));
}

static void calc_app_class_init(CalcAppClass *klass)
{
    G_OBJECT_CLASS(klass)->dispose = calc_app_dispose;
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass),
                                                "/org/gtk/daleclack/calcapp.ui");
}

CalcApp *calc_app_new()
{
    return CALC_APP(g_object_new(calc_app_get_type(), NULL));
}
