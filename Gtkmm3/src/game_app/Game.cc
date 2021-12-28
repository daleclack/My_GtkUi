#include "Game.hh"
#include <ctime>
#include <cstdlib>
#include <cstdio>

Game::Game(BaseObjectType *cobject,Glib::RefPtr<Gtk::Builder> &tmp_builder)
:Gtk::Window(cobject),
game_builder(tmp_builder),
running(false),
game_index{0,1,2,3}
{
    //Ininalize Window
    game_builder->get_widget("window",game_window);
    game_builder->get_widget("btn_exit",btnexit);
    game_builder->get_widget("btn_go",btngo);
    game_builder->get_widget("label",game_label);
    for(int i=0;i<4;i++){
        char str[6];
        sprintf(str,"btn_%d",i+1);
        game_builder->get_widget(str,game_btn[i]);
        game_btn[i]->signal_clicked().connect(sigc::bind(
            sigc::mem_fun(*this,&Game::gamebtn_clicked),&game_index[i]));
    }
    //Link Signals
    btngo->signal_clicked().connect(sigc::mem_fun(*this,&Game::btngo_clicked));
    btnexit->signal_clicked().connect(sigc::mem_fun(*this,&Game::win_closed));
    signal_hide().connect(sigc::mem_fun(*this,&Game::win_closed));
}

void Game::gamebtn_clicked(int *index){
    //Get Current button
    int flag;
    srand((unsigned)time(NULL));
    flag=rand()%4;
    game_btn[flag]->set_image_from_icon_name("My_GtkUI",Gtk::ICON_SIZE_DIALOG);
    if(*index==flag){
        game_label->set_label("Good Luck!");
    }else{
        game_label->set_label("The Button is here!");
    }
    //Disable Buttons
    for(int i=0;i<4;i++){
        game_btn[i]->set_sensitive(false);
    }
}

void Game::btngo_clicked(){
    //Clean Icons showed in button and enable buttons
     for(int i=0;i<4;i++){
        game_btn[i]->set_image_from_icon_name("");
        game_btn[i]->set_sensitive();
    }
    game_label->set_label("Select a button");
}

void Game::win_closed(){
    running=false;
}

Game * Game::create(){
    // Load the Builder file and instantiate its widgets.
    auto builder = Gtk::Builder::create_from_resource("/GtkUI/game1.ui");

    Game * window = nullptr;
    builder->get_widget_derived("window",window);

    return window;
}
