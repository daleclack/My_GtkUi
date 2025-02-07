#pragma once

#include <gtkmm.h>
#include "../tomlplusplus/toml.hpp"
// #include "jsonfile.hh"

class ScoresItem : public Glib::Object
{
public:
    ScoresItem(const std::string &player_name1, int win_time1)
    {
        player_name = player_name1;
        win_time = win_time1;
    }

    // Create function
    static Glib::RefPtr<ScoresItem> create(const std::string &player_name1, int win_time1)
    {
        return Glib::make_refptr_for_instance<ScoresItem>(new ScoresItem(player_name1, win_time1));
    }

    std::string player_name;
    int win_time;
};

class ScoresWin : public Gtk::Window
{
public:
    static ScoresWin *create();
    ScoresWin(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_Glade);
    void update_and_show();

protected:
    bool on_close_request() override;

private:
    Glib::RefPtr<Gtk::Builder> ref_builder;

    // Child widgets
    Gtk::Button *btnclose;
    Gtk::ColumnView *mine_view;

    // Store for win time and player name
    Glib::RefPtr<Gio::ListStore<ScoresItem>> scores_store;
    Glib::RefPtr<Gtk::NoSelection> no_selection;

    // Column for Names
    Glib::RefPtr<Gtk::SignalListItemFactory> name_factory;
    Gtk::ColumnViewColumn *name_column;
    void name_bind(const Glib::RefPtr<Gtk::ListItem> &item);
    void name_setup(const Glib::RefPtr<Gtk::ListItem> &item);

    // Column for Times
    Glib::RefPtr<Gtk::SignalListItemFactory> time_factory;
    Gtk::ColumnViewColumn *time_column;
    void time_bind(const Glib::RefPtr<Gtk::ListItem> &item);
    void time_setup(const Glib::RefPtr<Gtk::ListItem> &item);

    // Sort function
    // int sort_func(const Gtk::TreeModel::const_iterator &iter1, const Gtk::TreeModel::const_iterator &iter2);
};
