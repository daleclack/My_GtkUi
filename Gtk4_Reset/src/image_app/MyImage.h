#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(MyImage, my_image, MY, IMAGE, GtkDrawingArea)

MyImage *my_image_new();

void my_image_set_source_pixbuf(MyImage *image, GdkPixbuf *pixbuf);

void my_image_scale_draw(MyImage *image, double scale);
