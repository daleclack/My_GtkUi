#include "Game.hh"
#include <ctime>
#include <cstdlib>
#include <cstdio>

Game::Game(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &tmp_builder)
    : Glib::ObjectBase("Game"),
      Gtk::Window(cobject),
      game_builder(tmp_builder),
      game_index{0, 1, 2, 3}
{
    // Ininalize Window
    set_icon_name("game");
    header.set_ctrl_window(this);

    // Get widgets
    btnexit = game_builder->get_widget<Gtk::Button>("btn_exit");
    btngo = game_builder->get_widget<Gtk::Button>("btn_go");
    game_label = game_builder->get_widget<Gtk::Label>("label");
    for (int i = 0; i < 4; i++)
    {
        char str[6];
        sprintf(str, "btn_%d", i + 1);
        game_btn[i] = game_builder->get_widget<Gtk::Button>(str);
        // game_builder->get_widget(str, game_btn[i]);
        game_btn[i]->signal_clicked().connect(sigc::bind(
            sigc::mem_fun(*this, &Game::gamebtn_clicked), &game_index[i]));
    }
    // Link Signals
    btngo->signal_clicked().connect(sigc::mem_fun(*this, &Game::btngo_clicked));
    btnexit->signal_clicked().connect(sigc::mem_fun(*this, &Game::close));
}

void Game::gamebtn_clicked(int *index)
{
    // Get Current button
    int flag;
    srand((unsigned)time(NULL));
    flag = rand() % 4;
    game_btn[flag]->set_image_from_icon_name("My_GtkUI", Gtk::IconSize::LARGE);
    if (*index == flag)
    {
        game_label->set_label("Good Luck!");
    }
    else
    {
        game_label->set_label("The Button is here!");
    }
    // Disable Buttons
    for (int i = 0; i < 4; i++)
    {
        game_btn[i]->set_sensitive(false);
    }
}

void Game::btngo_clicked()
{
    // Clean Icons showed in button and enable buttons
    char str[6];
    for (int i = 0; i < 4; i++)
    {
        snprintf(str, 6, "%d", i + 1);
        game_btn[i]->set_label(str);
        game_btn[i]->set_sensitive();
    }
    game_label->set_label("Select a button");
}

Game *Game::create()
{
    // Load the Builder file and instantiate its widgets.
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/game1.ui");

    Game *window = nullptr;
    window = Gtk::Builder::get_widget_derived<Game>(builder, "window");
    // builder->get_widget_derived("window", window);

    return window;
}
