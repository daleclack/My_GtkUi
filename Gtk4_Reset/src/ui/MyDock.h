#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(MyDock, my_dock, MY, DOCK, GtkBox)

GtkWidget *my_dock_new();   // Create a new MyDock

void btnlaunch_clicked(GtkWidget *widget, MyDock *dock);    // Hide/Show the launchpad
