#include "ImageApp.h"
#include "MyImage.h"

struct _ImageApp
{
    GtkWindow parent_instance;
    // Child widgets
    GtkWidget *main_box, *btn_box;
    GtkWidget *image_sw;
    GtkWidget *image_scale, *btnopen;

    // Context Menu
    GtkGesture *gesture;
    GtkPopoverMenu *image_menu;

    // Main Image View
    MyImage *image_view;
};

G_DEFINE_TYPE(ImageApp, image_app, GTK_TYPE_APPLICATION_WINDOW)

static void image_app_dialog_response(GObject *dialog, GAsyncResult *result, gpointer data)
{
    GFile *file;
    ImageApp *app = IMAGE_APP(data);
    file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(dialog), result, NULL);
    if (file)
    {
        // Open a image and show it
        GdkPixbuf *pixbuf;
        pixbuf = gdk_pixbuf_new_from_file(g_file_get_path(file), NULL);
        my_image_set_source_pixbuf(app->image_view, pixbuf);
        g_object_unref(file);
    }
}

static void btnopen_clicked(GtkButton *btn, ImageApp *self)
{
    // Create a file dialog and show
    GtkFileDialog *dialog;
    dialog = gtk_file_dialog_new();
    gtk_file_dialog_open(dialog, GTK_WINDOW(self), NULL, image_app_dialog_response, self);
}

static void image_app_init(ImageApp *self)
{
    // Initalize window
    gtk_window_set_title(GTK_WINDOW(self), "Image Viewer");
    gtk_window_set_default_size(GTK_WINDOW(self), 800, 450);
    gtk_window_set_icon_name(GTK_WINDOW(self), "image_app");

    // Create child widgets
    self->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    self->btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    self->image_view = my_image_new();
    self->image_sw = gtk_scrolled_window_new();
    self->image_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.1, 10.0, 0.1);
    self->btnopen = gtk_button_new_with_label("Open Image");
    g_signal_connect(self->btnopen, "clicked", G_CALLBACK(btnopen_clicked), self);

    // Initalize widgets
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(self->image_sw),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_hexpand(self->image_sw, TRUE);
    gtk_widget_set_vexpand(self->image_sw, TRUE);
    gtk_widget_set_hexpand(self->image_scale, TRUE);
    gtk_range_set_value(GTK_RANGE(self->image_scale), 1.0);
    gtk_scale_set_draw_value(GTK_SCALE(self->image_scale), TRUE);
    gtk_scale_set_value_pos(GTK_SCALE(self->image_scale), GTK_POS_LEFT);

    // Pack Widgets
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(self->image_sw), GTK_WIDGET(self->image_view));
    gtk_box_append(GTK_BOX(self->main_box), self->image_sw);
    gtk_box_append(GTK_BOX(self->btn_box), self->image_scale);
    gtk_box_append(GTK_BOX(self->btn_box), self->btnopen);
    gtk_box_append(GTK_BOX(self->main_box), self->btn_box);
    gtk_window_set_child(GTK_WINDOW(self), self->main_box);
}

static void image_app_class_init(ImageAppClass *klass)
{
}

ImageApp *image_app_new()
{
    return IMAGE_APP(g_object_new(image_app_get_type(), NULL));
}
