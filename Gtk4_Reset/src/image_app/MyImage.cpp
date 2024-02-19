#include "MyImage.h"

struct _MyImage
{
    GtkDrawingArea parent_instance;
    cairo_surface_t *surface;
    GdkPixbuf *pixbuf;
    double scale_radio;
};

G_DEFINE_TYPE(MyImage, my_image, GTK_TYPE_DRAWING_AREA)

static void my_image_draw(GtkDrawingArea *area, cairo_t *cr,
                          int width, int height, gpointer data)
{
    MyImage *image_view = MY_IMAGE(data);

    if (!image_view->pixbuf)
    {
        return;
    }

    // Get width and height for drawing area
    gtk_widget_set_size_request(GTK_WIDGET(area), width, height);

    // Scale the image and draw
    cairo_set_source_surface(cr, image_view->surface, 0, 0);
    
    cairo_surface_set_device_scale(image_view->surface,
                                   image_view->scale_radio, image_view->scale_radio);
    cairo_paint(cr);
}

static void my_image_init(MyImage *self)
{
    self->scale_radio = 1.0;
    gtk_widget_set_hexpand(GTK_WIDGET(self), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(self), TRUE);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(self), my_image_draw, self, NULL);
}

static void my_image_class_init(MyImageClass *klass)
{
}

MyImage *my_image_new()
{
    return MY_IMAGE(g_object_new(my_image_get_type(), NULL));
}

void my_image_set_source_pixbuf(MyImage *image, GdkPixbuf *pixbuf)
{
    int width, height;
    if (!pixbuf)
    {
        return;
    }

    if (image->pixbuf)
    {
        g_object_unref(image->pixbuf);
    }
    image->pixbuf = pixbuf;
    width = gdk_pixbuf_get_width(pixbuf);
    height = gdk_pixbuf_get_height(pixbuf);

    if (!image->surface)
    {
        image->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    }
    gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(image), width);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(image), height);

    cairo_t *cr = cairo_create(image->surface);
    gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);
    cairo_paint(cr);
    gtk_widget_queue_draw(GTK_WIDGET(image));
}

void my_image_scale_draw(MyImage *image, double scale)
{
    // Set the scale radio and scale
    if (scale <= 0)
    {
        image->scale_radio = 10.0;
        return;
    }
    if (scale > 10.0)
    {
        image->scale_radio = 0.1;
        return;
    }
    image->scale_radio = 1.0 / scale;
    gtk_widget_queue_draw(GTK_WIDGET(image));
}
