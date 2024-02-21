#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(GameApp, game_app, GAME, APP, GtkWindow)

GameApp *game_app_new();
