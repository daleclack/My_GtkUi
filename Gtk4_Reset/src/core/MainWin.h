#pragma once

#include <gtk/gtk.h>

// Declare type for main window
G_DECLARE_FINAL_TYPE(MainWin, main_win, MAIN, WIN, GtkApplicationWindow)

// Create a new main window
MainWin *main_win_new(GtkApplication *app);
