#pragma once

#include <gtkmm.h>

class LeftPanel{
public:
    LeftPanel();
    void add_panel(Gtk::Overlay &overlay);
private:
    //LeftPanel Builder
    Glib::RefPtr<Gtk::Builder> panel_builder;
    //Child widgets
    Gtk::Box *btnbox;
};
