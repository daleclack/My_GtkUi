#pragma once

#include <gtkmm.h>

class MyDock : public Gtk::Box
{
public:
    MyDock(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> ref_Glade);
    MyDock * create();
private:
    
};
