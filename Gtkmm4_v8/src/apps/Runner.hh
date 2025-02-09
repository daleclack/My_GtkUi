#pragma once

#include <gtkmm.h>

class Runner : public Gtk::Window
{
public:
    Runner();

protected:
    bool on_close_request() override
    {
        set_visible(false);
        return true;
    }

private:
    // Header bar
    Gtk::HeaderBar header;

    // Child widgets
    Gtk::Button btn_ok, btn_cancel, btn_path;
    Gtk::Entry entry_path;
    Gtk::Box main_box;

    // Signal handlers
    void btnok_clicked();
    void btnpath_clicked();
    void opendialog_finish(Glib::RefPtr<Gio::AsyncResult> &result,
                           Glib::RefPtr<Gtk::FileDialog> &dialog);
};