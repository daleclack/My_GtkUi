#pragma once

#include <gtk/gtk.h>
#include "MainWin.h"

// This #define for gtk 4.10 changes
#define gtk_style_context_add_provider(widget, style, priority)                            \
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(widget)), \
                                               style, GTK_STYLE_PROVIDER_PRIORITY_FALLBACK)

#define gtk_widget_show(widget) gtk_window_present(GTK_WINDOW(widget))

// Add the dock
void add_dock(MainWin *win);

void btnfile_clicked(GtkButton *button, GtkWindow *parent);

// For "Home" Button
void btnhome_clicked(GtkButton *button, GtkWindow *parent);
