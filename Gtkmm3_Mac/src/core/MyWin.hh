#pragma once

#include <gtkmm.h>
#include "MyStack.hh"
#include "MyPrefs.hh"

class MyWin : public Gtk::ApplicationWindow{
public:
    MyWin();
private:
    //Child widgets
    Gtk::Overlay m_overlay;
    Gtk::Image m_background;
    Gtk::Box menubox;
    Gtk::Popover context_menu;

    //Gesture Control
    Glib::RefPtr<Gtk::GestureMultiPress> gesture;
    void press(int n_press,double x,double y);

    //Main Stack
    MyStack main_stack;

    //Window Preferences
    MyPrefs prefs_win;

    //Signal Handlers
    void logout_activated();
    void quit_activated();
    void about_activated();
    void back_actiavted();

};