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
    panel_builder->get_widget("btnrun",btnrun);
    panel_builder->get_widget("btneditor",btneditor);
    panel_builder->get_widget("panel_editor",panel_editor);
    
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
    btnrun->signal_clicked().connect(sigc::mem_fun(*this,&LeftPanel::btnrun_clicked));
    btneditor->signal_clicked().connect(sigc::mem_fun(*this,&LeftPanel::btnedit_clicked));
    panel_editor->signal_clicked().connect(sigc::mem_fun(*this,&LeftPanel::btnedit_clicked));
}

void LeftPanel::add_panel(Gtk::Window *parent1,Gtk::Overlay &overlay){
    //Pack the panel
    btnbox->set_halign(Gtk::ALIGN_START);
    btnbox->set_valign(Gtk::ALIGN_START);
    overlay.add_overlay(*btnbox); 
    //Set Parent Window
    parent=parent1;
    runner1.set_transient_for(*parent1);
    editor1.set_transient_for(*parent1);
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
    //Simulate the panel by Gdk::Window Proprties
    auto game_win=game1.get_window();
    if(game_win){
        auto state=game_win->get_state();
        switch(state){
            case Gdk::WINDOW_STATE_ICONIFIED:
                game1.deiconify();
                break;
            case Gdk::WINDOW_STATE_WITHDRAWN:
                game1.show_game_window(*parent);
                break;
            default:
                game1.iconify();
        }
    }else{
        game1.show_game_window(*parent);
        game1.running=true;
    }
    popover->popdown();
}

bool LeftPanel::on_timeout(){
    //When Game running and closed,use different icons
    if(game1.running){
        panelgame->set_image_from_icon_name("game_running",Gtk::ICON_SIZE_DIALOG);
    }else{
        panelgame->set_image_from_icon_name("game",Gtk::ICON_SIZE_DIALOG);
    }
    if(editor1.running){
        panel_editor->set_image_from_icon_name("gedit_running",Gtk::ICON_SIZE_DIALOG);
    }else{
        panel_editor->set_image_from_icon_name("gedit",Gtk::ICON_SIZE_DIALOG);
    }
    return true;
}

void LeftPanel::btnrun_clicked(){
    runner1.show_all();
    popover->popdown();
}

void LeftPanel::btnedit_clicked(){
    //Simulate the panel by Gdk::Window Proprties
    auto editor_win=editor1.get_window();
    if(editor_win){
        auto state=editor_win->get_state();
        switch(state){
            case Gdk::WINDOW_STATE_ICONIFIED:
                editor1.deiconify();
                break;
            case Gdk::WINDOW_STATE_WITHDRAWN:
                editor1.show();
                editor1.running=true;
                break;
            default:
                editor1.iconify();
        }
    }else{
        editor1.show();
        editor1.running=true;
    }
    popover->popdown();
}
