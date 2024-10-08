#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(FileWindow, file_window, FILE, WINDOW, GtkApplicationWindow)

FileWindow *file_window_new(GtkWindow *win, double dpi_value);

void home_clicked(GtkWidget *widget, FileWindow *win);

GListModel *file_window_get_grid_model(FileWindow *self);

GListModel *file_window_get_column_model(FileWindow *self);

GtkWidget *file_window_get_folder_entry(FileWindow *self);
