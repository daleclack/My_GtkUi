#include "MyFinder.hh"

MyFinder::MyFinder(){
    //Add a menubar
    auto menubuilder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/menubar.xml");

    //Add widgets to menubox
    btnlogo.set_image_from_icon_name("My_GtkUI",Gtk::ICON_SIZE_BUTTON);
    btnlogo.set_relief(Gtk::RELIEF_NONE);
    btnlogo.set_use_popover(false);
    btnlogo.set_menu_model(get_menu(menubuilder,"main_menu"));
    pack_start(btnlogo,Gtk::PACK_SHRINK);

    btntitle.set_label("MyFinder");
    btntitle.set_relief(Gtk::RELIEF_NONE);
    btntitle.set_use_popover(false);
    btntitle.set_menu_model(get_menu(menubuilder,"title_menu"));
    pack_start(btntitle,Gtk::PACK_SHRINK);

    btnfile.set_label("File");
    btnfile.set_relief(Gtk::RELIEF_NONE);
    btnfile.set_use_popover(false);
    btnfile.set_menu_model(get_menu(menubuilder,"menu_file"));
    pack_start(btnfile,Gtk::PACK_SHRINK);

    btnedit.set_label("Edit");
    btnedit.set_relief(Gtk::RELIEF_NONE);
    btnedit.set_use_popover(false);
    btnedit.set_menu_model(get_menu(menubuilder,"menu_edit"));
    pack_start(btnedit,Gtk::PACK_SHRINK);

    btnshow.set_label("Show");
    btnshow.set_relief(Gtk::RELIEF_NONE);
    btnshow.set_use_popover(false);
    btnshow.set_menu_model(get_menu(menubuilder,"menu_show"));
    pack_start(btnshow,Gtk::PACK_SHRINK);

    btngoto.set_label("Go to");
    btngoto.set_relief(Gtk::RELIEF_NONE);
    btngoto.set_use_popover(false);
    btngoto.set_menu_model(get_menu(menubuilder,"menu_goto"));
    pack_start(btngoto,Gtk::PACK_SHRINK);

    btnwin.set_label("Windows");
    btnwin.set_relief(Gtk::RELIEF_NONE);
    btnwin.set_use_popover(false);
    btnwin.set_menu_model(get_menu(menubuilder,"menu_win"));
    pack_start(btnwin,Gtk::PACK_SHRINK);

    btnhelp.set_label("Help");
    btnhelp.set_relief(Gtk::RELIEF_NONE);
    btnhelp.set_use_popover(false);
    btnhelp.set_menu_model(get_menu(menubuilder,"menu_help"));
    pack_start(btnhelp,Gtk::PACK_SHRINK);

    //Add menu to window
    set_halign(Gtk::ALIGN_FILL);
    set_valign(Gtk::ALIGN_START);

    //Add a timer
    time_popover.add(calender);
    calender.show();
    calender.set_margin_bottom(5);
    calender.set_margin_end(5);
    calender.set_margin_start(5);
    calender.set_margin_end(5);

    //Timer
    timer_button.set_label("2022/1/23 18:32");
    timer_button.set_relief(Gtk::RELIEF_NONE);
    timer_button.set_popover(time_popover);
    pack_end(timer_button,Gtk::PACK_SHRINK);
}

Glib::RefPtr<Gio::Menu> MyFinder::get_menu(Glib::RefPtr<Gtk::Builder> &builder, const Glib::ustring &id){
    auto object = builder->get_object(id);
    auto gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
    return gmenu;
}
