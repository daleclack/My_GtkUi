#pragma once

#include <gtkmm.h>
#include "Game.hh"
#include "Runner.hh"
#include "TextEditor.hh"

class LeftPanel{
public:
    LeftPanel();
    void add_panel(Gtk::Window *parent,Gtk::Overlay &overlay);
private:
    //LeftPanel Builder
    Glib::RefPtr<Gtk::Builder> panel_builder;
    //Child widgets
    Gtk::Box *btnbox;
    Gtk::Popover *popover;
    Gtk::Button *btnaud,*btngedit,*btnvlc,*btnnote,*btnvlc_win32,*btngame,*panelgame,*btnrun,*btneditor;
    //Parent Window
    Gtk::Window *parent;
    Game game1;
    //Timer for panel monitor
    sigc::connection paneltimer;
    bool on_timeout();
    //A Simple Runner
    GtkRunner runner1;
    //Text Editor
    TextEditor editor1;
    //Signal Handlers for application start
    void btnaud_clicked();
    void btngedit_clicked();
    void btnvlc_clicked();
    void btnnote_clicked();
    void winvlc_clicked();
    void btngame_clicked();
    void btnrun_clicked();
    void btnedit_clicked();
};
