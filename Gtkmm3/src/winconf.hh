#pragma once

#include <gtkmm.h>

class WinConf{
public:
    WinConf(Gtk::Window *window);
    void show_dialog();
    void dialog_response(int response_id);
private:
    //The window to change config
    Gtk::Window *confwin;
    //Main Widgets
    Glib::RefPtr<Gtk::Builder> conf_builder;
    Gtk::SpinButton *width_spin,*height_spin;
    Gtk::Button *btnget;
};

void get_config(int *width,int *height);
