#include "ImageApp.h"

struct _ImageApp{
    GtkWindow parent_instance;
};

G_DEFINE_TYPE(ImageApp, image_app, GTK_TYPE_APPLICATION_WINDOW)

static void image_app_init(ImageApp *self)
{
    gtk_window_set_title(GTK_WINDOW(self), "Image Viewer");
}

static void image_app_class_init(ImageAppClass *klass)
{}

ImageApp *image_app_new()
{
    return IMAGE_APP(g_object_new(image_app_get_type(), NULL));
}
