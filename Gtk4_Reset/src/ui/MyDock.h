#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(MyDock, my_dock, MY, DOCK, GtkBox)

GtkWidget *my_dock_new();
