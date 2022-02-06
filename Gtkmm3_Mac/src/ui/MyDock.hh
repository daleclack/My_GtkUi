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

class MyDock : public Gtk::Box
{
public:
    MyDock(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_Glade);
    static MyDock *create();
    void btnset_clicked();
    void mydock_init(Gtk::Window *window, Gtk::Image *background1);

private:
    // Child widgets
    Gtk::Box *finder_box, *dock_box;
    Gtk::Button *btnlaunch, *btndraw, *btnfiles,
        *btngame, *btnedit, *btnimage, *btnset;
    Gtk::Separator *separator_start, *separator_end;

    // Finder
    MyFinder finder;

    // LaunchPad
    bool launchpad_shown;
    Gtk::Stack *launchpad_stack;
    Gtk::Grid *default_page, *launchpad_page;
    Gtk::Button *padaud, *paddraw, *padfile, *padgedit, *padgame, *padset,
        *padimage, *padnote, *padedit, *padvlc, *padvlc_win32, *padrun;
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

    // Finder
    FileWindow file_app;
    bool file_win_closed(GdkEventAny *event);
    void btnfile_clicked();
    void padfile_clicked();

    // Guess Game
    Game *game_win;
    bool game_win_closed(GdkEventAny *event);
    void game_win_hide();
    void btngame_clicked();
    void padgame_clicked();

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

    // A Simple Runner
    GtkRunner runner1;

    // Window Control
    void window_ctrl(Gtk::Window &window, bool on_dock = true);

    // Other
    Gtk::Window *parent_win;
    Glib::RefPtr<Gtk::Builder> ref_builder;
    Glib::RefPtr<Gtk::CssProvider> provider;

    // Signal Handlers
    void btnlaunch_clicked();
    void padaud_clicked();
    void padgedit_clicked();
    void padvlc_clicked();
    void padvlc_win32_clicked();
    void padnote_clicked();
    void btnrun_clicked();
};
