#include "MyDock.h"
#include "Start.h"

void add_dock(MainWin * win){
    GtkWidget * dock_box, * main_box;

    //Create a box for layout
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL,5);

    //Get Widget
    GtkBuilder * builder = gtk_builder_new_from_resource("/org/gtk/daleclack/dock.ui");
    dock_box = (GtkWidget*)gtk_builder_get_object(builder,"main_dock");

    //Add a start
    add_start(win, GTK_BOX(dock_box));

    //Put the dock on the overlay
    gtk_box_append(GTK_BOX(main_box),dock_box);
    gtk_widget_set_halign(main_box,GTK_ALIGN_CENTER);
    gtk_widget_set_valign(main_box,GTK_ALIGN_END);
    gtk_overlay_add_overlay(main_win_get_overlay(win),main_box);

    //Add a separator
    GtkWidget * space = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(main_box),space);

    //Set Style
    gtk_style_context_add_provider(gtk_widget_get_style_context(dock_box), 
                                    main_win_get_style(win), G_MAXINT);
}
