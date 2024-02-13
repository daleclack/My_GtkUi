#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(RunApp, run_app, RUN, APP, GtkWindow)

RunApp *run_app_new(GtkWindow *parent);