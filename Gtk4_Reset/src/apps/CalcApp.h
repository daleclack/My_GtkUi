#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(CalcApp, calc_app, CALC, APP, GtkWindow)

CalcApp *calc_app_new(double dpi_value);
