#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(ImageApp, image_app, IMAGE, APP, GtkApplicationWindow)

ImageApp *image_app_new(double dpi_value);