#include "MyFinder.h"
#include <cstdio>
#include <ctime>

struct _MyFinder
{
    GtkBox parent_instance;
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
    self->separator = gtk_label_new("   ");
    self->find_button = gtk_button_new_from_icon_name("finder-find");
    self->screen_button = gtk_button_new_from_icon_name("finder-computer");
    self->battery_button = gtk_button_new_from_icon_name("finder-battery");
    self->audio_button = gtk_button_new_from_icon_name("finder_audio");
    self->network_button = gtk_button_new_from_icon_name("finder-wifi");
    self->menu_button = gtk_button_new_from_icon_name("open-menu");
    gtk_widget_set_hexpand(self->separator, TRUE);
    gtk_widget_set_halign(self->separator, GTK_ALIGN_FILL);
    gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(self->btn_icon), "My_GtkUI");
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
    // Make box in the buttons transparent
    gtk_widget_add_css_class(finder->btn_icon, "finder_buttons");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(finder->btn_icon),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_widget_add_css_class(GTK_WIDGET(finder->btntitle), "finder_buttons");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(finder->btntitle),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_widget_add_css_class(GTK_WIDGET(finder->btnfile), "finder_buttons");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(finder->btnfile),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    gtk_widget_add_css_class(GTK_WIDGET(finder->btnedit), "finder_buttons");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(finder->btnedit),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_widget_add_css_class(GTK_WIDGET(finder->btnshow), "finder_buttons");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(finder->btnshow),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    gtk_widget_add_css_class(GTK_WIDGET(finder->btngoto), "finder_buttons");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(finder->btngoto),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    gtk_widget_add_css_class(GTK_WIDGET(finder->btnwin), "finder_buttons");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(finder->btnwin),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    gtk_widget_add_css_class(GTK_WIDGET(finder->btnhelp), "finder_buttons");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(finder->btnhelp),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    gtk_widget_add_css_class(GTK_WIDGET(finder->menu_button), "finder_buttons");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(finder->menu_button),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
}

static void my_finder_class_init(MyFinderClass *klass)
{
}

GtkWidget *my_finder_new(GtkOrientation orientation, int spacing)
{
    return GTK_WIDGET(g_object_new(my_finder_get_type(),
                                   "orientation", orientation, "spacing", spacing, NULL));
}
