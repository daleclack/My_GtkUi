#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(Game24App, game24_app, GAME24, APP, GtkWindow)

Game24App *game24_app_new();
