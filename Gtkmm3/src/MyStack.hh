#pragma once

#include <gtkmm.h>
#include "TopPanel.hh"
#include "LeftPanel.hh"

class MyStack{
public:
    MyStack(Gtk::Window *parent1);
    void add_stack(Gtk::Box &vbox);
    Gtk::Stack *stack;
private:
    //Parent Window
    Gtk::Window *parent;
    //Main builder
    Glib::RefPtr<Gtk::Builder> ref_builder;
    //Child widgets
    Gtk::Overlay *main_overlay,*login_overlay;
    Gtk::Box *user_box;
    Gtk::Image *user_image;
    Gtk::Button *login_btn;
    //TopPanel and LeftPanel
    Gtk::Box labelbox;
    Gtk::Label toplabel;
    TopPanel panel1;
    LeftPanel panel2;
    //Signal Handlers
    void btnlogin_clicked();
};
