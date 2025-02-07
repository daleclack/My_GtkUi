#pragma once

#include <gtkmm.h>
// #include "jsonfile.hh"
#include "ScoresWin.hh"

class InputBox : public Gtk::Window{
    public:
        static InputBox *create();
        InputBox(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_Glade);
        void read_scores(bool show_scores_win = true);
        void set_game_time(int time);
        void set_scores_window(ScoresWin *win1);
    
    protected:
        bool on_close_request() override;
        // void on_response(int response_id) override;
    
    private:
        int game_time;

        // Data to write to json file
        toml::array *names, *times;
        // std::vector<std::string> names;
        // std::vector<int> times;

        // Builder Object
        Glib::RefPtr<Gtk::Builder> ref_builder;

        // Child widget
        Gtk::Entry entry_name;
        Gtk::Box *input_box;
        Gtk::CheckButton *check_scores;
        Gtk::Button *btn_ok, *btn_cancel;

        // Scores Window
        ScoresWin *scores_win1;

        // Signal Handlers
        void entry_activated();
        void btnok_clicked();
        void btncancel_clicked();
};