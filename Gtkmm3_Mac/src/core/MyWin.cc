#include "MyWin.hh"
#include "winpe.xpm"

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

    //Add a menubar
    auto menubuilder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/menubar.xml");

    //Add widgets to menubox
    btnlogo.set_image_from_icon_name("My_GtkUI",Gtk::ICON_SIZE_BUTTON);
    btnlogo.set_relief(Gtk::RELIEF_NONE);
    btnlogo.set_use_popover(false);
    btnlogo.set_menu_model(get_menu(menubuilder,"main_menu"));
    menubox.pack_start(btnlogo,Gtk::PACK_SHRINK);

    btntitle.set_label("MyFinder");
    btntitle.set_relief(Gtk::RELIEF_NONE);
    btntitle.set_use_popover(false);
    btntitle.set_menu_model(get_menu(menubuilder,"title_menu"));
    menubox.pack_start(btntitle,Gtk::PACK_SHRINK);

    btnfile.set_label("File");
    btnfile.set_relief(Gtk::RELIEF_NONE);
    btnfile.set_use_popover(false);
    btnfile.set_menu_model(get_menu(menubuilder,"menu_file"));
    menubox.pack_start(btnfile,Gtk::PACK_SHRINK);

    btnedit.set_label("Edit");
    btnedit.set_relief(Gtk::RELIEF_NONE);
    btnedit.set_use_popover(false);
    btnedit.set_menu_model(get_menu(menubuilder,"menu_edit"));
    menubox.pack_start(btnedit,Gtk::PACK_SHRINK);

    btnshow.set_label("Show");
    btnshow.set_relief(Gtk::RELIEF_NONE);
    btnshow.set_use_popover(false);
    btnshow.set_menu_model(get_menu(menubuilder,"menu_show"));
    menubox.pack_start(btnshow,Gtk::PACK_SHRINK);

    btngoto.set_label("Go to");
    btngoto.set_relief(Gtk::RELIEF_NONE);
    btngoto.set_use_popover(false);
    btngoto.set_menu_model(get_menu(menubuilder,"menu_goto"));
    menubox.pack_start(btngoto,Gtk::PACK_SHRINK);

    btnwin.set_label("Windows");
    btnwin.set_relief(Gtk::RELIEF_NONE);
    btnwin.set_use_popover(false);
    btnwin.set_menu_model(get_menu(menubuilder,"menu_win"));
    menubox.pack_start(btnwin,Gtk::PACK_SHRINK);

    btnhelp.set_label("Help");
    btnhelp.set_relief(Gtk::RELIEF_NONE);
    btnhelp.set_use_popover(false);
    btnhelp.set_menu_model(get_menu(menubuilder,"menu_help"));
    menubox.pack_start(btnhelp,Gtk::PACK_SHRINK);

    //Add menu to window
    menubox.set_halign(Gtk::ALIGN_FILL);
    menubox.set_valign(Gtk::ALIGN_START);
    m_overlay.add_overlay(menubox);

    add(m_overlay);
    show_all_children();
}

Glib::RefPtr<Gio::Menu> MyWin::get_menu(Glib::RefPtr<Gtk::Builder> &builder, const Glib::ustring &id){
    auto object = builder->get_object(id);
    auto gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
    return gmenu;
}