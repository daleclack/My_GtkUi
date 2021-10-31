#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(DrawingApp,drawing_app,DRAWING,APP,GtkWindow)

DrawingApp * drawing_app_new(GtkWindow * parent_win);
