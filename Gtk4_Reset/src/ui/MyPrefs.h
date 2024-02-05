#pragma once

#include <gtk/gtk.h>

typedef enum
{
    Pos_Left,
    Pos_Right,
    Pos_Bottom
}DockPos;

G_DECLARE_FINAL_TYPE(MyPrefs, my_prefs, MY, PREFS, GtkWindow)

void my_prefs_start_scan(MyPrefs *self);

void my_prefs_first_load(MyPrefs *self);

MyPrefs *my_prefs_new(GtkWidget *back);
