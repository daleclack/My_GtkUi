#pragma once

#include <gtkmm.h>

class WinConf{
public:
    WinConf(Gtk::Window *window);
    void get_config(int *width1,int *height1);
    void show_dialog();
private:
    //Window size
    int width,height;
    //The window to change config
    Gtk::Window *confwin;
    //Main Widgets
    Glib::RefPtr<Gtk::Builder> conf_builder;
    Gtk::Dialog *dialog;
    Gtk::SpinButton *width_spin,*height_spin;
    Gtk::Button *btnget,*btn_default;
    //Signal Handlers
    void get_size();
    void default_size();
    void dialog_response(int response_id);
};
