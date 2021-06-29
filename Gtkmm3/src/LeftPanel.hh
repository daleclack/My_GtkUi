#pragma once

#include <gtkmm.h>

class LeftPanel{
public:
    LeftPanel(Gtk::Window *parent1);
    void add_panel(Gtk::Overlay &overlay);
private:
    //LeftPanel Builder
    Glib::RefPtr<Gtk::Builder> panel_builder;
    //Child widgets
    Gtk::Box *btnbox;
    //Parent Window
    Gtk::Window *parent;
};
