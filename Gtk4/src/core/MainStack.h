#pragma once

#include <gtk/gtk.h>
#include <ctime>
#include "MainWin.h"

GtkWidget * create_main_stack(MainWin * win,GMenuModel * model);
