#include "ScoresWin.hh"

ScoresWin::ScoresWin(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_Glade)
    : Glib::ObjectBase("ScoresWin"),
      Gtk::Window(cobject),
      ref_builder(ref_Glade)
{
    // Get Widgets
    btnclose = ref_builder->get_widget<Gtk::Button>("btnclose");
    mine_view = ref_builder->get_widget<Gtk::ColumnView>("mine_view");

    // Create the store and the columns
    scores_store = Gio::ListStore<ScoresItem>::create();
    no_selection = Gtk::NoSelection::create(scores_store);
    mine_view->set_model(no_selection);

    // Add the columns to the view

    // Column for Names
    name_factory = Gtk::SignalListItemFactory::create();
    name_factory->signal_setup().connect(sigc::mem_fun(*this, &ScoresWin::name_setup));
    name_factory->signal_bind().connect(sigc::mem_fun(*this, &ScoresWin::name_bind));
    auto column_name = Gtk::ColumnViewColumn::create("Player Name", name_factory);
    mine_view->append_column(column_name);

    // Column for Win Time
    time_factory = Gtk::SignalListItemFactory::create();
    time_factory->signal_setup().connect(sigc::mem_fun(*this, &ScoresWin::time_setup));
    time_factory->signal_bind().connect(sigc::mem_fun(*this, &ScoresWin::time_bind));
    auto column_time = Gtk::ColumnViewColumn::create("Win Time (s)", time_factory);
    mine_view->append_column(column_time);

    // Connect Signals
    btnclose->signal_clicked().connect([this]
                                       { close(); });

    update_and_show();
}

bool ScoresWin::on_close_request()
{
    set_visible(false);
    return true;
}

void ScoresWin::update_and_show()
{
    // Open the file to read data from it
    std::fstream infile;
    infile.open("scores.toml", std::ios_base::in);

    if (infile.is_open())
    {
        // Read data from toml file
        auto toml = toml::parse(infile);
        auto names = *(toml["names"].as_array());
        auto times = *(toml["times"].as_array());

        // Clear the store
        scores_store->remove_all();

        // Append data to the store
        for (int i = 0; i < names.size(); i++)
        {
            std::string name = names[i].as_string()->get();
            int time = times[i].as_integer()->get();
            scores_store->append(ScoresItem::create(name, time));
        }
    }
    infile.close();

    // present();
}

void ScoresWin::name_bind(const Glib::RefPtr<Gtk::ListItem> &item)
{
    // Get the item's child and cast it to a label
    auto label = dynamic_cast<Gtk::Label *>(item->get_child());
    if (label)
    {
        // Get the item's data and set it to the label
        guint pos = item->get_position();
        auto data = scores_store->get_item(pos);
        label->set_text(data->player_name);
    }
}

void ScoresWin::name_setup(const Glib::RefPtr<Gtk::ListItem> &item)
{
    // Add a label to the item
    auto label = Gtk::make_managed<Gtk::Label>();
    item->set_child(*label);
}

void ScoresWin::time_bind(const Glib::RefPtr<Gtk::ListItem> &item)
{
    char time[10];
    // Get the item's child and cast it to a label
    auto label = dynamic_cast<Gtk::Label *>(item->get_child());
    if (label)
    {
        // Get the item's data and set it to the label
        guint pos = item->get_position();
        auto data = scores_store->get_item(pos);
        snprintf(time, sizeof(time), "%d", data->win_time);
        label->set_text(time);
    }
}

void ScoresWin::time_setup(const Glib::RefPtr<Gtk::ListItem> &item)
{
    // Add a label to the item
    auto label = Gtk::make_managed<Gtk::Label>();
    item->set_child(*label);
}

// int ScoresWin::sort_func(const Gtk::TreeModel::const_iterator &iter1, const Gtk::TreeModel::const_iterator &iter2)
// {
//     // Sort by the game time
//     auto row1 = *iter1;
//     auto row2 = *iter2;
//     if (row1[column1.win_time] < row2[column1.win_time])
//     {
//         // g_print("test1\n");
//         return -1;
//     }
//     if (row1[column1.win_time] == row2[column1.win_time])
//     {
//         // g_print("test2\n");
//         return 0;
//     }
//     else
//     {
//         return 1;
//     }
// }

ScoresWin *ScoresWin::create()
{
    // Create a window
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/cambalache/scoreswin.ui");
    ScoresWin *main_win;

    // builder->get_widget_derived("scores_win", main_win);
    main_win = Gtk::Builder::get_widget_derived<ScoresWin>(builder, "scores_win");

    return main_win;
}
