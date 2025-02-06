#pragma once

#include <gtkmm.h>
#include "MyHeader.hh"
// #include "MainWin.hh"    // can't include this because of circular dependency

class PrefsBtn : public Gtk::Button
{
public:
    PrefsBtn()
    {
        set_has_frame(false);
        set_child(btn_img);
        btn_img.set_pixel_size(160);
    }

    // Set image from file path or resource path
    void set_image_from_resource(Glib::ustring &path)
    {
        // Gtk::Image btn_img;
        btn_img.set_from_resource(path);
    }

    void set_image_from_file(Glib::ustring &path)
    {
        // Use gobj() to get the underlying GtkWidget pointer
        gtk_image_set_from_file(btn_img.gobj(), path.c_str());
    }

    Glib::RefPtr<Gdk::Paintable> get_paintable()
    {
        return btn_img.get_paintable();
    }

    // Set and get image id
    void set_image_id(guint id) { image_id = id; }
    guint get_image_id() { return image_id; }

private:
    Gtk::Image btn_img;
    guint image_id;
};

class MyPrefs : public Gtk::Window
{
public:
    MyPrefs(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);
    static MyPrefs *create(Gtk::Window &parent);
    void set_background_widget(Gtk::Picture *picture);

protected:
    // bool on_close_request();

private:
    Glib::RefPtr<Gtk::Builder> ref_builder;

    // Header bar
    MyHeader header;

    // Child widgets
    Gtk::StackSwitcher *back_switcher;
    Gtk::Stack *back_stack;

    // Widget for appearance settings
    Gtk::Switch *switch_dark;
    Gtk::Frame *back_frame;
    Gtk::Button *btn_add, *btn_remove, *btn_removeall;

    // Store and widget for background images
    Gtk::GridView images_view;
    Gtk::ScrolledWindow images_scroll;
    Glib::RefPtr<Gtk::StringList> images_store;
    Glib::RefPtr<Gtk::SingleSelection> images_selection;
    Glib::RefPtr<Gtk::SignalListItemFactory> images_factory;
    void images_view_setup(const Glib::RefPtr<Gtk::ListItem> &item);
    void images_view_bind(const Glib::RefPtr<Gtk::ListItem> &item);

    // Background image widget
    Gtk::Picture *background_widget;

    // Signal handlers for background preferences
    void image_btn_clicked(PrefsBtn *btn);
    void btnadd_clicked();
    void btnremove_clicked();
    void btnremoveall_clicked();
    void file_dialog_finish(const Glib::RefPtr<Gio::AsyncResult> &result,
                            const Glib::RefPtr<Gtk::FileDialog> &file_dialog);

    // Child widgets for launcher settings
    Gtk::Scale *scale_size;
    void scale_size_changed();

    // Constant value for standard dpi and resuolution settings
    const float dpi_values[7] = {1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5};
    const int width_values[13] = {640, 800, 1024, 1280, 1366, 1440, 1600, 1680, 1792, 1896, 1920, 2048, 2560};
    const int height_values[13] = {360, 450, 576, 720, 768, 810, 900, 945, 1008, 1044, 1080, 1152, 1440};

    // Child widgets for window size settings
    int width = 1280, height = 720;
    Gtk::DropDown dropdown_size;
    Glib::RefPtr<Gtk::StringList> size_store;
    Gtk::Box *combo_box;
    Gtk::CheckButton *radio_default, *radio_custom;
    Gtk::SpinButton *spin_width, *spin_height;
    Glib::RefPtr<Gtk::Adjustment> adj_width, adj_height;
    Gtk::Button *btnGet, *btnapply;

    // Config control
    void config_load();
    void config_save();
};