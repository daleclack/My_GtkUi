#pragma once

#include <gtkmm.h>
#include "MyDock.hh"

class MyStack
{
public:
    MyStack();
    void logout();
    void mystack_init(Gtk::Window *window, Gtk::Image *m_back);
    void show_prefs();
    void set_mydock_mode(DockMode mode);

    // Main Widget
    Gtk::Stack *stack;
private:
    Glib::RefPtr<Gtk::Builder> stackbuilder;

    // Child widgets
    Gtk::Box *login_page, *main_page;
    Gtk::Button *btnlogin;
    Gtk::Label *user_label;

    // Dock Widget
    MyDock *main_dock;

    // CssProvider for login page
    Glib::RefPtr<Gtk::CssProvider> provider;

    // Signal Handlers
    void btnlogin_clicked();
};
