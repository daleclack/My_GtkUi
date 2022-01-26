#include "MyStack.hh"

MyStack::MyStack(){
    // Get Widgets
    stackbuilder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/stack.ui");
    stackbuilder->get_widget("main_stack",stack);
    stackbuilder->get_widget("login_page",login_page);
    stackbuilder->get_widget("main_page",main_page);
    stackbuilder->get_widget("btnlogin",btnlogin);

    // Link Signals
    btnlogin->signal_clicked().connect(sigc::mem_fun(*this,&MyStack::btnlogin_clicked));

    //Add Widget
    main_page->pack_start(finder);
}

void MyStack::logout(){
    stack->set_visible_child(*login_page);
}

void MyStack::btnlogin_clicked(){
    stack->set_visible_child(*main_page);
}
