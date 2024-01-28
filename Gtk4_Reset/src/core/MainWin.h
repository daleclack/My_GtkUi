#pragma once

#include <gtk/gtk.h>
#include "MyPrefs.h"

// Declare type for main window
G_DECLARE_FINAL_TYPE(MainWin, main_win, MAIN, WIN, GtkApplicationWindow)

// Create a new main window
MainWin *main_win_new(GtkApplication *app);

// The Preference widget
void main_win_set_prefs(MainWin *win, MyPrefs *prefs);
MyPrefs *main_win_get_prefs(MainWin *win);
