#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(MyInfoBar, my_infobar, MY, INFOBAR, GtkBox)

MyInfoBar *my_infobar_new();

// Copy message, the limit is 256 characters
void my_infobar_show_message(MyInfoBar *info_bar, const char *message);
