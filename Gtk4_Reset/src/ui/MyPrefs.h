#pragma once

#include <gtk/gtk.h>

typedef enum
{
    Pos_Left,
    Pos_Right,
    Pos_Bottom
}DockPos;

G_DECLARE_FINAL_TYPE(MyPrefs, my_prefs, MY, PREFS, GtkWindow)

MyPrefs *my_prefs_new(GtkWidget *back);
