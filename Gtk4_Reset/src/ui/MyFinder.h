#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(MyFinder, my_finder, MY, FINDER, GtkBox)

GtkWidget *my_finder_new(GtkOrientation orientation, int spacing);

void my_finder_add_style(MyFinder *finder, GtkCssProvider *provider);

void my_finder_apply_dpi(MyFinder *finder, double dpi);

gboolean get_dark_mode(gpointer self);
