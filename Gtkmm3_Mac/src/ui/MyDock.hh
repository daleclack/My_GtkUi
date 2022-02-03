#pragma once

#include <gtkmm.h>
#include "MyFinder.hh"

class MyDock : public Gtk::Box
{
public:
    MyDock(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> ref_Glade);
    static MyDock * create();

private:
    // Child widgets
    Gtk::Box * finder_box;
    //Finder
    MyFinder finder;

    Gtk::Window * parent_win;
    Glib::RefPtr<Gtk::Builder> ref_builder;
};
