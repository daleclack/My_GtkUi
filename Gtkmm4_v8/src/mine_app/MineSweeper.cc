#include "MineSweeper.hh"
#include <string>
#include <iostream>

MineSweeper::MineSweeper()
    : main_box(Gtk::Orientation::VERTICAL, 5),
      btn_box(Gtk::Orientation::HORIZONTAL, 5),
      cell(nullptr)
{
    // Initalize Window
    set_title("MineSweeper");
    header.set_ctrl_window(this);
    // header.set_title_bar(*this);
    // set_titlebar(header);
    // header.set_show_title_buttons();
    // header.set_decoration_layout("close,minimize,maximize:menu");
    header.pack_end(menu_btn);
    set_icon_name("org.gtk.daleclack");

    // Initalize Menu
    menu_builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/mine_menu.xml");
    auto object = menu_builder->get_object<Gio::MenuModel>("mine_menu");
    popover1.set_menu_model(object);
    // popover1.show();
    menu_btn.set_popover(popover1);

    // Add Actions
    add_action("new_game", sigc::mem_fun(*this, &MineSweeper::new_game));
    add_action("scores", sigc::mem_fun(*this, &MineSweeper::show_scores));
    add_action("show_mines", sigc::mem_fun(*this, &MineSweeper::show_mines));
    add_action("quit", sigc::mem_fun(*this, &MineSweeper::hide));

    // Create mine cells
    cell = new MineCell[49];
    for (int i = 0; i < 7; ++i){
        for (int j = 0; j < 7; ++j)
        {
            mine_grid.attach(cell[i * 7 + j], i, j);
        }
    }

    // Default setting
    reset_game();
    pause_game();

    // Buttons
    btnstart.set_label("Start/Reset");
    btnpause.set_label("Pause");
    btnshow.set_label("Show All");
    btnexit.set_label("Exit");
    btn_box.append(btnstart);
    btn_box.append(btnpause);
    btn_box.append(btnshow);
    btn_box.append(btnexit);
    btnstart.signal_clicked().connect(sigc::mem_fun(*this, &MineSweeper::new_game));
    btnpause.signal_clicked().connect(sigc::mem_fun(*this, &MineSweeper::pause_game));
    btnshow.signal_clicked().connect(sigc::mem_fun(*this, &MineSweeper::show_mines));
    btnexit.signal_clicked().connect(sigc::mem_fun(*this, &MineSweeper::hide));

    // Pack widgets
    status_label.set_halign(Gtk::Align::CENTER);
    btn_box.set_halign(Gtk::Align::CENTER);
    mine_grid.set_halign(Gtk::Align::CENTER);
    main_box.append(status_label);
    main_box.append(mine_grid);
    main_box.append(btn_box);

    // Create a dialog
    input_dialog = InputBox::create();

    // Create Scores Window
    scores_win = ScoresWin::create();

    // Bind windows
    input_dialog->set_transient_for(*this);
    scores_win->set_transient_for(*this);
    input_dialog->set_scores_window(scores_win);

    // Add style for the mine grid
    auto style_provider = Gtk::CssProvider::create();
    style_provider->load_from_resource("/org/gtk/daleclack/mine_app.css");
    main_box.add_css_class("mine_app");
    Gtk::StyleProvider::add_provider_for_display(main_box.get_display(),
                                                 style_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Show everything
    set_child(main_box);
    // show_all_children();
}

MineSweeper::~MineSweeper()
{
    // Delete all resources
    delete input_dialog;
    delete scores_win;
    if (cell != nullptr)
    {
        delete[] cell;
    }
}

void MineSweeper::new_game()
{
    // New game = reset game
    reset_game();
}

void MineSweeper::reset_game(int width, int height, int mines)
{
    // // Clear the cells
    // if (cell != nullptr)
    // {
    //     delete[] cell;
    // }

    // Reset timer
    mytimer.disconnect();
    timer_count = 0;
    mytimer = Glib::signal_timeout().connect(sigc::mem_fun(*this, &MineSweeper::timer_func), 1000);

    mine_count = 0;
    mines_clear = 0;
    mine_grid.set_sensitive();
    // Reset all data
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            // Initalize cell
            cell[i * 7 + j].set_has_frame();
            cell[i * 7 + j].set_image_from_icon_name("", Gtk::IconSize::LARGE);
            // cell[i * 7 + j].set_always_show_image();
            cell[i * 7 + j].set_size_request(40, 40);
            cell[i * 7 + j].mines_around = 0;
            cell[i * 7 + j].has_mine = false;
            cell[i * 7 + j].cleared = false;
        }
    }

    // Reset mines
    while (mine_count < mines)
    {
        int index = g_random_int_range(0, width * height);
        if (!(cell[index].has_mine))
        {
            cell[index].has_mine = true;
            // cell[index].set_label("x");
            mine_count++;
        }
    }
    game_status = GameStatus::Running;
    status_label.set_label(" ");
    calc_mines();

    // Append buttons to grid
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // cell[i * 7 + j].set_label("?");
            cell[i * width + j].signal_clicked().connect(sigc::bind(
                sigc::mem_fun(*this, &MineSweeper::cell_clicked), &cell[i * width + j]));
            cell[i * width + j].set_has_frame();
            cell[i * width + j].x = j;
            cell[i * width + j].y = i;
            cell[i * width + j].cleared = false;
        }
    }

    mine_grid.show();
}

