#pragma once

#include <gtk/gtk.h>

// Just to warp the other widgets to the GtkWidget Object without casting it.
#define my_titlebar_set_window(title_bar, window) \
    _my_titlebar_set_window(My_TitleBar(title_bar), GTK_WIDGET(window))
#define my_titlebar_pack_end(title_bar, widget) \
    _my_titlebar_pack_end(My_TitleBar(title_bar), GTK_WIDGET(widget))

G_DECLARE_FINAL_TYPE(MyTitleBar, my_titlebar, My, TitleBar, GObject)

void my_titlebar_set_dpi_scale(MyTitleBar *title_bar, double dpi_scale = 1.0);

void _my_titlebar_set_window(MyTitleBar *title_bar, GtkWidget *window);

void _my_titlebar_pack_end(MyTitleBar *title_bar, GtkWidget *widget);

MyTitleBar *my_titlebar_new();