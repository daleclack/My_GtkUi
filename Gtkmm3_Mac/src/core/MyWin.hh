#pragma once

#include <gtkmm.h>
#include "MyStack.hh"

class MyWin : public Gtk::ApplicationWindow{
public:
    MyWin();
private:
    //Child widgets
    Gtk::Overlay m_overlay;
    Gtk::Image m_background;
    Gtk::Box menubox;
    Gtk::DrawingArea draw_area;

    //Main Stack
    MyStack main_stack;

    //Signal Handlers
    void logout_activated();
    void quit_activated();

};