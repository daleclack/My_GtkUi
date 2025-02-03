#pragma once

#include <gtkmm.h>
#include "AppMenu.hh"

class MyPanel
{
public:
    Gtk::Box *panel_box;
    MyPanel();

private:
    Glib::RefPtr<Gtk::Builder> builder;

    // Child widgets
    Gtk::Button *btnstart;
    Gtk::Stack *apps_stack;
    Gtk::Box *apps_box;
    Gtk::ScrolledWindow *apps_sw;

    // Button and images for the app task manager
    Gtk::Button *btnfiles, *btndraw, *btncalc, *btngame,
        *btnedit, *btnviewer, *btnset, *btngame24, *btnmine;
    Gtk::Image *imagefiles, *imagedraw, *imagecalc, *imagegame,
        *imageedit, *imageviewer, *imageset, *imagegame24, *imagemine;

    // The App Menu
    AppMenu app_menu;

    // Signal handlers
    void btnstart_clicked();
    void btnfiles_clicked();
    void btndraw_clicked();
    void btncalc_clicked();
    void btngame_clicked();
    void btnedit_clicked();
    void btnviewer_clicked();
    void btnset_clicked();
    void btngame24_clicked();
    void btnmine_clicked();
};
