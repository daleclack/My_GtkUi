#pragma once

#include <gtk/gtk.h>
#include "MainWin.h"

//Add the dock
void add_dock(MainWin * win);

void btnfile_clicked(GtkButton * button, GtkWindow * parent);

//For "Home" Button
void btnhome_clicked(GtkButton * button, GtkWindow * parent);
