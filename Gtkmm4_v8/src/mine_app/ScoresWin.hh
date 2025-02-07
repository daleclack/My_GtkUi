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

private:
    Glib::RefPtr<Gtk::Builder> ref_builder;

    // Child widgets
    Gtk::Button *btnclose;
    // Gtk::TreeView *tree_view;

    // // TreeView data
    // class ModelColumns : public Gtk::TreeModelColumnRecord
    // {
    // public:
    //     ModelColumns()
    //     {
    //         add(player_name);
    //         add(win_time);
    //     }
    //     Gtk::TreeModelColumn<std::string> player_name;
    //     Gtk::TreeModelColumn<int> win_time;
    // };
    // ModelColumns column1;

    // Glib::RefPtr<Gtk::ListStore> store;

    // Sort function
    // int sort_func(const Gtk::TreeModel::const_iterator &iter1, const Gtk::TreeModel::const_iterator &iter2);
};
