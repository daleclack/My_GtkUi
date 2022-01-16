#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(FileWindow,file_window,FILE,WINDOW,GtkWindow)

FileWindow * file_window_new(GtkWindow * parent);

void home_clicked(GtkWidget * item,FileWindow * win);
