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

    // Signal handlers
    void image_btn_clicked(PrefsBtn *btn);
};