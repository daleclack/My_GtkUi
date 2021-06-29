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
    Gtk::Popover *popover;
    Gtk::Button *btnaud,*btngedit,*btnvlc,*btnnote,*btnvlc_win32;
    //Parent Window
    Gtk::Window *parent;
    //Signal Handlers for application start
    void btnaud_clicked();
    void btngedit_clicked();
    void btnvlc_clicked();
    void btnnote_clicked();
    void winvlc_clicked();
};
