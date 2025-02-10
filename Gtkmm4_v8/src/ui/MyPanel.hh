#pragma once

#include <gtkmm.h>
#include "AppMenu.hh"
#include "MyPrefs.hh"
#include "MineSweeper.hh"
#include "MyMediaPlayer.hh"
#include "FileWindow.hh"
#include "CalcApp.hh"
#include "Game.hh"
#include "Game24Win.hh"
#include "Runner.hh"

class MyPanel
{
public:
    Gtk::Box *panel_box;
    MyPanel();
    Gtk::Image *get_prefs_image();
    void set_prefs_win(MyPrefs *prefs);
    void set_internal_style(const Glib::RefPtr<Gtk::CssProvider> &provider);
    void set_parent_window(Gtk::Window &parent);
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

    // File Browser
    FileWindow file_window;

    // Calculator
    CalcApp *calc_window;

    // Guess Game
    Game *game_window;

    // 24 Game
    Game24Win *game24_window;

    // MineSweeper
    MineSweeper mine_window;

    // Media Player
    MyMediaPlayer media_window;

    // Runner
    Runner runner_window;

    // Signal handlers
    void window_ctrl(Gtk::Window &window);
    void btnstart_clicked();
    void btnfiles_clicked();
    bool filewin_closed();
    void btndraw_clicked();
    void btncalc_clicked();
    bool calcwin_closed();
    void btngame_clicked();
    bool gamewin_closed();
    void btnedit_clicked();
    void btnviewer_clicked();
    void btnset_clicked();
    bool setwin_closed();
    void btngame24_clicked();
    bool game24win_closed();
    void btnmine_clicked();
    bool minewin_closed();
    void btnmedia_clicked();
    bool mediawin_closed();

    // Static members
    static MyPanel *instance;
    static void padbtn_clicked(guint id);
};
