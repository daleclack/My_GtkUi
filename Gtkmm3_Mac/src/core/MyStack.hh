#pragma once

#include <gtkmm.h>

class MyStack{
public:
    MyStack();
    void add_stack(Gtk::Overlay &overlay);

private:
    Gtk::Stack * main_stack;
};
