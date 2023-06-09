#include "MyStack.h"

static void btnlogin_clicked(GtkWidget *widget, GtkStack *stack){
    gtk_stack_set_visible_child_name(stack, "main_page1");
}

void create_main_stack(GtkWindow *win){
    // Create a builder
    GtkBuilder *builder = gtk_builder_new_from_resource("/org/gtk/daleclack/stack.ui");

    // Get Widgets
    GtkWidget *stack = GTK_WIDGET(gtk_builder_get_object(builder, "main_stack"));
    GtkWidget *login_page = GTK_WIDGET(gtk_builder_get_object(builder, "login_page"));
    GtkWidget *main_page = GTK_WIDGET(gtk_builder_get_object(builder, "main_page"));
    GtkWidget *btnlogin = GTK_WIDGET(gtk_builder_get_object(builder, "btnlogin"));

    // Link Signals
    g_signal_connect(btnlogin, "clicked", G_CALLBACK(btnlogin_clicked), stack);

    gtk_window_set_child(win, stack);
}
