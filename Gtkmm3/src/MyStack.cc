#include "MyStack.hh"

MyStack::MyStack(){
    //Create a builder
    ref_builder=Gtk::Builder::create_from_file("../res/main_stack.ui");
    
    //Get widgets
    ref_builder->get_widget("main_stack",stack);
    ref_builder->get_widget("login_overlay",login_overlay);
    ref_builder->get_widget("user_box",user_box);
    ref_builder->get_widget("user_image",user_image);
    ref_builder->get_widget("login_btn",login_btn);
    ref_builder->get_widget("main_overlay",main_overlay);

    //Initalize Interface
    user_image->set_from_icon_name("my_user",Gtk::ICON_SIZE_DIALOG);
    login_btn->signal_clicked().connect(sigc::mem_fun(*this,&MyStack::btnlogin_clicked));
    login_overlay->add_overlay(*user_box);

    //Add Toppanel
    panel1.set_halign(Gtk::ALIGN_END);
    panel1.set_valign(Gtk::ALIGN_START);
    main_overlay->add_overlay(panel1);
}

void MyStack::add_stack(Gtk::Box &vbox){
    vbox.pack_start(*stack);
}

void MyStack::btnlogin_clicked(){
    stack->set_visible_child(*main_overlay);
}