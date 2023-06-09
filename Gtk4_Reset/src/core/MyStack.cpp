#include "MyStack.h"
#include "MyDock.h"
#include "winpe.xpm"

static void btnlogin_clicked(GtkWidget *widget, GtkStack *stack)
{
    gtk_stack_set_visible_child_name(stack, "main_page1");
}

static void logout_activated(GSimpleAction *action,
                             GVariant *parmeter,
                             gpointer win)
{
    GtkWidget *stack = gtk_window_get_child(GTK_WINDOW(win));
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "login_page1");
}

void create_main_stack(GtkWindow *win)
{
    static GActionEntry entries[] = {
        {"logout", logout_activated, NULL, NULL, NULL}};

    // Create a builder
    GtkBuilder *builder = gtk_builder_new_from_resource("/org/gtk/daleclack/stack.ui");

    // Get Widgets
    GtkWidget *stack = GTK_WIDGET(gtk_builder_get_object(builder, "main_stack"));
    GtkWidget *login_page = GTK_WIDGET(gtk_builder_get_object(builder, "login_page"));
    GtkWidget *login_overlay = GTK_WIDGET(gtk_builder_get_object(builder, "login_overlay"));
    GtkWidget *main_page = GTK_WIDGET(gtk_builder_get_object(builder, "main_page"));
    GtkWidget *btnlogin = GTK_WIDGET(gtk_builder_get_object(builder, "btnlogin"));

    // Add background for login page
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_xpm_data(winpe);
    GdkPixbuf *sized = gdk_pixbuf_scale_simple(pixbuf, 1024, 576, GDK_INTERP_BILINEAR);
    GtkWidget *login_pic = gtk_picture_new_for_pixbuf(sized);
    g_object_unref(pixbuf);
    g_object_unref(sized);
    gtk_overlay_set_child(GTK_OVERLAY(login_overlay), login_pic);

    // Add login buttons
    gtk_overlay_add_overlay(GTK_OVERLAY(login_overlay), login_page);

    // Add actions
    g_action_map_add_action_entries(G_ACTION_MAP(win), entries,
                                    G_N_ELEMENTS(entries), win);

    // Link Signals
    g_signal_connect(btnlogin, "clicked", G_CALLBACK(btnlogin_clicked), stack);

    gtk_window_set_child(win, stack);
}
