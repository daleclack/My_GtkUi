#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(MyFinder, my_finder, MY, FINDER, GtkBox)

GtkWidget *my_finder_new(GtkOrientation orientation, int spacing);
