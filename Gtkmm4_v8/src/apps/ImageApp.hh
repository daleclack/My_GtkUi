#pragma once

#include <gtkmm.h>
#include "MyImage.hh"
#include "MyHeader.hh"

class ImageApp : public Gtk::ApplicationWindow
{
public:
    ImageApp();
    
private:
    // Custom titlebar
    MyHeader header;
    
    // Child widgets
    Gtk::ScrolledWindow sw;
    MyImage image_area;
    Gtk::Box main_box, btnbox;
    Gtk::Button btnopen;
    Gtk::Scale scale;
    Gtk::Overlay overlay;
    Glib::RefPtr<Gtk::Adjustment> m_adjustment, hadjustment, vadjustment;
    Glib::RefPtr<Gtk::FileDialog> dialog;

    // Gesture control
    Glib::RefPtr<Gtk::GestureDrag> gesture_drag;
    Glib::RefPtr<Gtk::GestureClick> gesture_click;
    void drag_begin(double x, double y);
    void drag_update(double x, double y);
    void drag_end(double x, double y);
    void move_to(double x, double y);

    // Menu for image control
    Gtk::PopoverMenu popover;
    void press(int n_press, double x, double y);

    // Signal Handlers
    void btnopen_clicked();
    void dialog_open_file(const Glib::RefPtr<Gio::AsyncResult>& result);
    void scale_changed();
    void image_zoom_in();
    void image_zoom_out();
    void image_zoom_reset();
};
