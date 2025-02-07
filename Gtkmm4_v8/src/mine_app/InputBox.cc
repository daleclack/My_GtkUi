#include "InputBox.hh"
#include <iostream>

InputBox::InputBox(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_Glade)
    : Gtk::Window(cobject),
      ref_builder(ref_Glade)
{
    // Get Widgets
    // entry_name = ref_builder->get_widget<Gtk::Entry>("entry_name");
    input_box = ref_builder->get_widget<Gtk::Box>("input_box");
    check_scores = ref_builder->get_widget<Gtk::CheckButton>("check_scores");
    btn_ok = ref_builder->get_widget<Gtk::Button>("btn_ok");
    btn_cancel = ref_builder->get_widget<Gtk::Button>("btn_cancel");

    input_box->prepend(entry_name);
    entry_name.set_text("111");

    // Connect signals
    entry_name.signal_activate().connect(sigc::mem_fun(*this, &InputBox::entry_activated));
    btn_ok->signal_clicked().connect(sigc::mem_fun(*this, &InputBox::btnok_clicked));
    btn_cancel->signal_clicked().connect(sigc::mem_fun(*this, &InputBox::btncancel_clicked));
}

bool InputBox::on_close_request()
{
    set_visible(false);
    return true;
}

void InputBox::btnok_clicked()
{
    // Create toml object for data
    static constexpr std::string_view source = R"(
        names = []
        times = []
    )";

    toml::table tbl = toml::parse(source);

    // Append data to names and paths
    std::string name = std::string(entry_name.get_text());
    names.push_back(name);
    times.push_back(game_time);
    tbl.insert_or_assign("names", names);
    tbl.insert_or_assign("times", times);

    // Save toml data
    std::fstream outfile;
    outfile.open("scores.toml", std::ios_base::out);
    if (outfile.is_open())
    {
        outfile << tbl;
    }
    outfile.close();

    // If show scores checkbutton is checked, show scores window
    read_scores(check_scores->get_active());

    // Close the window
    close();
}

void InputBox::btncancel_clicked()
{
    // Close the window
    close();
}

void InputBox::read_scores(bool show_scores_win)
{
    // If show scores checkbutton is checked, show scores window
    if (show_scores_win)
    {
        scores_win1->update_and_show();
    }
}

void InputBox::set_game_time(int time)
{
    // Try to load scores from config file
    std::fstream scores_file;
    scores_file.open("scores.toml", std::ios_base::in);

    // If toml file opened, read the data
    if (scores_file.is_open())
    {
        auto toml = toml::parse(scores_file);
        names = *(toml.get_as<toml::array>("names"));
        times = *(toml.get_as<toml::array>("times"));
    }

    // Initalize time
    game_time = time;
}

void InputBox::entry_activated()
{
    // Default response
    btnok_clicked();
}

void InputBox::set_scores_window(ScoresWin *win1)
{
    // Bind Scores Window
    scores_win1 = win1;
}

InputBox *InputBox::create()
{
    // Create a inputbox object
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/cambalache/win_input.ui");

    InputBox *dialog;
    dialog = Gtk::Builder::get_widget_derived<InputBox>(builder, "dialog");

    return dialog;
}
