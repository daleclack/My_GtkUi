#pragma once

#include <gtkmm.h>

class MyWin : public Gtk::ApplicationWindow{
public:
    MyWin();
private:
    //Child widgets
    Gtk::Overlay m_overlay;
    Gtk::Image m_background;
    Gtk::MenuBar * menubar;
};