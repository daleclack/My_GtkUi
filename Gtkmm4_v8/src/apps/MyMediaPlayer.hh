#pragma once

#include <gtkmm.h>
#include "LyricsParser.hh"
#include "../tomlplusplus/toml.hpp"

// Play mode
enum class PlayMode
{
    List_Once,          // Play the media with a playlist once
    List_Repeat,        // Play the media with a playlist repeatly
    List_Shuffle,       // Random play a media in the playlist
    One_Repeat          // Repeat a media file
};

// Item class for the list view
class MyItem : public Glib::Object
{
public:
    MyItem(const Glib::ustring &file_name, const Glib::ustring &file_path)
    {
        m_name = file_name;
        m_path = file_path;
    }
    static Glib::RefPtr<MyItem> create(const Glib::ustring &file_name, const Glib::ustring &file_path)
    {
        return Glib::make_refptr_for_instance<MyItem>(new MyItem(file_name, file_path));
    }
    Glib::ustring get_name() { return m_name; }
    Glib::ustring get_path() { return m_path; }

private:
    Glib::ustring m_name;
    Glib::ustring m_path;
};

// Main window class
class MyMediaPlayer : public Gtk::Window
{
public:
    MyMediaPlayer();
    virtual ~MyMediaPlayer();

private:
    // Media Controls area
    Gtk::Video video;
    Gtk::Box main_box, ctrl_box;
    Gtk::Button btnplay, btnprev, btnnext, btnstop, btnmode;
    Gtk::ScrolledWindow lyrics_area;
    Gtk::Label label_lyrics;
    Gtk::ColorDialogButton btncolor;
    Glib::RefPtr<Gtk::ColorDialog> color_dialog;

    // The Playlist area
    Gtk::Expander expander_list;
    Gtk::Box list_box, ctrl_box2;
    Gtk::ListView list_view;
    Gtk::ScrolledWindow list_scroller;
    Gtk::Button btn_add, btn_remove, btn_clear, btn_load, btn_save;
    Glib::RefPtr<Gio::ListStore<MyItem>> media_list;
    Glib::RefPtr<Gtk::SingleSelection> media_selection;
    Glib::RefPtr<Gtk::SignalListItemFactory> media_factory;
    guint current_index, n_media;

    // Functions for the list view
    void setup_view(const Glib::RefPtr<Gtk::ListItem>& list_item);
    void bind_view(const Glib::RefPtr<Gtk::ListItem>& list_item);
    void item_activated(guint pos);

    // Media Play handler and timer
    sigc::connection timer;
    PlayMode current_mode;
    bool timeout_func();

    // Lyrics handler
    LyricsParser lyrics_parser;
    void update_lyrics_label(const Glib::ustring &lyrics_string);
    void update_audio(guint index);

    // The Playlist controls
    void save_playlist(const std::string &filename);
    void load_playlist(const std::string &filename);

    // Signal Handlers
    void btnplay_clicked();
    void btnprev_clicked();
    void btnnext_clicked();
    void btnstop_clicked();
    void btnmode_clicked();
    void btnadd_clicked();
    void btnremove_clicked();
    void btnclear_clicked();
    void btnload_clicked();
    void btnsave_clicked();
    void openfile_dialog_finish(Glib::RefPtr<Gio::AsyncResult> &result, Glib::RefPtr<Gtk::FileDialog> &dialog);
    void savelist_dialog_finish(Glib::RefPtr<Gio::AsyncResult> &result, Glib::RefPtr<Gtk::FileDialog> &dialog);
    void openlist_dialog_finish(Glib::RefPtr<Gio::AsyncResult> &result, Glib::RefPtr<Gtk::FileDialog> &dialog);
};
