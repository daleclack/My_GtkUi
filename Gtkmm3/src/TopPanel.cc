#include "TopPanel.hh"

TopPanel::TopPanel():
Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,5)
{
    //Initalize menu
    menu_builder=Gtk::Builder::create_from_file("../res/appmenu.xml");
    auto object=menu_builder->get_object("app-menu");
    auto gmenu=Glib::RefPtr<Gio::Menu>::cast_dynamic(object);

    //Set Popover Menu
    popover.bind_model(gmenu);
    popover.set_halign(Gtk::ALIGN_END);
    menubtn.set_popover(popover);
    menubtn.set_relief(Gtk::RELIEF_NONE);

    pack_end(menubtn,Gtk::PACK_SHRINK);
}
