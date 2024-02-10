#include "GameApp.h"

struct _GameApp
{
    GtkWindow parent_instance;
    GtkWidget *label;
    GtkWidget *btn_1, *btn_2, *btn_3, *btn_4;
    GtkWidget *btn_go, *btn_exit;
};

G_DEFINE_TYPE(GameApp, game_app, GTK_TYPE_WINDOW)

static void game_app_init(GameApp *self)
{
    gtk_widget_init_template(GTK_WIDGET(self));
}

static void game_app_class_init(GameAppClass *klass)
{
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass),
                                                "/org/gtk/daleclack/game1.ui");
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameApp, label);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameApp, btn_1);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameApp, btn_2);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameApp, btn_3);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameApp, btn_4);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameApp, btn_go);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameApp, btn_exit);
}

GameApp *game_app_new()
{
    return GAME_APP(g_object_new(game_app_get_type(), NULL));
}