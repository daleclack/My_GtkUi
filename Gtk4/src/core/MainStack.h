#pragma once

#include <gtk/gtk.h>
#include <ctime>
#include "MainWin.h"

G_DECLARE_FINAL_TYPE(MainStack, main_stack, Main, Stack, GObject)

MainStack *main_stack_new(MainWin *win, GMenuModel *model);

void main_stack_set_color_theme(MainStack *stack, int gray);

GtkWidget *main_stack_get_stack(MainStack *stack);
