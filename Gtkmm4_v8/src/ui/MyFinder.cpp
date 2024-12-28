#include "MyFinder.h"
#include <cstdio>
#include <ctime>
#include <string>

struct _MyFinder
{
    GtkBox parent_instance;
    GtkWidget *icon_image, *icon_find, *icon_menu, *icon_battery,
        *icon_network, *icon_screen, *icon_audio;
    GtkWidget *btn_icon, *btntitle, *btnfile, *btnedit,
        *btnshow, *btngoto, *btnwin, *btnhelp;
    GtkWidget *label_title, *labelfile, *labeledit, *labelshow,
        *labelgoto, *labelwin, *labelhelp, *time_label;
    GtkWidget *separator;
    GtkWidget *time_button, *time_popover, *calendar;
    GtkWidget *find_button, *menu_button, *battery_button,
        *network_button, *screen_button, *audio_button;
};

G_DEFINE_TYPE(MyFinder, my_finder, GTK_TYPE_BOX)

static void get_menu(GtkBuilder *builder, const char *id, GtkWidget *menu_button)
{
    // Initalize Menu button
    gtk_menu_button_set_has_frame(GTK_MENU_BUTTON(menu_button), FALSE);

    // Get Menu model and initalize the popover relate to the menubutton
    GMenuModel *menu_model = G_MENU_MODEL(gtk_builder_get_object(builder, id));
    gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(menu_button), menu_model);

    // Make the menu has no arrow and pack the start
    GtkPopover *popover = gtk_menu_button_get_popover(GTK_MENU_BUTTON(menu_button));
    gtk_popover_set_has_arrow(popover, FALSE);
    gtk_widget_set_halign(GTK_WIDGET(popover), GTK_ALIGN_START);
}

