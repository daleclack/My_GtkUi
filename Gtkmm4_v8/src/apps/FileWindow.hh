#pragma once

#include <gtkmm.h>
#include "MyHeader.hh"

class FileWindow : public Gtk::ApplicationWindow
{
public:
    FileWindow();

private:
    Gtk::Box main_box;
};