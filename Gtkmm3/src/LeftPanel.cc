#include "LeftPanel.hh"
#include <thread>
#include <cstdlib>

LeftPanel::LeftPanel(){
    //Get Widgets
    panel_builder=Gtk::Builder::create_from_resource("/GtkUI/leftpanel.ui");
    panel_builder->get_widget("popover1",popover);
    panel_builder->get_widget("btnbox",btnbox);
    panel_builder->get_widget("btnaud",btnaud);
    panel_builder->get_widget("btngedit",btngedit);
    panel_builder->get_widget("btnvlc",btnvlc);
    panel_builder->get_widget("btn_note",btnnote);
    panel_builder->get_widget("btn_vlc",btnvlc_win32);
    panel_builder->get_widget("btngame",btngame);
    panel_builder->get_widget("panel_game",panelgame);
    
    //Add timer
    paneltimer=Glib::signal_timeout().connect(sigc::mem_fun(*this,&LeftPanel::on_timeout),100);

    //Link Signals
    btnaud->signal_clicked().connect(sigc::mem_fun(*this,&LeftPanel::btnaud_clicked));
    btngedit->signal_clicked().connect(sigc::mem_fun(*this,&LeftPanel::btngedit_clicked));
    btnvlc->signal_clicked().connect(sigc::mem_fun(*this,&LeftPanel::btnvlc_clicked));
    btnnote->signal_clicked().connect(sigc::mem_fun(*this,&LeftPanel::btnnote_clicked));
    btnvlc_win32->signal_clicked().connect(sigc::mem_fun(*this,&LeftPanel::winvlc_clicked));
    btngame->signal_clicked().connect(sigc::mem_fun(*this,&LeftPanel::btngame_clicked));
    panelgame->signal_clicked().connect(sigc::mem_fun(*this,&LeftPanel::btngame_clicked));
}

void LeftPanel::add_panel(Gtk::Window *parent1,Gtk::Overlay &overlay){
    //Pack the panel
    btnbox->set_halign(Gtk::ALIGN_START);
    btnbox->set_valign(Gtk::ALIGN_START);
    overlay.add_overlay(*btnbox); 
    //Set Parent Window
    parent=parent1;
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
    popover->popdown();
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

void LeftPanel::btngame_clicked(){
    if(game1.minimized==true||game1.running==false){
        game1.show_game_window(*parent);
        game1.minimized=false;
    }else{
        game1.hide_game_window();
        game1.minimized=true;
    }
}

bool LeftPanel::on_timeout(){
    //When Game running and closed,use different icons
    if(game1.running){
        panelgame->set_image_from_icon_name("game_running",Gtk::ICON_SIZE_DIALOG);
    }else{
        panelgame->set_image_from_icon_name("game",Gtk::ICON_SIZE_DIALOG);
    }
    return true;
}
