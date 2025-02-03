#pragma once

#include <gtkmm.h>

class MyHeader
{
public:
    MyHeader();
    void set_ctrl_window(Gtk::Window *ctrl_window1);
    void set_title_bar(Gtk::Window &window);
    void set_dpi_scale(double dpi_scale);

private:
    Gtk::Button btn_close, btn_minimize;
    Gtk::Image img_close, img_minimize;
    Gtk::HeaderBar header;
    Gtk::Window *ctrl_window;

    // Signal handlers:
    void ctrl_win_close();
    void ctrl_win_minimize();
};
