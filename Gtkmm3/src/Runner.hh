#pragma once

#include <gtkmm.h>

class GtkRunner : public Gtk::Dialog{
public:
    GtkRunner();
protected:
    void on_response(int response_id) override;
private:
    //Child Widgets
    Gtk::Box hbox;
    Gtk::Entry app_entry;
    Gtk::Button open_btn;
    //File Dialog
    Glib::RefPtr<Gtk::FileChooserNative> dialog;
    void file_dialog();
    void dialog_response(int response);
};
