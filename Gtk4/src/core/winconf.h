#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(ConfDlg, conf_dlg, CONF, DLG, GtkWindow)

ConfDlg *conf_dlg_new(GtkWindow *parent);

void get_config(int *width, int *height);
