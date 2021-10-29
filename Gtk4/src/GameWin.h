#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(GameWin,game_win,GAME,WIN,GtkWindow)

GameWin * game_win_new(GtkWindow * win);

