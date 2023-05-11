#include "MyStack.hh"
#include <iostream>

MyStack::MyStack()
{
    // Get Widgets
    stackbuilder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/stack.ui");
    stack = stackbuilder->get_widget<Gtk::Stack>("main_stack");
    login_page = stackbuilder->get_widget<Gtk::Box>("login_page");
    main_page = stackbuilder->get_widget<Gtk::Box>("main_page");
    btnlogin = stackbuilder->get_widget<Gtk::Button>("btnlogin");

    // Link Signals
    btnlogin->signal_clicked().connect(sigc::mem_fun(*this, &MyStack::btnlogin_clicked));
}

void MyStack::mystack_init(Gtk::Window *window, Gtk::Image *m_back)
{
    // main_dock->mydock_init(window, m_back);
}

void MyStack::show_prefs()
{
    // main_dock->btnset_clicked();
}

void MyStack::logout()
{
    stack->set_visible_child(*login_page);
}

void MyStack::btnlogin_clicked()
{
    stack->set_visible_child(*main_page);
}

// void MyStack::set_mydock_mode(DockMode mode)
// {
//     // main_dock->set_dock_mode(mode);
//     // switch (mode)
//     // {
//     // case DockMode::MODE_DOCK:
//     //     std::cout << "dock mode" << std::endl;
//     //     break;
//     // case DockMode::MODE_PANEL:
//     //     std::cout << "panel mode" << std::endl;
//     //     break;
//     // default:
//     //     std::cout << "undefined" << std::endl;
//     //     break;
//     // }
//     // Add Widget
//     main_dock = MyDock::create(mode);
//     main_dock->set_hexpand();
//     main_dock->set_vexpand();
//     main_page->pack_start(*main_dock);
// }
