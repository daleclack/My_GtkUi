#pragma once

//This is the Toppanel of the window,like the panel in gnome

#include <gtkmm.h>

class TopPanel : public Gtk::Box
{
public:
    TopPanel();
private:
    Gtk::Popover popover;
    Gtk::MenuButton menubtn;
    Glib::RefPtr<Gtk::Builder> menu_builder;
};
