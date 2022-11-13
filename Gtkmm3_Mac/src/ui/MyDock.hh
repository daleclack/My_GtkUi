#pragma once

#include <gtkmm.h>
#include "MyFinder.hh"
#include "MyPrefs.hh"
#include "drawing.hh"
#include "FileWindow.hh"
#include "Game.hh"
#include "ImageApp.hh"
#include "Runner.hh"
#include "TextEditor.hh"
#include "Game24Win.hh"
#include "CalcApp.hh"
#include "MineSweeper.hh"

class MyDock : public Gtk::Box
{
public:
    MyDock(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_Glade);
    static MyDock *create(DockMode mode);
    void btnset_clicked();
    void mydock_init(Gtk::Window *window, Gtk::Image *background1);
    // void set_dock_mode(DockMode mode);

private:
    // Child widgets
    Gtk::Box *finder_box, *dock_box;
    Gtk::Button *btnlaunch, *btndraw, *btnfiles, *btncalc, 
        *btngame, *btnedit, *btnimage, *btnset, *btngame24, *btnmine;
    Gtk::Separator *separator_start, *separator_end;

    // Finder
    MyFinder finder;

    // LaunchPad
    bool launchpad_shown;
    Gtk::Stack *launchpad_stack;
    Gtk::Grid *default_page, *launchpad_page, *apps_grid;
    Gtk::Button *padaud, *paddraw, *padfile, *padgedit, *padgame, *padset, *padcalc, 
        *padimage, *padnote, *padedit, *padvlc, *padvlc_win32, *padrun, *padgame24, *padmine;
    Gtk::Label label1;

    // Window Preferences
    MyPrefs prefs_win;
    bool prefs_win_closed(GdkEventAny *event);
    void padset_clicked();

    // Drawing Window
    Drawing draw_app;
    bool draw_win_closed(GdkEventAny *event);
    void btndraw_clicked();
    void paddraw_clicked();
    void draw_win_hide();

    // Finder
    FileWindow file_app;
    bool file_win_closed(GdkEventAny *event);
    void btnfile_clicked();
    void padfile_clicked();

    // Calculator App
    CalcApp *calc_win;
    bool calc_win_closed(GdkEventAny *event);
    void calc_win_hide();
    void btncalc_clicked();
    void padcalc_clicked();

    // Guess Game
    Game *game_win;
    bool game_win_closed(GdkEventAny *event);
    void game_win_hide();
    void btngame_clicked();
    void padgame_clicked();

    // 24-Game
    Game24Win *game24_win;
    bool game24_win_closed(GdkEventAny *event);
    void game24_win_hide();
    void btngame24_clicked();
    void padgame24_clicked();

    // Image Viewer
    ImageApp image_win;
    bool image_win_closed(GdkEventAny *event);
    void btnimage_clicked();
    void padimage_clicked();

    // Text Editor
    TextEditor editor_win;
    bool editor_win_closed(GdkEventAny *event);
    void btnedit_clicked();
    void padedit_clicked();

    // MineSweeper
    MineSweeper mine_win;
    bool mine_win_closed(GdkEventAny *event);
    void btnmine_clicked();
    void padmine_clicked();

    // A Simple Runner
    GtkRunner runner1;

    // Window Control
    void window_ctrl(Gtk::Window &window, bool on_dock = true);

    // Other
    Gtk::Window *parent_win;
    Glib::RefPtr<Gtk::Builder> ref_builder;
    Glib::RefPtr<Gtk::CssProvider> provider;
    void apply_style(Gtk::Widget &widget);

    // Signal Handlers
    void btnlaunch_clicked();
    void padaud_clicked();
    void padgedit_clicked();
    void padvlc_clicked();
    void padvlc_win32_clicked();
    void padnote_clicked();
    void btnrun_clicked();
};
