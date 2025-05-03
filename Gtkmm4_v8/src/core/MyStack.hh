#pragma once

#include <gtkmm.h>
#include "MyPanel.hh"
#include "MyFinder.hh"
#include "MyPrefs.hh"
#include "config.hh"

class MyStack
{
public:
    Gtk::Stack *m_stack;
    MyStack();
    void create_prefs_win(Gtk::Window &parent);
    MyPrefs *get_prefs_win();
    void show_prefs_win();
    void bind_main_background(Gtk::Picture *background);

private:
    // Object builder
    Glib::RefPtr<Gtk::Builder> m_builder;

    // Child widgets
    Gtk::Box login_box;
    Gtk::Picture login_background;
    Gtk::Image login_image;
    Gtk::Label login_label;
    Gtk::Button login_button;
    Gtk::Overlay *login_page;
    Gtk::Box *main_page, *panel_area,
        *topmenu_area, *menu_area;

    // The Left Panel
    MyPanel panel1;

    // The Main Menu
    MyFinder finder1;

    // Preferences Windows
    MyPrefs *prefs_window;

    // Signal handlers
    void login_button_clicked();
};