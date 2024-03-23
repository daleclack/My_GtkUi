#include "Game24Win.hh"

Game24Win::Game24Win(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_Glade)
    : Gtk::Window(cobject),
      ref_builder(ref_Glade),
      winned(true)
{
    set_icon_name("24game");
    title_bar.set_ctrl_win(this);
    title_bar.set_title("24 Game");

    // Get Widgets
    ref_builder->get_widget("label_numbers", label_numbers);
    ref_builder->get_widget("entry_ans", entry_ans);
    ref_builder->get_widget("btnstart", btnstart);
    ref_builder->get_widget("btnnext",btnnext);
    ref_builder->get_widget("btncheck", btncheck);
    ref_builder->get_widget("btnclear",btnclear);
    ref_builder->get_widget("btnexit", btnexit);
    ref_builder->get_widget("btn0", btns[0]);
    ref_builder->get_widget("btn1", btns[1]);
    ref_builder->get_widget("btn2", btns[2]);
    ref_builder->get_widget("btn3", btns[3]);
    ref_builder->get_widget("btn4", btns[4]);
    ref_builder->get_widget("btn5", btns[5]);
    ref_builder->get_widget("btn6", btns[6]);
    ref_builder->get_widget("btn7", btns[7]);
    ref_builder->get_widget("btn8", btns[8]);
    ref_builder->get_widget("btn9", btns[9]);
    ref_builder->get_widget("btnadd", btns[10]);
    ref_builder->get_widget("btnsubb", btns[11]);
    ref_builder->get_widget("btnmul", btns[12]);
    ref_builder->get_widget("btndiv", btns[13]);
    ref_builder->get_widget("btnleft", btns[14]);
    ref_builder->get_widget("btnright", btns[15]);

    // Link Signals
    btnstart->signal_clicked().connect(sigc::mem_fun(*this, &Game24Win::btnstart_clicked));
    btnexit->signal_clicked().connect(sigc::mem_fun(*this, &Game24Win::hide));
    btnnext->signal_clicked().connect(sigc::mem_fun(*this, &Game24Win::btnnext_clicked));
    btncheck->signal_clicked().connect(sigc::mem_fun(*this, &Game24Win::btncheck_clicked));
    btnclear->signal_clicked().connect(sigc::mem_fun(*this,&Game24Win::btnclear_clicked));
    for (int i = 0; i < 16; i++)
    {
        btns[i]->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &Game24Win::btns_clicked), btns[i]));
    }
    entry_ans->signal_grab_focus().connect(sigc::mem_fun(*this, &Game24Win::entry_ans_focus));
}

void Game24Win::btns_clicked(Gtk::Button *button)
{
    // Get Original text and add number from button

    // 1.Get text buffer for insert
    auto buffer = entry_ans->get_buffer();

    // 2.Get text to insert
    Glib::ustring label = button->get_label();

    if (focused)
    {
        pos = entry_ans->get_position();
    }

    // 3.Get Position
    buffer->insert_text(pos, label);
    pos++;
    focused = false;
}

void Game24Win::entry_ans_focus()
{
    focused = true;
}

void Game24Win::btnstart_clicked()
{
    // Start Game
    if (winned)
    {
        main_game.getnumbers();
        label_numbers->set_label(main_game.numbers_char);
    }
    else
    {
        label_numbers->set_label(tmp);
    }

    // Save the numbers
    tmp = label_numbers->get_label();
}

void Game24Win::btncheck_clicked()
{
    // Check Answer
    Glib::ustring answer = entry_ans->get_text();
    winned = main_game.startgame(std::string(answer.c_str()));
    if (winned)
    {
        label_numbers->set_label("You Winned!");
    }
    else
    {
        label_numbers->set_label("You Lost!");
    }
}

void Game24Win::btnnext_clicked(){
    main_game.getnumbers();
    label_numbers->set_label(main_game.numbers_char);
}

void Game24Win::btnclear_clicked(){
    // Clear the text
    entry_ans->set_text("");
}

Game24Win *Game24Win::create()
{
    // Create a new window
    Game24Win *window = nullptr;
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/game24.ui");
    builder->get_widget_derived("window", window);

    return window;
}