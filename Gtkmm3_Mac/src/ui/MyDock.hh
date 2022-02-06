#pragma once

#include <gtkmm.h>
#include "MyFinder.hh"
#include "MyPrefs.hh"

class MyDock : public Gtk::Box
{
public:
    MyDock(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_Glade);
    static MyDock *create();
    void btnset_clicked();
    void mydock_init(Gtk::Window * window, Gtk::Image *background1);

private:
    // Child widgets
    Gtk::Box *finder_box, *dock_box;
    Gtk::Button *btnlaunch, *btndraw, *btnfiles,
        *btngame, *btnedit, *btnimage, *btnset;

    // Finder
    MyFinder finder;

    // LaunchPad
    bool launchpad_shown;
    Gtk::Stack *launchpad_stack;
    Gtk::Grid *default_page, *launchpad_page;

    //Window Preferences
    MyPrefs prefs_win;

    // Other
    Gtk::Window *parent_win;
    Glib::RefPtr<Gtk::Builder> ref_builder;
    Glib::RefPtr<Gtk::CssProvider> provider;

    // Signal Handlers
    void btnlaunch_clicked();
};
