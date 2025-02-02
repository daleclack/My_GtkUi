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

void my_prefs_stop_scan(MyPrefs *self);

void my_prefs_first_load(MyPrefs *self);

DockPos my_prefs_get_dock_pos(MyPrefs *self);

void my_prefs_set_background(MyPrefs *prefs_win, GtkWidget *back);

double my_prefs_get_dpi_value(MyPrefs *self);

MyPrefs *my_prefs_new();
