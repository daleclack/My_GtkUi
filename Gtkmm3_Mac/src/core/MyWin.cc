#include "MyWin.hh"
#include "winpe.xpm"

MyWin::MyWin(){
    //Initalize window
    set_default_size(1024,576);
    set_icon_name("My_GtkUI");
    set_title("My GtkUI macOS Version");

    //Add background
    auto pixbuf = Gdk::Pixbuf::create_from_xpm_data(winpe);
    auto sized = pixbuf->scale_simple(1024,576,Gdk::INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(m_background.gobj(),sized->gobj());
    m_overlay.add(m_background);

    //Add a menubar
    auto menubuilder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/menubar.xml");
    auto object = menubuilder->get_object("model");
    auto gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);

    menubar = Gtk::make_managed<Gtk::MenuBar>(gmenu);
    menubar->set_halign(Gtk::ALIGN_FILL);
    menubar->set_valign(Gtk::ALIGN_START);
    m_overlay.add_overlay(*menubar);

    add(m_overlay);
    show_all_children();
}