#pragma once

#include <gtkmm.h>

class MyTitleBar
{
public:
    MyTitleBar();
    void set_ctrl_win(Gtk::Window *window); // the Controlled window
    void set_title(const Glib::ustring &title); // The title
    void pack_start(Gtk::Widget &widget);   // Same to the GtkHeaderBar funcs
    void pack_end(Gtk::Widget &widget);
private:
    Gtk::HeaderBar header;
    Glib::RefPtr<Gtk::CssProvider> provider;
    Gtk::Button btn_close, btn_mini;
    Gtk::Label title_label;
    Gtk::Window *ctrl_win;
    void ctrl_window_close();
    void ctrl_window_hide();
};
