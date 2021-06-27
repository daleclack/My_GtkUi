#pragma once

#include <gtkmm.h>
#include "TopPanel.hh"

class MyStack{
public:
    MyStack();
    void add_stack(Gtk::Box &vbox);
    Gtk::Stack *stack;
private:
    //Main builder
    Glib::RefPtr<Gtk::Builder> ref_builder;
    //Child widgets
    Gtk::Overlay *main_overlay,*login_overlay;
    Gtk::Box *user_box;
    Gtk::Image *user_image;
    Gtk::Button *login_btn;
    TopPanel panel1;
    //Signal Handlers
    void btnlogin_clicked();
};
