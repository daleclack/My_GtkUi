#include "GameApp.h"
#include <cstdlib>
#include <ctime>

struct _GameApp
{
    GtkWindow parent_instance;
    GtkWidget *label;
    GtkWidget *btn_1, *btn_2, *btn_3, *btn_4;
    GtkWidget *btn_go, *btn_exit;
};

G_DEFINE_TYPE(GameApp, game_app, GTK_TYPE_WINDOW)

static void btns_clicked(GtkButton *widget, GameApp *self)
{
    // Get random index
    srand(time(NULL));
    int index = (rand() % 4) + 1;

    // Get Button index
    const char *button_label = gtk_button_get_label(widget);

    // Create image widget
    GtkWidget *image1 = gtk_image_new();
    gtk_image_set_pixel_size(GTK_IMAGE(image1), 48);
    gtk_image_set_from_icon_name(GTK_IMAGE(image1), "My_GtkUI");

    // Check button index and set icon
    switch (index)
    {
    case 1:
        gtk_button_set_child(GTK_BUTTON(self->btn_1), image1);
        break;
    case 2:
        gtk_button_set_child(GTK_BUTTON(self->btn_2), image1);
        break;
    case 3:
        gtk_button_set_child(GTK_BUTTON(self->btn_3), image1);
        break;
    case 4:
        gtk_button_set_child(GTK_BUTTON(self->btn_4), image1);
        break;
    }

    // Set Label
    if (index == (int)(button_label[0] - 48))
    {
        gtk_label_set_label(GTK_LABEL(self->label), "Good Luck!");
    }
    else
    {
        gtk_label_set_label(GTK_LABEL(self->label), "The Button is here!");
    }
}

static void btngo_clicked(GtkWidget *widget, GameApp *self)
{
    // Initalize label and btns
    gtk_label_set_label(GTK_LABEL(self->label), "Select a button");
    gtk_button_set_label(GTK_BUTTON(self->btn_1), "1");
    gtk_button_set_label(GTK_BUTTON(self->btn_2), "2");
    gtk_button_set_label(GTK_BUTTON(self->btn_3), "3");
    gtk_button_set_label(GTK_BUTTON(self->btn_4), "4");
}

static void btnexit_clicked(GtkWidget *widget, GameApp *self)
{
    // Close the game
    gtk_window_close(GTK_WINDOW(self));
}

static void game_app_dispose(GObject *object)
{
    // Destructer
    gtk_widget_dispose_template(GTK_WIDGET(object), game_app_get_type());
    G_OBJECT_CLASS(game_app_parent_class)->dispose(object);
}

static void game_app_init(GameApp *self)
{
    // Initalize template
    gtk_widget_init_template(GTK_WIDGET(self));

    // Link Signals
    g_signal_connect(self->btn_1, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btn_2, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btn_3, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btn_4, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btn_go, "clicked", G_CALLBACK(btngo_clicked), self);
    g_signal_connect(self->btn_exit, "clicked", G_CALLBACK(btnexit_clicked), self);
}

static void game_app_class_init(GameAppClass *klass)
{
    // Set dispose signal for the window
    G_OBJECT_CLASS(klass)->dispose = game_app_dispose;

    // Set template
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass),
                                                "/org/gtk/daleclack/game1.ui");

    // Get widget from template
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
