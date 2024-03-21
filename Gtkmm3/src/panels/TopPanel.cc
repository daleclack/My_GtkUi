#include "TopPanel.hh"
#include <cstdio>
#include <ctime>

TopPanel::TopPanel():
Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,5),
toplabel("My GtkUI Desktop"),
midlabel("  "),
timer_value(1000)
{
    //Add Header
    pack_start(toplabel,Gtk::PACK_SHRINK);
    pack_start(midlabel);

    //Initalize Timer
    mytimer=Glib::signal_timeout().connect(sigc::mem_fun(*this,&TopPanel::on_timeout),timer_value);
    time_btn.set_relief(Gtk::RELIEF_NONE);
    time_btn.set_label("18:13:00 2021/6/14");
    time_btn.set_popover(time_popover);
    time_popover.add(calender);
    calender.set_size_request(300,250);
    calender.show();
    pack_start(time_btn,Gtk::PACK_SHRINK);

    //Initalize menu
    menu_builder=Gtk::Builder::create_from_resource("/GtkUI/appmenu.xml");
    auto object=menu_builder->get_object("app-menu");
    auto gmenu=Glib::RefPtr<Gio::Menu>::cast_dynamic(object);

    //Set Popover Menu
    popover.bind_model(gmenu);
    popover.set_halign(Gtk::ALIGN_END);
    menubtn.set_popover(popover);
    menubtn.set_relief(Gtk::RELIEF_NONE);

    //Other things
    panel_builder=Gtk::Builder::create_from_resource("/GtkUI/toppanel.ui");
    panel_builder->get_widget("volumebtn",volume1);
    panel_builder->get_widget("spin1",spin1);
    panel_builder->get_widget("lockbtn",lockbtn);
    lockbtn->set_relief(Gtk::RELIEF_NONE);
    pack_start(*spin1,Gtk::PACK_SHRINK);
    pack_start(*lockbtn,Gtk::PACK_SHRINK);
    pack_start(*volume1,Gtk::PACK_SHRINK);

    pack_end(menubtn,Gtk::PACK_SHRINK);

    //Add Style for TopPanel
    provider = Gtk::CssProvider::create();
    provider->load_from_resource("/org/gtk/daleclack/style.css");
    auto style = get_style_context();
    style->add_provider(provider,G_MAXUINT);
}

bool TopPanel::on_timeout(){
    //Get Local Time
    time_t t;
    t=time(NULL);
    struct tm *local;
    local=localtime(&t);
    //Show time
    sprintf(time_string,"%02d:%02d:%02d %04d/%02d/%02d",
    local->tm_hour,local->tm_min,local->tm_sec,local->tm_year+1900,local->tm_mon+1,local->tm_mday);
    time_btn.set_label(time_string);
    return true;
}
