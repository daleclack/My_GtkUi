#include "MyDock.h"

void add_dock(GtkWidget * overlay, GtkStyleProvider * provider){
    GtkWidget * dock_box;
    //Get Widget
    GtkBuilder * builder = gtk_builder_new_from_resource("/org/gtk/daleclack/dock.ui");
    dock_box = (GtkWidget*)gtk_builder_get_object(builder,"main_dock");

    //Put the dock on the overlay
    gtk_widget_set_halign(dock_box,GTK_ALIGN_CENTER);
    gtk_widget_set_valign(dock_box,GTK_ALIGN_END);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay),dock_box);

    //Set Style
    gtk_style_context_add_provider(gtk_widget_get_style_context(dock_box), provider, G_MAXINT);
}
