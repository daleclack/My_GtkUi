#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(MediaPlayer,media_player,MEDIA,PLAYER,GtkApplicationWindow)

MediaPlayer * media_player_new(GtkWindow * parent_win);
