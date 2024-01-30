#pragma once

#include <gtk/gtk.h>
#include "MyPrefs.h"

G_DECLARE_FINAL_TYPE(MyDock, my_dock, MY, DOCK, GtkBox)

GtkWidget *my_dock_new(GtkWindow *parent); // Create a new MyDock

void hide_launchpad(MyDock *dock); // Hide the launchpad

GtkWidget *my_dock_get_background(MyDock *dock);  // Get Background widget

MyPrefs *my_dock_get_prefs(MyDock *dock);       // Get Preferences window
