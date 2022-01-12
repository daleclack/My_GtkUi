#include "Start.h"

void add_start(GtkBox * box){
    //Create a builder for menu
    GtkBuilder * startbuilder = gtk_builder_new_from_resource("/org/gtk/daleclack/start.ui");

    //Get Widgets
    GtkWidget * menu_btn = (GtkWidget*)gtk_builder_get_object(startbuilder,"menu_btn");
    GtkWidget * label = gtk_label_new(" ");

    //Pack widgets
    gtk_box_prepend(box,menu_btn);
    gtk_box_prepend(box,label);
}