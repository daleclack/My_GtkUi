#pragma once

#include <gtk/gtk.h>
#include <ctime>
#include "MainWin.h"

G_DECLARE_FINAL_TYPE(MainStack, main_stack, Main, Stack, GObject)

MainStack *main_stack_new();

GtkWidget * create_main_stack(MainWin * win,GMenuModel * model);
