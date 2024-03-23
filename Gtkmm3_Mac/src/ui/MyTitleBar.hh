#pragma once

#include <gtkmm.h>

class MyTitleBar
{
public:
    MyTitleBar();
    void set_ctrl_win(Gtk::Window *window);
    void set_title(const Glib::ustring &title);
    Gtk::HeaderBar header;
private:
    Glib::RefPtr<Gtk::CssProvider> provider;
    Gtk::Button btn_close, btn_mini;
    Gtk::Label title_label;
    Gtk::Window *ctrl_win;
    void ctrl_window_close();
    void ctrl_window_hide();
};
