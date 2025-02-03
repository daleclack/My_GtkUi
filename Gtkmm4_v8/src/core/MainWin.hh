#pragma once

#include <gtkmm.h>
#include "MyStack.hh"

class MainWin : public Gtk::ApplicationWindow
{
public:
    MainWin(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder>& ref_builder1);
    static MainWin *create();
    virtual ~MainWin() {}

protected:
    
private:
    // Child widgets
    Gtk::Overlay *main_overlay;
    Gtk::Picture *main_background;

    // Builder for the window
    Glib::RefPtr<Gtk::Builder> ref_builder;

    // The context menu
    Gtk::PopoverMenu context_menu;
    Glib::RefPtr<Gtk::Builder> menu_builder;

    // Gesture for context menu
    Glib::RefPtr<Gtk::GestureClick> gesture_click;
    void gesture_pressed(int n_press, double x, double y);

    // Other widgets
    MyStack main_stack;

    // Signal Handlers
    void about_activate();
    void logout_activate();
    void shutdown_activate();
};
