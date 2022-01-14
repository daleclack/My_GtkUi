#include "Start.h"

static void btnprogram_clicked(GtkWidget * widget, GtkStack * stack){
    gtk_stack_set_visible_child_name(stack,"page1");
}

static void btnback_clicked(GtkWidget * widget, GtkStack * stack){
    gtk_stack_set_visible_child_name(stack,"page0");
}

void add_start(MainWin * win, GtkBox * box){
    //Create a builder for menu
    GtkBuilder * startbuilder = gtk_builder_new_from_resource("/org/gtk/daleclack/start.ui");

    //Get Widgets
    GtkWidget * menu_btn = (GtkWidget*)gtk_builder_get_object(startbuilder,"menu_btn");
    GtkWidget * label = gtk_label_new(" ");

    //Stack Change effect
    GtkWidget * stack_program, * btn_program, * btn_back;
    stack_program = (GtkWidget*)gtk_builder_get_object(startbuilder,"stack_program");
    btn_program = (GtkWidget*)gtk_builder_get_object(startbuilder,"btn_program");
    btn_back = (GtkWidget*)gtk_builder_get_object(startbuilder,"btn_back");
    g_signal_connect(btn_program,"clicked",G_CALLBACK(btnprogram_clicked),stack_program);
    g_signal_connect(btn_back,"clicked",G_CALLBACK(btnback_clicked),stack_program);

    //Enable Shutdown(Close)
    GtkWidget * btnshut = (GtkWidget*)gtk_builder_get_object(startbuilder,"btnshut");
    g_signal_connect_swapped(btnshut,"clicked",G_CALLBACK(gtk_window_destroy),win);

    //Pack widgets
    gtk_box_prepend(box,menu_btn);
    gtk_box_prepend(box,label);
}
