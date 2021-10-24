#pragma once 

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(LeftPanel,left_panel,LEFT,PANEL,GtkBox)

LeftPanel * left_panel_new();

void left_panel_set_parent(LeftPanel * self,GtkWindow * parent_win1);
