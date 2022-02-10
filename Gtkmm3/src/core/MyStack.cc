#include "MyStack.hh"

MyStack::MyStack(Gtk::Window *parent1)
{
    //Create a builder
    ref_builder=Gtk::Builder::create_from_resource("/GtkUI/main_stack.ui");
    
    //Get widgets
    ref_builder->get_widget("main_stack",stack);
    ref_builder->get_widget("login_overlay",login_overlay);
    ref_builder->get_widget("user_box",user_box);
    ref_builder->get_widget("user_image",user_image);
    ref_builder->get_widget("login_btn",login_btn);
    ref_builder->get_widget("home_btn",home_btn);
    ref_builder->get_widget("main_overlay",main_overlay);

    //Initalize Interface
    user_image->set_from_icon_name("my_user",Gtk::ICON_SIZE_DIALOG);
    login_btn->signal_clicked().connect(sigc::mem_fun(*this,&MyStack::btnlogin_clicked));
    home_btn->signal_clicked().connect(sigc::mem_fun(*this,&MyStack::btnhome_clicked));
    login_overlay->add_overlay(*user_box);

    panel1.set_halign(Gtk::ALIGN_FILL);
    panel1.set_valign(Gtk::ALIGN_START);
    main_overlay->add_overlay(panel1);

    //Add LeftPanel
    panel2.add_panel(parent1,*main_overlay);

    //Bind Parent Window
    parent=parent1;
}

void MyStack::add_stack(Gtk::Box &vbox){
    vbox.pack_start(*stack);
}

void MyStack::btnlogin_clicked(){
    stack->set_visible_child(*main_overlay);
}

void MyStack::logout(){
    stack->set_visible_child(*login_overlay);
}

void MyStack::btnhome_clicked(){
    panel2.file_manager();
}
