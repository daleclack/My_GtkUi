#pragma once

#include <gtkmm.h>

class MyHeader
{
public:
    MyHeader();

private:
    Gtk::Button btn_close, btn_minimize;
    Gtk::Image img_close, img_minimize;
    Gtk::HeaderBar header;
    Gtk::Window *ctrl_window;
};
