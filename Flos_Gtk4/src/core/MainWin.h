#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(MainWin, main_win, MAIN, WIN, GtkApplicationWindow)

MainWin *main_win_new(GtkApplication *app);

GtkOverlay *main_win_get_overlay(MainWin *win);

GtkStyleProvider *main_win_get_style(MainWin *win);

bool main_win_get_dark_mode(MainWin *win);
