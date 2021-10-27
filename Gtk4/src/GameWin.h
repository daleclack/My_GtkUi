#pragma once

#include <gtk/gtk.h>
#include <ctime>
#include <cstdlib>
#include <cstring>

G_DECLARE_FINAL_TYPE(GameWin,game_win,GAME,WIN,GtkWindow)

GameWin * game_win_new(GtkWindow * win);

