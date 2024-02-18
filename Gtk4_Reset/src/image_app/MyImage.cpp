#include "MyImage.h"

struct _MyImage{
    GtkDrawingArea parent_instance;
    cairo_surface_t *surface;
};

G_DEFINE_TYPE(MyImage, my_image, GTK_TYPE_DRAWING_AREA)

static void my_image_init(MyImage *self)
{

}

static void my_image_class_init(MyImageClass *klass)
{

}