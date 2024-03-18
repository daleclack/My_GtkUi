#include "MyStack.hh"
#include <iostream>

MyStack::MyStack()
{
    // Get Widgets
    stackbuilder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/stack.ui");
    stackbuilder->get_widget("main_stack", stack);
    stackbuilder->get_widget("login_page", login_page);
    stackbuilder->get_widget("main_page", main_page);
    stackbuilder->get_widget("btnlogin", btnlogin);
    stackbuilder->get_widget("user_label", user_label);

    // Link Signals
    btnlogin->signal_clicked().connect(sigc::mem_fun(*this, &MyStack::btnlogin_clicked));

    // Create provider
    provider = Gtk::CssProvider::create();
    provider->load_from_resource("/org/gtk/daleclack/login_style.css");
    auto style1 = user_label->get_style_context();
    style1->add_provider(provider, G_MAXUINT);
    auto style2 = btnlogin->get_style_context();
    style2->add_provider(provider, G_MAXUINT);
}

void MyStack::mystack_init(Gtk::Window *window, Gtk::Image *m_back)
{
    main_dock->mydock_init(window, m_back);
}

void MyStack::show_prefs()
{
    main_dock->btnset_clicked();
}

void MyStack::logout()
{
    stack->set_visible_child(*login_page);
}

void MyStack::btnlogin_clicked()
{
    stack->set_visible_child(*main_page);
}

void MyStack::set_mydock_mode(DockMode mode)
{
    // main_dock->set_dock_mode(mode);
    // switch (mode)
    // {
    // case DockMode::MODE_DOCK:
    //     std::cout << "dock mode" << std::endl;
    //     break;
    // case DockMode::MODE_PANEL:
    //     std::cout << "panel mode" << std::endl;
    //     break;
    // default:
    //     std::cout << "undefined" << std::endl;
    //     break;
    // }
    // Add Widget
    main_dock = MyDock::create(mode);
    main_dock->set_hexpand();
    main_dock->set_vexpand();
    main_page->pack_start(*main_dock);
}
