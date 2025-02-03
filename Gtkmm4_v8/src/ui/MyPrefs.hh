#pragma once

#include <gtkmm.h>
#include "MyHeader.hh"
// #include "MainWin.hh"    // can't include this because of circular dependency

class MyPrefs : public Gtk::Window
{
public:
    MyPrefs(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);
    static MyPrefs *create(Gtk::Window &parent);

protected:
    bool on_close_request();

private:
    Glib::RefPtr<Gtk::Builder> ref_builder;

    // Child widgets
    Gtk::StackSwitcher *back_switcher;
    Gtk::Stack *back_stack;

    // Header bar
    MyHeader header;
};