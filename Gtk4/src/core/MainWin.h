#pragma once 

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(MainWin,main_win,MAIN,WIN,GtkApplicationWindow)

MainWin * main_win_new(GtkApplication *app);

GtkWidget * main_win_get_background(MainWin * win);

void btnabout_clicked(GtkWidget * widget,gpointer data);

int main_win_get_height(MainWin *win);