static gboolean timeout_func(gpointer data)
{
    // Get Localtime
    time_t t;
    t = time(NULL);
    struct tm *local;
    local = localtime(&t);
    char time_data[57];
    snprintf(time_data, sizeof(time_data), "%04d/%02d/%02d %02d:%02d:%02d", local->tm_year + 1900,
             local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
    gtk_label_set_label(GTK_LABEL(data), time_data);
    return TRUE;
}

static void my_finder_init(MyFinder *self)
{
    // Create widgets
    self->btn_icon = gtk_menu_button_new();
    self->btntitle = gtk_menu_button_new();
    self->btnfile = gtk_menu_button_new();
    self->btnedit = gtk_menu_button_new();
    self->btnshow = gtk_menu_button_new();
    self->btngoto = gtk_menu_button_new();
    self->btnwin = gtk_menu_button_new();
    self->btnhelp = gtk_menu_button_new();
    self->time_button = gtk_menu_button_new();
    self->label_title = gtk_label_new("My Gtk UI");
    self->labelfile = gtk_label_new("File");
    self->labeledit = gtk_label_new("Edit");
    self->labelshow = gtk_label_new("Show");
    self->labelgoto = gtk_label_new("Go to");
    self->labelwin = gtk_label_new("Windows");
    self->labelhelp = gtk_label_new("Help");
    self->time_label = gtk_label_new("2023/6/9 22:29:00");
    self->find_button = gtk_button_new();
    self->screen_button = gtk_button_new();
    self->battery_button = gtk_button_new();
    self->audio_button = gtk_button_new();
    self->network_button = gtk_button_new();
    self->menu_button = gtk_button_new();
    self->separator = gtk_label_new("   ");
    if (get_dark_mode(self))
    {
        self->icon_find = gtk_image_new_from_icon_name("finder-find-dark");;
        self->icon_screen = gtk_image_new_from_icon_name("finder-computer-dark");
        self->icon_battery = gtk_image_new_from_icon_name("finder-battery-dark");
        self->icon_audio = gtk_image_new_from_icon_name("finder_audio-dark");
        self->icon_network = gtk_image_new_from_icon_name("finder-wifi-dark");
    }
    else
    {
        self->icon_find = gtk_image_new_from_icon_name("finder-find");
        self->icon_screen = gtk_image_new_from_icon_name("finder-computer");
        self->icon_battery = gtk_image_new_from_icon_name("finder-battery");
        self->icon_audio = gtk_image_new_from_icon_name("finder_audio");
        self->icon_network = gtk_image_new_from_icon_name("finder-wifi");
    }
    self->icon_menu = gtk_image_new_from_icon_name("open-menu");
    self->icon_image = gtk_image_new_from_icon_name("My_GtkUI");
    gtk_widget_set_valign(self->icon_image, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(self->separator, TRUE);
    gtk_widget_set_halign(self->separator, GTK_ALIGN_FILL);
    gtk_menu_button_set_child(GTK_MENU_BUTTON(self->btn_icon), self->icon_image);
    gtk_button_set_child(GTK_BUTTON(self->find_button), self->icon_find);
    gtk_button_set_child(GTK_BUTTON(self->screen_button), self->icon_screen);
    gtk_button_set_child(GTK_BUTTON(self->battery_button), self->icon_battery);
    gtk_button_set_child(GTK_BUTTON(self->audio_button), self->icon_audio);
    gtk_button_set_child(GTK_BUTTON(self->network_button), self->icon_network);
    gtk_button_set_child(GTK_BUTTON(self->menu_button), self->icon_menu);
    // gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(self->btn_icon), "My_GtkUI");
    gtk_button_set_has_frame(GTK_BUTTON(self->find_button), FALSE);
    gtk_button_set_has_frame(GTK_BUTTON(self->screen_button), FALSE);
    gtk_button_set_has_frame(GTK_BUTTON(self->battery_button), FALSE);
    gtk_button_set_has_frame(GTK_BUTTON(self->audio_button), FALSE);
    gtk_button_set_has_frame(GTK_BUTTON(self->network_button), FALSE);
    gtk_button_set_has_frame(GTK_BUTTON(self->menu_button), FALSE);
    gtk_menu_button_set_has_frame(GTK_MENU_BUTTON(self->time_button), FALSE);

    // Add labels to the menu button
    gtk_menu_button_set_child(GTK_MENU_BUTTON(self->btntitle), self->label_title);
    gtk_menu_button_set_child(GTK_MENU_BUTTON(self->btnfile), self->labelfile);
    gtk_menu_button_set_child(GTK_MENU_BUTTON(self->btnedit), self->labeledit);
    gtk_menu_button_set_child(GTK_MENU_BUTTON(self->btnshow), self->labelshow);
    gtk_menu_button_set_child(GTK_MENU_BUTTON(self->btngoto), self->labelgoto);
    gtk_menu_button_set_child(GTK_MENU_BUTTON(self->btnwin), self->labelwin);
    gtk_menu_button_set_child(GTK_MENU_BUTTON(self->btnhelp), self->labelhelp);
    gtk_menu_button_set_child(GTK_MENU_BUTTON(self->time_button), self->time_label);

    // Create GtkBuilder for menu
    GtkBuilder *menu_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/menubar.xml");
    get_menu(menu_builder, "main_menu", self->btn_icon);
    get_menu(menu_builder, "title_menu", self->btntitle);
    get_menu(menu_builder, "menu_file", self->btnfile);
    get_menu(menu_builder, "menu_edit", self->btnedit);
    get_menu(menu_builder, "menu_show", self->btnshow);
    get_menu(menu_builder, "menu_goto", self->btngoto);
    get_menu(menu_builder, "menu_win", self->btnwin);
    get_menu(menu_builder, "menu_help", self->btnhelp);

    // Create timer
    g_timeout_add(1000, timeout_func, self->time_label);
    self->time_popover = gtk_popover_new();
    self->calendar = gtk_calendar_new();
    gtk_popover_set_child(GTK_POPOVER(self->time_popover), self->calendar);
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(self->time_button),
                                self->time_popover);

    // Pack widgets
    gtk_widget_set_hexpand(self->separator, TRUE);
    gtk_box_append(GTK_BOX(self), self->btn_icon);
    gtk_box_append(GTK_BOX(self), self->btntitle);
    gtk_box_append(GTK_BOX(self), self->btnfile);
    gtk_box_append(GTK_BOX(self), self->btnedit);
    gtk_box_append(GTK_BOX(self), self->btnshow);
    gtk_box_append(GTK_BOX(self), self->btngoto);
    gtk_box_append(GTK_BOX(self), self->btnwin);
    gtk_box_append(GTK_BOX(self), self->btnhelp);
    gtk_box_append(GTK_BOX(self), self->separator);
    gtk_box_append(GTK_BOX(self), self->screen_button);
    gtk_box_append(GTK_BOX(self), self->audio_button);
    gtk_box_append(GTK_BOX(self), self->network_button);
    gtk_box_append(GTK_BOX(self), self->battery_button);
    gtk_box_append(GTK_BOX(self), self->time_button);
    gtk_box_append(GTK_BOX(self), self->find_button);
    gtk_box_append(GTK_BOX(self), self->menu_button);
}

void my_finder_add_style(MyFinder *finder, GtkCssProvider *provider)
{
    // Main finder style
    GtkWidget *parent = gtk_widget_get_parent(GTK_WIDGET(finder));
    gtk_widget_add_css_class(parent, "finder_style");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(parent),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    // Make box in the buttons transparent
    gtk_widget_add_css_class(GTK_WIDGET(finder), "default_style");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(finder)),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void my_finder_apply_dpi(MyFinder *finder, double dpi)
{
    gtk_widget_set_size_request(finder->btnedit, 40 * dpi, 25 * dpi);
    gtk_widget_set_size_request(finder->btnfile, 40 * dpi, 25 * dpi);
    gtk_widget_set_size_request(finder->btngoto, 40 * dpi, 25 * dpi);
    gtk_widget_set_size_request(finder->btnhelp, 40 * dpi, 25 * dpi);
    gtk_widget_set_size_request(finder->btnshow, 40 * dpi, 25 * dpi);
    gtk_widget_set_size_request(finder->btnwin, 40 * dpi, 25 * dpi);
    gtk_widget_set_size_request(finder->btntitle, 40 * dpi, 25 * dpi);
    gtk_image_set_pixel_size(GTK_IMAGE(finder->icon_image), 20 * dpi);
    gtk_image_set_pixel_size(GTK_IMAGE(finder->icon_find), 16 * dpi);
    gtk_image_set_pixel_size(GTK_IMAGE(finder->icon_screen), 16 * dpi);
    gtk_image_set_pixel_size(GTK_IMAGE(finder->icon_battery), 16 * dpi);
    gtk_image_set_pixel_size(GTK_IMAGE(finder->icon_audio), 16 * dpi);
    gtk_image_set_pixel_size(GTK_IMAGE(finder->icon_network), 16 * dpi);
    gtk_image_set_pixel_size(GTK_IMAGE(finder->icon_menu), 16 * dpi);
}

static void my_finder_class_init(MyFinderClass *klass)
{
}

GtkWidget *my_finder_new(GtkOrientation orientation, int spacing)
{
    return GTK_WIDGET(g_object_new(my_finder_get_type(),
                                   "orientation", orientation, "spacing", spacing, NULL));
}

gboolean get_dark_mode(gpointer self)
{
    GtkIconTheme *theme = gtk_icon_theme_get_for_display(
        gtk_widget_get_display(GTK_WIDGET(self)));
    char *theme_name = gtk_icon_theme_get_theme_name(theme);

    // Get Last 4 chars
    char tmp_str[5] = {0};
    for (int i = 0; i < 4; i++)
    {
        tmp_str[i] = tolower(theme_name[strlen(theme_name) - 4 + i]);
    }
    g_free(theme_name);
    // g_print("%s", tmp_str);
    if (strncmp(tmp_str, "dark", 4) == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
