#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(AboutWin, about_win, ABOUT, WIN, GtkWindow)

AboutWin *about_win_new();
