#pragma once

#include <gtkmm.h>
#include "MyHeader.hh"

class Game : public Gtk::Window
{
public:
    Game(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &tmp_builder);
    static Game *create();

private:
    // Title bar
    MyHeader header;

    // The main builder
    Glib::RefPtr<Gtk::Builder> game_builder;

    // Child Widgets
    // Gtk::Window *game_window;
    Gtk::Grid *main_grid;
    Gtk::Button *win_close, *win_mini;
    Gtk::Button *game_btn[4], *btngo, *btnexit;
    Gtk::Label *game_label;
    int game_index[4];

    // Signal Handlers
    void gamebtn_clicked(int *index);
    void btngo_clicked();
    // void btnexit_clicked();
    // void win_closed();
};