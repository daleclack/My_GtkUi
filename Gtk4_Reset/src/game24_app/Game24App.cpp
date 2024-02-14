#include "Game24App.h"

struct _Game24App
{
    GtkWindow parent_instance;
    GtkWidget *label_numbers; // Child widgets
    GtkWidget *entry_ans;
    GtkWidget *btnstart, *btnnext, *btnclear,
        *btncheck, *btnexit;
    GtkWidget *btn0, *btn1, *btn2, *btn3, *btn4,
        *btn5, *btn6, *btn7, *btn8, *btn9;
    GtkWidget *btnadd, *btnsubb, *btnmul, *btndiv,
        *btnleft, *btnright;
};

G_DEFINE_TYPE(Game24App, game24_app, GTK_TYPE_WINDOW)

static void game24_app_dispose(GObject *object)
{
    gtk_widget_dispose_template(GTK_WIDGET(object),
                                game24_app_get_type());
    G_OBJECT_CLASS(game24_app_parent_class)->dispose(object);
}

static void game24_app_init(Game24App *self)
{
    // Init Template
    gtk_widget_init_template(GTK_WIDGET(self));
    gtk_window_set_title(GTK_WINDOW(self), "24 Game");
    gtk_window_set_icon_name(GTK_WINDOW(self), "24game");
}

static void game24_app_class_init(Game24AppClass *klass)
{
    G_OBJECT_CLASS(klass)->dispose = game24_app_dispose;
    // Set template
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass),
                                                "/org/gtk/daleclack/game24.ui");
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, label_numbers);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, entry_ans);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnstart);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnnext);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btncheck);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnclear);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnexit);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn0);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn1);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn2);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn3);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn4);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn5);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn6);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn7);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn8);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn9);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnadd);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnsubb);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnmul);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btndiv);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnleft);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnright);
}

Game24App *game24_app_new()
{
    // Create main window
    return GAME24_APP(g_object_new(game24_app_get_type(), NULL));
}