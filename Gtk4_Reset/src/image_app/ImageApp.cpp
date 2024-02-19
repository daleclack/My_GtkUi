#include "ImageApp.h"
#include "MyImage.h"

struct _ImageApp{
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

    // Initalize widgets
    
}

static void image_app_class_init(ImageAppClass *klass)
{}

ImageApp *image_app_new()
{
    return IMAGE_APP(g_object_new(image_app_get_type(), NULL));
}
