#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(DrawApp, draw_app, DRAW, APP, GtkWindow)

DrawApp *draw_app_new(double dpi_value);
