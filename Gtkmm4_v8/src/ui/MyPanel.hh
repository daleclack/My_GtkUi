#pragma once

#include <gtkmm.h>
#include "AppMenu.hh"
#include "MyPrefs.hh"
#include "MineSweeper.hh"
#include "MyMediaPlayer.hh"

class MyPanel
{
public:
    Gtk::Box *panel_box;
    MyPanel();
    Gtk::Image *get_prefs_image();
    void set_prefs_win(MyPrefs *prefs);
    void set_internal_style(const Glib::RefPtr<Gtk::CssProvider> &provider);
    void hide_launchpad();

private:
    Glib::RefPtr<Gtk::Builder> builder;

    // Child widgets
    Gtk::Button *btnstart;
    Gtk::Stack *apps_stack;
    Gtk::Box *apps_box;
    Gtk::ScrolledWindow *apps_sw;

    // Button and images for the app task manager
    Gtk::Button *btnfiles, *btndraw, *btncalc, *btngame,
        *btnedit, *btnviewer, *btnset, *btngame24, *btnmine, *btnmedia;
    Gtk::Image *imagefiles, *imagedraw, *imagecalc, *imagegame,
        *imageedit, *imageviewer, *imageset, *imagegame24, *imagemine, *imagemedia;

    // The App Menu
    AppMenu app_menu;

    // Preferences window
    MyPrefs *prefs_window;

    // MineSweeper
    MineSweeper mine_window;

    // Media Player
    MyMediaPlayer media_window;

    // Signal handlers
    void window_ctrl(Gtk::Window &window);
    void btnstart_clicked();
    void btnfiles_clicked();
    void btndraw_clicked();
    void btncalc_clicked();
    void btngame_clicked();
    void btnedit_clicked();
    void btnviewer_clicked();
    void btnset_clicked();
    bool setwin_closed();
    void btngame24_clicked();
    void btnmine_clicked();
    bool minewin_closed();

    // Static members
    static MyPanel *instance;
    static void padbtn_clicked(guint id);
};
