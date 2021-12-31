#pragma once

#include <gtkmm.h>
#include "Game.hh"
#include "Runner.hh"
#include "TextEditor.hh"
#include "drawing.hh"
#include "FileWindow.hh"
#include "ImageApp.hh"

class LeftPanel{
public:
    LeftPanel();
    ~LeftPanel();
    void add_panel(Gtk::Window *parent,Gtk::Overlay &overlay);
    void file_manager();
private:
    //LeftPanel Builder
    Glib::RefPtr<Gtk::Builder> panel_builder;
    //Child widgets
    Gtk::Box *btnbox;
    Gtk::Popover *popover;
    Gtk::Button *btnaud,*btngedit,*btnvlc,*btnnote,*btnvlc_win32,
                *btngame,*btnfile,*btnimage,
                *panelgame,*btnrun,*btneditor,*panel_editor,
                *btndraw,*panel_draw,*panel_file,*panel_image;
    //Parent Window
    Gtk::Window *parent;
    Game *game1;
    //Timer for panel monitor
    sigc::connection paneltimer;
    bool on_timeout();
    //A Simple Runner
    GtkRunner runner1;
    //Text Editor
    TextEditor editor1;
    //Drawing App
    Drawing drawing_app;
    //File Manager
    FileWindow file_app;
    //Image Viewer
    ImageApp image_app;
    //Window Control Func
    void window_ctrl(Gtk::Window &ctrled_win,bool &running);
    //Signal Handlers for application start
    void btnaud_clicked();
    void btngedit_clicked();
    void btnvlc_clicked();
    void btnnote_clicked();
    void winvlc_clicked();
    void btngame_clicked();
    void btnrun_clicked();
    void btnedit_clicked();
    void btndraw_clicked();
    void btnfile_clicked();
    void btnimage_clicked();
};
