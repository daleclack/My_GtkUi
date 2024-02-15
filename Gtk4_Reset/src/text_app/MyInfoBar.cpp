#include "MyInfoBar.h"

struct _MyInfoBar
{
    GtkBox parent_instance;
    GtkWidget *btn_box;
    GtkWidget *space_label;
    GtkWidget *revealer, *msg_label, *button;
};

G_DEFINE_TYPE(MyInfoBar, my_infobar, GTK_TYPE_BOX)

static void my_infobar_btn_clicked(GtkButton *btn, MyInfoBar *self)
{
    // Hide info bar content
    gtk_widget_set_visible(self->btn_box, FALSE);
}

static void my_infobar_init(MyInfoBar *self)
{
    // Create widgets
    self->revealer = gtk_revealer_new();
    self->btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    self->msg_label = gtk_label_new("");
    self->space_label = gtk_label_new(" ");
    self->button = gtk_button_new_with_label("OK");
    g_signal_connect(self->button, "clicked", G_CALLBACK(my_infobar_btn_clicked), self);

    // Add childs
    gtk_box_append(GTK_BOX(self->btn_box), self->msg_label);
    gtk_box_append(GTK_BOX(self->btn_box), self->space_label);
    gtk_box_append(GTK_BOX(self->btn_box), self->button);

    // Properties
    gtk_widget_set_halign(self->msg_label, GTK_ALIGN_START);
    gtk_widget_set_valign(self->button, GTK_ALIGN_END);
    gtk_widget_set_hexpand(self->space_label, TRUE);
    gtk_widget_set_margin_start(self->btn_box, 10);
    gtk_widget_set_margin_end(self->btn_box, 10);

    gtk_revealer_set_child(GTK_REVEALER(self->revealer), self->btn_box);
    gtk_box_append(GTK_BOX(self), self->revealer);
}

static void my_infobar_class_init(MyInfoBarClass *self)
{
}

MyInfoBar *my_infobar_new()
{
    return MY_INFOBAR(g_object_new(my_infobar_get_type(), NULL));
}

void my_info_bar_set_message(MyInfoBar *info_bar, const char *message)
{
    // Set label for info bar
    gtk_label_set_label(GTK_LABEL(info_bar->msg_label), message);
}
