#include "MyTitleBar.h"

struct _MyTitleBar
{
    GObject parent_instance;
    GtkWidget *ctrl_window;
    GtkWidget *header;
    GtkWidget *btn_close, *btn_mini;
    GtkWidget *img_close, *img_mini;
};

G_DEFINE_TYPE(MyTitleBar, my_titlebar, G_TYPE_OBJECT)

static void ctrl_win_hide(MyTitleBar *self)
{
    gtk_widget_set_visible(self->ctrl_window, FALSE);
}

static void ctrl_win_close(MyTitleBar *self)
{
    gtk_window_close(GTK_WINDOW(self->ctrl_window));
}

void my_titlebar_set_dpi_scale(MyTitleBar *title_bar, double dpi_scale)
{
    // Button size
    gtk_widget_set_size_request(title_bar->btn_close, 18 * dpi_scale, 18 * dpi_scale);
    gtk_widget_set_size_request(title_bar->btn_mini, 18 * dpi_scale, 18 * dpi_scale);
    gtk_widget_set_margin_bottom(title_bar->btn_close, 2 * dpi_scale);
    gtk_widget_set_margin_end(title_bar->btn_close, 2 * dpi_scale);
    gtk_widget_set_margin_start(title_bar->btn_close, 2 * dpi_scale);
    gtk_widget_set_margin_top(title_bar->btn_close, 2 * dpi_scale);
    gtk_widget_set_margin_bottom(title_bar->btn_mini, 2 * dpi_scale);
    gtk_widget_set_margin_end(title_bar->btn_mini, 2 * dpi_scale);
    gtk_widget_set_margin_start(title_bar->btn_mini, 2 * dpi_scale);
    gtk_widget_set_margin_top(title_bar->btn_mini, 2 * dpi_scale);

    // Image size of the buttons
    gtk_image_set_pixel_size(GTK_IMAGE(title_bar->img_close), 12 * dpi_scale);
    gtk_image_set_pixel_size(GTK_IMAGE(title_bar->img_mini), 12 * dpi_scale);
}

void _my_titlebar_set_window(MyTitleBar *self, GtkWidget *window)
{
    gtk_window_set_titlebar(GTK_WINDOW(window), self->header);
    self->ctrl_window = window;
}

void _my_titlebar_pack_end(MyTitleBar *self, GtkWidget *widget)
{
    gtk_widget_set_css_classes(widget, NULL);
    gtk_widget_add_css_class(widget, "ctrl_btn");
    gtk_header_bar_pack_end(GTK_HEADER_BAR(self->header), widget);
}

static void my_titlebar_init(MyTitleBar *self)
{
    // Create widgets
    self->header = gtk_header_bar_new();
    gtk_widget_set_css_classes(self->header, NULL);
    gtk_widget_add_css_class(self->header, "mytitlebar");

    // Initalize headerbar
    gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(self->header), FALSE);

    // Add a close button
    // self->btn_close = gtk_button_new_with_label("\u00d7");
    self->img_close = gtk_image_new_from_icon_name("header_close");
    self->btn_close = gtk_button_new();
    gtk_button_set_child(GTK_BUTTON(self->btn_close), self->img_close);
    gtk_widget_set_css_classes(self->btn_close, NULL);
    gtk_widget_add_css_class(self->btn_close, "ctrl_btn");
    gtk_widget_add_css_class(self->btn_close, "close");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(self->header), self->btn_close);

    // Add a minimize button
    self->img_mini = gtk_image_new_from_icon_name("header_minimize");
    self->btn_mini = gtk_button_new();
    gtk_button_set_child(GTK_BUTTON(self->btn_mini), self->img_mini);
    gtk_widget_set_css_classes(self->btn_mini, NULL);
    gtk_widget_add_css_class(self->btn_mini, "ctrl_btn");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(self->header), self->btn_mini);

    // Add style for headerbar
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(provider, "/org/gtk/daleclack/title_style.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(self->header)),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_signal_connect_swapped(self->btn_close, "clicked", G_CALLBACK(ctrl_win_close), self);
    g_signal_connect_swapped(self->btn_mini, "clicked", G_CALLBACK(ctrl_win_hide), self);
}

static void my_titlebar_class_init(MyTitleBarClass *klass)
{
}

MyTitleBar *my_titlebar_new()
{
    return My_TitleBar(g_object_new(my_titlebar_get_type(), NULL));
}
