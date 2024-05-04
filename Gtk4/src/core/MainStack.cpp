#include "MainStack.h"
#include "LeftPanel.h"

static gboolean change_time(gpointer data)
{
    // Get local time
    time_t t;
    struct tm *local;
    t = time(NULL);
    local = localtime(&t);
    // Format the time and set the time
    char current_time[20];
    sprintf(current_time, "%02d:%02d %04d/%02d/%02d",
            local->tm_hour, local->tm_min, local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
    // g_print("%s\n",current_time);
    gtk_label_set_label(GTK_LABEL(data), current_time);
    return TRUE;
}

static void stack_login(GtkWidget *widget, GtkStack *stack)
{
    gtk_stack_set_visible_child_name(stack, "main_page");
}

GtkWidget *create_main_stack(MainWin *win, GMenuModel *model)
{
    GtkBuilder *stack_builder;
    stack_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/stack.ui");

    // Load Style
    GtkCssProvider *provider;
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(provider, "/org/gtk/daleclack/style.css");

    // Get Main Widget
    GtkWidget *main_stack;
    main_stack = (GtkWidget *)gtk_builder_get_object(stack_builder, "stack");

    // User Label
    GtkWidget *label_user = (GtkWidget *)gtk_builder_get_object(stack_builder, "label_user");
    gtk_widget_add_css_class(label_user, "label_white");

    // Login Button
    GtkWidget *btnlogin = (GtkWidget *)gtk_builder_get_object(stack_builder, "btnlogin");
    g_signal_connect(btnlogin, "clicked", G_CALLBACK(stack_login), main_stack);
    GtkWidget *btn_label = gtk_button_get_child(GTK_BUTTON(btnlogin));
    gtk_widget_add_css_class(btn_label, "label_white");

    // Label for app
    GtkWidget *label_app = (GtkWidget *)gtk_builder_get_object(stack_builder, "label_app");
    gtk_widget_add_css_class(label_app, "label_def");

    // Time Label
    GtkWidget *label_time = (GtkWidget *)gtk_builder_get_object(stack_builder, "label_time");
    g_timeout_add(100, change_time, label_time);
    gtk_widget_add_css_class(label_time, "label_def");

    // Menu Button
    GtkWidget *menubtn = (GtkWidget *)gtk_builder_get_object(stack_builder, "menu_button");
    GtkWidget *popover = gtk_popover_menu_new_from_model(model);
    gtk_widget_set_halign(popover, GTK_ALIGN_END);
    gtk_popover_set_has_arrow(GTK_POPOVER(popover), FALSE);
    gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(menubtn), "shut_down");
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(menubtn), popover);

    // Box for LeftPanel
    GtkWidget *left_box = (GtkWidget *)gtk_builder_get_object(stack_builder, "leftbox");
    LeftPanel *panel = left_panel_new();
    left_panel_set_parent(panel, GTK_WINDOW(win));
    gtk_box_append(GTK_BOX(left_box), GTK_WIDGET(panel));

    // Add Styles
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(label_user),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(btn_label),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(label_app),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(label_time),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    return main_stack;
}
