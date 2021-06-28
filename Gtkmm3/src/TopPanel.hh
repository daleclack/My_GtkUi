#pragma once

//This is the Toppanel of the window,like the panel in gnome

#include <gtkmm.h>
#include <gtkmm/lockbutton.h>

class TopPanel : public Gtk::Box
{
public:
    TopPanel();
private:
    //The main menu
    Gtk::Popover popover;
    Gtk::MenuButton menubtn;
    Glib::RefPtr<Gtk::Builder> menu_builder;
    //Timer
    sigc::connection mytimer;
    Gtk::Label time_label;
    const int timer_value;
    bool on_timeout();
    char time_string[57];
    //Other Things
    Glib::RefPtr<Gtk::Builder> panel_builder;
    Gtk::VolumeButton *volume1;
    Gtk::Spinner *spin1;
    Gtk::LockButton *lockbtn;
};
