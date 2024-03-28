#pragma once

#include <gtk/gtk.h>

#define my_titlebar_set_window(title_bar, window) \
    _my_titlebar_set_window(My_TitleBar(title_bar), GTK_WIDGET(window))

G_DECLARE_FINAL_TYPE(MyTitleBar, my_titlebar, My, TitleBar, GObject)

void _my_titlebar_set_window(MyTitleBar *title_bar, GtkWidget *window);

MyTitleBar *my_titlebar_new();