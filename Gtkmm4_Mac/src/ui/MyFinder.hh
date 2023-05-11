#pragma once

#include <gtkmm.h>

class MyFinder : public Gtk::Box
{
public:
    MyFinder();

private:
    // Child Widgets
    Gtk::MenuButton btnlogo, btntitle, btnfile, btnedit, btnshow,
        btngoto, btnwin, btnhelp;
    Gtk::MenuButton timer_button;
    Gtk::Popover time_popover;
    Gtk::Calendar calender;
    Gtk::Button find_button, menu_button, battery_button, 
                network_button, screen_button, audio_button;

    // Get Menu
    Glib::RefPtr<Gio::Menu> get_menu(Glib::RefPtr<Gtk::Builder> &builder, const Glib::ustring &id);

    //Style for finder
    Glib::RefPtr<Gtk::CssProvider> provider;

    //Timer
    sigc::connection mytimer;
    bool time_out();
};
