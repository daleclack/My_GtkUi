#include "LeftPanel.hh"
#include <thread>
#include <cstdlib>

LeftPanel::LeftPanel(Gtk::Window *parent1){
    //Get Widgets
    panel_builder=Gtk::Builder::create_from_resource("/GtkUI/leftpanel.ui");
    panel_builder->get_widget("popover1",popover);
    panel_builder->get_widget("btnbox",btnbox);
    panel_builder->get_widget("btnaud",btnaud);
    panel_builder->get_widget("btngedit",btngedit);
    panel_builder->get_widget("btnvlc",btnvlc);
    panel_builder->get_widget("btn_note",btnnote);
    panel_builder->get_widget("btn_vlc",btnvlc_win32);
    
    //Link Signals
    btnaud->signal_clicked().connect(sigc::mem_fun(*this,&LeftPanel::btnaud_clicked));
    btngedit->signal_clicked().connect(sigc::mem_fun(*this,&LeftPanel::btngedit_clicked));
    btnvlc->signal_clicked().connect(sigc::mem_fun(*this,&LeftPanel::btnvlc_clicked));
    btnnote->signal_clicked().connect(sigc::mem_fun(*this,&LeftPanel::btnnote_clicked));
    btnvlc_win32->signal_clicked().connect(sigc::mem_fun(*this,&LeftPanel::winvlc_clicked));
    parent=parent1;
}

void LeftPanel::add_panel(Gtk::Overlay &overlay){
    btnbox->set_halign(Gtk::ALIGN_START);
    btnbox->set_valign(Gtk::ALIGN_START);
    overlay.add_overlay(*btnbox);
}

void LeftPanel::btnaud_clicked(){
    std::thread first(system,"audacious");
    first.detach();
    popover->popdown();
}

void LeftPanel::btngedit_clicked(){
    std::thread second(system,"gedit");
    second.detach();
    popover->popdown();
}

void LeftPanel::btnvlc_clicked(){
    std::thread third(system,"vlc");
    third.detach();
}

void LeftPanel::btnnote_clicked(){
    std::thread fourth(system,"start notepad");
    fourth.detach();
    popover->popdown();
}

void LeftPanel::winvlc_clicked(){
    std::thread fifth(system,"start ..\\vlc\\vlc.exe");
    fifth.detach();
    popover->popdown();
}
