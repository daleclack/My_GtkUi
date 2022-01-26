#include "MyWin.hh"
#include "winpe.xpm"
#include <iostream>

MyWin::MyWin()
:menubox(Gtk::ORIENTATION_HORIZONTAL,0)
{
    //Initalize window
    set_default_size(1024,576);
    set_icon_name("My_GtkUI");
    set_title("My GtkUI macOS Version");

    //Add background
    auto pixbuf = Gdk::Pixbuf::create_from_xpm_data(winpe);
    auto sized = pixbuf->scale_simple(1024,576,Gdk::INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(m_background.gobj(),sized->gobj());
    m_overlay.add(m_background);

    //Add Action for menus
    add_action("logout",sigc::mem_fun(*this,&MyWin::logout_activated));
    add_action("quit",sigc::mem_fun(*this,&MyWin::quit_activated));

    //Add Stack
    m_overlay.add_overlay(*(main_stack.stack));

    //Add context menu
    auto menubuilder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/mainmenu.xml");

    add(m_overlay);
    show_all_children();
}

void MyWin::logout_activated(){
    main_stack.logout();
}

void MyWin::quit_activated(){
    hide();
}
