#include <cstdio>
#include "winconf.h"
#include "MainWin.h"

struct _ConfDlg
{
    GtkWindow parent_instance;
    GtkWidget *width_spin, *height_spin;
    GtkWidget *btn_ok, *btn_cancel;
    GtkWidget *main_box, *btn_box;
};

G_DEFINE_TYPE(ConfDlg, conf_dlg, GTK_TYPE_WINDOW)

static void btnopen_clicked(GtkButton *dialog, gpointer data)
{
    // Get width and height value and save the value to a file
    int width, height;
    width = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(CONF_DLG(data)->width_spin));
    height = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(CONF_DLG(data)->height_spin));
    freopen("winsize.conf", "w", stdout);
    g_print("width=%d\nheight=%d\n", width, height);
    fclose(stdout);
    gtk_window_destroy(GTK_WINDOW(data));
}

static void set_default(GtkWidget *widget, ConfDlg *dialog)
{
    // Discard changes and set to default config
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(dialog->width_spin), 800);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(dialog->height_spin), 450);
}

static void get_winsize(GtkWidget *widget, ConfDlg *dialog)
{
    int width = 800, height = 450;
    GtkWindow *window;
    // Get main window
    window = gtk_window_get_transient_for(GTK_WINDOW(dialog));
    // Get Window Size
    GtkWidget *back = main_win_get_background(MAIN_WIN(window));
    width = gtk_widget_get_size(back, GTK_ORIENTATION_HORIZONTAL);
    height = gtk_widget_get_size(back, GTK_ORIENTATION_VERTICAL);
    // Set Value of spin buttons
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(dialog->width_spin), width);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(dialog->height_spin), height);
}

static void conf_dlg_init(ConfDlg *self)
{
    // Initalize dialog
    gtk_window_set_title(GTK_WINDOW(self), "Window Size Config");
    // gtk_dialog_add_buttons(GTK_DIALOG(self), "OK", GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL);
    // gtk_window_set_default_size(GTK_WINDOW(self),320,150);
    gtk_window_set_icon_name(GTK_WINDOW(self), "My_GtkUI");
    self->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    self->btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_halign(self->btn_box, GTK_ALIGN_END);

    // Create Buttons
    self->btn_ok = gtk_button_new_with_label("OK");
    self->btn_cancel = gtk_button_new_with_label("Cancel");
    gtk_box_append(GTK_BOX(self->btn_box), self->btn_ok);
    gtk_box_append(GTK_BOX(self->btn_box), self->btn_cancel);

    // Get Content area
    // GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(self));

    // Initalize Spin Button
    GtkAdjustment *width_adj, *height_adj;
    width_adj = gtk_adjustment_new(800, 640, 9999, 1, 1, 1);
    height_adj = gtk_adjustment_new(450, 360, 9999, 1, 1, 1);
    self->width_spin = gtk_spin_button_new(width_adj, 1, 0);
    self->height_spin = gtk_spin_button_new(height_adj, 1, 0);

    // Add Child Widgets
    GtkWidget *label_width, *label_height, *btn_getsize, *btn_default;
    GtkWidget *vbox, *box_width, *box_height, *box_utils, *center_box;

    // Create Boxes
    center_box = gtk_center_box_new();
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    box_width = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    box_height = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    box_utils = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_halign(box_width, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(box_height, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(center_box, TRUE);
    gtk_widget_set_vexpand(center_box, TRUE);
    gtk_widget_set_valign(center_box, GTK_ALIGN_CENTER);
    gtk_widget_set_vexpand(self->main_box, TRUE);

    // Create Widgets
    label_width = gtk_label_new("Width:");
    label_height = gtk_label_new("Height");
    btn_default = gtk_button_new_with_label("Reset to Default Size");
    btn_getsize = gtk_button_new_with_label("Get Current Size");

    // Get Current Config
    int width, height;
    get_config(&width, &height);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(self->width_spin), width);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(self->height_spin), height);

    // Width
    gtk_box_append(GTK_BOX(box_width), label_width);
    gtk_box_append(GTK_BOX(box_width), self->width_spin);

    // Height
    gtk_box_append(GTK_BOX(box_height), label_height);
    gtk_box_append(GTK_BOX(box_height), self->height_spin);

    // Utils
    gtk_box_append(GTK_BOX(box_utils), btn_getsize);
    gtk_box_append(GTK_BOX(box_utils), btn_default);

    gtk_box_append(GTK_BOX(vbox), box_width);
    gtk_box_append(GTK_BOX(vbox), box_height);
    gtk_box_append(GTK_BOX(vbox), box_utils);
    gtk_center_box_set_center_widget(GTK_CENTER_BOX(center_box), vbox);
    gtk_box_append(GTK_BOX(self->main_box), center_box);
    gtk_box_append(GTK_BOX(self->main_box), self->btn_box);

    // Link Signals
    g_signal_connect(btn_default, "clicked", G_CALLBACK(set_default), self);
    g_signal_connect(btn_getsize, "clicked", G_CALLBACK(get_winsize), self);
    g_signal_connect(self->btn_ok, "clicked", G_CALLBACK(btnopen_clicked), self);
    g_signal_connect_swapped(self->btn_cancel, "clicked", G_CALLBACK(gtk_window_destroy), self);

    gtk_window_set_child(GTK_WINDOW(self), self->main_box);
}

static void conf_dlg_class_init(ConfDlgClass *klass)
{
}

ConfDlg *conf_dlg_new(GtkWindow *parent)
{
    // Create a new dialog
    return (ConfDlg *)g_object_new(conf_dlg_get_type(), "transient-for", parent, NULL);
}

void get_config(int *width, int *height)
{
    freopen("winsize.conf", "r", stdin);
    scanf("width=%d", width);
    getchar();
    scanf("height=%d", height);
    fclose(stdin);
}
