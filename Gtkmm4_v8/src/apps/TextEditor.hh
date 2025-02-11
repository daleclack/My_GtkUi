#pragma once

#include <gtkmm.h>
#include "MyHeader.hh"

class TextEditor : public Gtk::ApplicationWindow
{
public:
    TextEditor();

private:
    // Header widgets
    MyHeader header;
    Gtk::MenuButton menubtn;
    Gtk::PopoverMenu popover;
    Gtk::ToggleButton search_button;
    Glib::RefPtr<Gtk::Builder> menu_builder, expend_builder;

    // SearchBar
    Gtk::SearchBar searchbar;
    Gtk::SearchEntry search_entry;
    Gtk::Box searchbox;
    Gtk::Button search_up, search_down;
    Glib::RefPtr<Glib::Binding> search_binding;
    Gtk::TextIter curr_iter_up, curr_iter_down;

    // Window widgets
    Gtk::Box vbox, hbox, *infobox;
    Gtk::ScrolledWindow sw1;
    Glib::RefPtr<Gtk::TextBuffer> buffer1;
    Gtk::TextView textview1;
    Gtk::Box infobar;
    Gtk::Button info_ok;
    Gtk::Label label1;
    Gtk::Expander *expender;
    Gtk::Button *btns[26], *btntab, *btnenter;
    Gtk::ToggleButton *btnshift, *btncaps;

    // File Dialog
    Glib::RefPtr<Gtk::FileDialog> dialog;
    Glib::ustring curr_filename;
    bool file_opened;

    // Signal Handlers
    // bool close_request();
    // bool window_delete_event(GdkEventAny *event);

    // File Operation functions
    void btnopen_clicked();
    void dialog_open_file(const Glib::RefPtr<Gio::AsyncResult> &result);
    void btnsave_clicked();
    void btnsaveas_clicked();
    void dialog_save_file(const Glib::RefPtr<Gio::AsyncResult> &result);

    // Copy, Paste and text operations
    void btncopy_clicked();
    void btnpaste_clicked();
    void btnclose_clicked();
    void buffer1_changed();
    void clipboard_receive(const Glib::RefPtr<Gio::AsyncResult> &result,
                           const Glib::RefPtr<Gdk::Clipboard> &clipboard);
    void infobar_response();

    // Search funtion
    void search_entry_changed();
    void search_forward();
    void search_backward();

    // Keyboard press
    void key_pressed(Gtk::Button *button);
    void btntab_clicked();
    void btnenter_clicked();

    // Other Signal Handlers
    void about_activated();
};
