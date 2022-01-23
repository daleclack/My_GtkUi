#pragma once

#include <gtkmm.h>

class MyWin : public Gtk::ApplicationWindow{
public:
    MyWin();
private:
    //Child widgets
    Gtk::Overlay m_overlay;
    Gtk::Image m_background;
    Gtk::Box menubox;
    Gtk::MenuButton btnlogo, btntitle, btnfile, btnedit, btnshow, 
                    btngoto, btnwin, btnhelp;
    Gtk::MenuButton timer_button;
    Gtk::Popover time_popover;
    Gtk::Calendar calender;

    //Get Menu
    Glib::RefPtr<Gio::Menu> get_menu(Glib::RefPtr<Gtk::Builder> &builder, const Glib::ustring &id);

    //Timer auto update
    sigc::connection mytimer;
};