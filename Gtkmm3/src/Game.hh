#pragma once

#include <gtkmm.h>

class Game{
public:
    Game();
    bool running;
    void show_game_window(Gtk::Window &parent);
    Glib::RefPtr<Gdk::Window> get_window();
    //Behaver like Gtk::Window::iconify and Gtk::Window::deiconify
    void iconify();
    void deiconify();
private:
    //The main builder
    Glib::RefPtr<Gtk::Builder> game_builder;
    //Child Widgets
    Gtk::Window *game_window;
    Gtk::Button *win_close,*win_mini;
    Gtk::Button *game_btn[4],*btngo,*btnexit;
    Gtk::Label *game_label;
    int game_index[4];
    //Signal Handlers
    void gamebtn_clicked(int *index);
    void btngo_clicked();
    void btnexit_clicked();
    void win_closed();
    bool on_delete_event(GdkEventAny *event);
};