// Pause or Resume Game
void MineSweeper::pause_game()
{
    if (game_status == GameStatus::Running)
    {
        game_status = GameStatus::Paused;
        mine_grid.set_sensitive(false);
    }
    else
    {
        game_status = GameStatus::Running;
        mine_grid.set_sensitive();
    }
}

void MineSweeper::calc_mines()
{
    // Calculate the mines around a cell
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            int index1, index2;
            // The Search cell should not over the grids
            for (index1 = MAX(0, i - 1); index1 < MIN(i + 1, 6) + 1; index1++)
            {
                for (index2 = MAX(0, j - 1); index2 < MIN(j + 1, 6) + 1; index2++)
                {
                    if ((cell[index1 * 7 + index2].has_mine))
                    {
                        cell[i * 7 + j].mines_around++;
                    }
                }
            }
        }
    }
}

void MineSweeper::show_mines()
{
    // Show all cell with a mine
    for (int i = 0; i < 49; i++)
    {
        if (cell[i].has_mine)
        {
            cell[i].set_image_from_icon_name("mine", Gtk::IconSize::LARGE);
        }
    }
}

void MineSweeper::show_scores()
{
    // Show Scores Window
    input_dialog->read_scores();
}

void MineSweeper::game_lost(int explode_index)
{
    // When a cell with mine is clicked, show other mines
    for (int i = 0; i < 49; i++)
    {
        if (cell[i].has_mine && i != explode_index)
        {
            cell[i].set_image_from_icon_name("mine", Gtk::IconSize::LARGE);
        }
    }
}

bool MineSweeper::timer_func()
{
    if (game_status == GameStatus::Running)
    {
        // Set timer
        char tmp[50];
        timer_count++;
        sprintf(tmp, "Time:%d", timer_count);
        status_label.set_label(tmp);
    }
    return true;
}

void MineSweeper::cell_clicked(MineCell *cell1)
{
    cell1->set_has_frame(false);
    if (game_status == GameStatus::Running && !cell1->cleared)
    {
        //
        // If get mine, the game will end now
        if (cell1->has_mine)
        {
            // Set game to stop
            // winned = false;
            game_status = GameStatus::Ended;
            cell1->cleared = true;
            cell1->set_image_from_icon_name("exploded", Gtk::IconSize::LARGE);

            // End the game
            game_lost(cell1->y * 7 + cell1->x);
            status_label.set_label("You lost!");
            // game_ended = true;
            mytimer.disconnect();
            mine_grid.set_sensitive(false);
        }
        else
        {
            // If no mines, check the cell around
            check_mines(cell1->x, cell1->y);
        }
    }
}

void MineSweeper::check_mines(int pos_x, int pos_y)
{
    if (pos_x >= 0 && pos_x <= 6 &&
        pos_y >= 0 && pos_y <= 6)
    {
        if (!cell[pos_y * 7 + pos_x].has_mine &&
            !cell[pos_y * 7 + pos_x].cleared)
        {
            mines_clear++;
            // Show the cell has no mines around
            if (cell[pos_y * 7 + pos_x].mines_around == 0)
            {
                cell[pos_y * 7 + pos_x].set_image_from_icon_name("", Gtk::IconSize::LARGE);
            }
            else
            {
                // Show the numbers of mines around a cell
                char *label = g_strdup_printf("%dmines", cell[pos_y * 7 + pos_x].mines_around);
                cell[pos_y * 7 + pos_x].set_image_from_icon_name(label, Gtk::IconSize::LARGE);
                g_free(label);
            }

            // make the cell without mines cleared
            cell[pos_y * 7 + pos_x].set_has_frame(false);
            cell[pos_y * 7 + pos_x].cleared = true;

            // Check the cells around a cell that has no mines
            if (cell[pos_y * 7 + pos_x].mines_around == 0)
            {
                check_mines((pos_x - 1), (pos_y - 1));
                check_mines((pos_x + 1), (pos_y + 1));
                check_mines((pos_x - 1), (pos_y + 1));
                check_mines((pos_x + 1), (pos_y - 1));
                check_mines(pos_x, (pos_y - 1));
                check_mines(pos_x, (pos_y + 1));
                check_mines((pos_x + 1), pos_y);
                check_mines((pos_x - 1), pos_y);
            }
        }
    }

    // If all the mines has cleared, you has winned
    if (mines_clear == 40)
    {
        // Stop the game
        status_label.set_label("You winned!");
        game_status = GameStatus::Winned;
        mytimer.disconnect();

        // Save the time of game
        input_dialog->set_game_time(timer_count);
        input_dialog->show();

        mine_grid.set_sensitive(false);
    }
}
