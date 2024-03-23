#pragma once

#include <gtkmm.h>
#include "Game24.hh"
#include "MyTitleBar.hh"

class Game24Win : public Gtk::Window
{
public:
    static Game24Win *create();
    Game24Win(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_Glade);

private:
    // Main GtkBuilder
    Glib::RefPtr<Gtk::Builder> ref_builder;

    // 24-Game Class
    MyTitleBar title_bar;
    Game24 main_game;
    int pos;
    bool focused, winned;
    Glib::ustring tmp;

    // Child widgets
    Gtk::Entry *entry_ans;
    Gtk::Label *label_numbers;
    Gtk::Button *btns[16], *btnstart, *btnnext, *btncheck, *btnclear, *btnexit;

    // Signal Handlers
    void btns_clicked(Gtk::Button *button);
    void btnstart_clicked();
    void btnnext_clicked();
    void btncheck_clicked();
    void btnclear_clicked();
    void entry_ans_focus();
};