#include "MainStack.h"
#include "LeftPanel.h"

struct _MainStack
{
    GObject parent_instance;
    GtkBuilder *stack_builder;
    GtkCssProvider *provider;
    GtkWidget *main_stack;
    GtkWidget *left_box;
    GtkWidget *popover;
    GtkWidget *label_app;
    GtkWidget *label_time;
};

G_DEFINE_TYPE(MainStack, main_stack, G_TYPE_OBJECT)

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

static void main_stack_dispose(GObject *object)
{
    g_object_unref(Main_Stack(object)->stack_builder);
    G_OBJECT_CLASS(main_stack_parent_class)->dispose(object);
}

static void main_stack_init(MainStack *self)
{
    GtkBuilder *stack_builder;
    stack_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/stack.ui");

    // Load Style
    self->provider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(self->provider, "/org/gtk/daleclack/style.css");

    // Get Main Widget
    self->main_stack = (GtkWidget *)gtk_builder_get_object(stack_builder, "stack");

    // User Label
    GtkWidget *label_user = (GtkWidget *)gtk_builder_get_object(stack_builder, "label_user");
    gtk_widget_add_css_class(label_user, "label_white");

    // Login Button
    GtkWidget *btnlogin = (GtkWidget *)gtk_builder_get_object(stack_builder, "btnlogin");
    g_signal_connect(btnlogin, "clicked", G_CALLBACK(stack_login), self->main_stack);
    GtkWidget *btn_label = gtk_button_get_child(GTK_BUTTON(btnlogin));
    gtk_widget_add_css_class(btn_label, "label_white");

    // Label for app
    self->label_app = (GtkWidget *)gtk_builder_get_object(stack_builder, "label_app");
    // gtk_widget_add_css_class(self->label_app, "label_def");

    // Time Label
    self->label_time = (GtkWidget *)gtk_builder_get_object(stack_builder, "label_time");
    g_timeout_add(100, change_time, self->label_time);
    // gtk_widget_add_css_class(self->label_time, "label_def");

    // Menu Button
    GtkWidget *menubtn = (GtkWidget *)gtk_builder_get_object(stack_builder, "menu_button");
    self->popover = gtk_popover_menu_new_from_model(NULL);
    gtk_widget_set_halign(self->popover, GTK_ALIGN_END);
    gtk_popover_set_has_arrow(GTK_POPOVER(self->popover), FALSE);
    gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(menubtn), "shut_down");
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(menubtn), self->popover);

    // Add Styles
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(label_user),
                                               GTK_STYLE_PROVIDER(self->provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(btn_label),
                                               GTK_STYLE_PROVIDER(self->provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Box for LeftPanel
    self->left_box = (GtkWidget *)gtk_builder_get_object(stack_builder, "leftbox");
}

static void main_stack_class_init(MainStackClass *self)
{
    G_OBJECT_CLASS(self)->dispose = main_stack_dispose;
}

MainStack *main_stack_new(MainWin *win, GMenuModel *model)
{
    MainStack *stack = Main_Stack(g_object_new(main_stack_get_type(), NULL));

    // Initalize Menu
    gtk_popover_menu_set_menu_model(GTK_POPOVER_MENU(stack->popover), model);

    // Add Left Panel
    LeftPanel *panel = left_panel_new();
    left_panel_set_parent(panel, GTK_WINDOW(win));
    gtk_box_append(GTK_BOX(stack->left_box), GTK_WIDGET(panel));

    // Set stack properties
    gtk_widget_set_halign(stack->main_stack, GTK_ALIGN_FILL);
    gtk_widget_set_valign(stack->main_stack, GTK_ALIGN_FILL);

    return stack;
}

GtkWidget *main_stack_get_stack(MainStack *stack)
{
    return stack->main_stack;
}

void main_stack_set_color_theme(MainStack *stack, int gray)
{
    // Clear css classes
    gtk_widget_set_css_classes(stack->label_app, NULL);
    gtk_widget_set_css_classes(stack->label_time, NULL);

    if (gray < 128)
    {
        gtk_widget_add_css_class(stack->label_app, "label_white");
        gtk_widget_add_css_class(stack->label_time, "label_white");
    }
    else
    {
        gtk_widget_add_css_class(stack->label_app, "label_black");
        gtk_widget_add_css_class(stack->label_time, "label_black");
    }

    gtk_style_context_add_provider_for_display(gtk_widget_get_display(stack->label_app),
                                               GTK_STYLE_PROVIDER(stack->provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(stack->label_time),
                                               GTK_STYLE_PROVIDER(stack->provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}
