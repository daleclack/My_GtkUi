#include "RunApp.h"
#include <thread>
#include <cstdlib>

struct _RunApp
{
    GtkWindow parent_instance;
    GtkWidget *main_box, *entry_box, *btn_box;
    GtkWidget *entry_run;
    GtkWidget *btnok, *btncel, *btnpath;
    GtkWidget *label, *label2;
};

G_DEFINE_TYPE(RunApp, run_app, GTK_TYPE_WINDOW)

static void btnok_clicked(GtkButton *btn, RunApp *self)
{
    auto text = gtk_editable_get_text(GTK_EDITABLE(self->entry_run));
    std::thread run_app(system, text);
    run_app.detach();
    gtk_window_destroy(GTK_WINDOW(self));
}

static void btncancel_clicked(GtkButton *btn, RunApp *self)
{
    gtk_window_destroy(GTK_WINDOW(self));
}

static void btnpath_clicked(GtkButton *btn, RunApp *self)
{
}

static void run_app_init(RunApp *self)
{
    // Initalize windows
    gtk_window_set_title(GTK_WINDOW(self), "Run a command");
    gtk_window_set_icon_name(GTK_WINDOW(self), "myrunner");
    gtk_window_set_default_size(GTK_WINDOW(self), 300, 150);

    // Create widgets
    self->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    self->entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    self->btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    self->entry_run = gtk_entry_new();
    self->btnok = gtk_button_new_with_label("OK");
    self->btncel = gtk_button_new_with_label("Cancel");
    self->btnpath = gtk_button_new_with_label("...");
    self->label = gtk_label_new(" ");
    self->label2 = gtk_label_new(" ");
    gtk_editable_set_text(GTK_EDITABLE(self->entry_run), "vlc");
    gtk_widget_set_halign(self->entry_box, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(self->btn_box, GTK_ALIGN_END);

    // Pack widgets
    gtk_box_append(GTK_BOX(self->entry_box), self->entry_run);
    gtk_box_append(GTK_BOX(self->entry_box), self->btnpath);
    gtk_box_append(GTK_BOX(self->btn_box), self->btnok);
    gtk_box_append(GTK_BOX(self->btn_box), self->btncel);
    gtk_box_append(GTK_BOX(self->main_box), self->label);
    gtk_box_append(GTK_BOX(self->main_box), self->entry_box);
    gtk_box_append(GTK_BOX(self->main_box), self->label2);
    gtk_box_append(GTK_BOX(self->main_box), self->btn_box);

    // Link signals
    g_signal_connect(self->btnok, "clicked", G_CALLBACK(btnok_clicked), self);
    g_signal_connect(self->btncel, "clicked", G_CALLBACK(btncancel_clicked), self);
    g_signal_connect(self->btnpath, "clicked", G_CALLBACK(btnpath_clicked), self);

    // Set Child
    gtk_window_set_child(GTK_WINDOW(self), self->main_box);
}

static void run_app_class_init(RunAppClass *klass)
{
}

RunApp *run_app_new(GtkWindow *parent)
{
    return RUN_APP(g_object_new(run_app_get_type(),
                                "transient-for", parent, NULL));
}
