#pragma once

#include <gtkmm.h>
#include "MyHeader.hh"

// 4 Draw modes: default(free draw), circle, line, rectangle
enum class DrawMode{
    Default,
    Circle,
    Line,
    Rectangle
};

// Flage for process of drawing
enum class DrawProcess{
    Begin,      // The beginning of draw
    Update,     // The Process(Position update)
    End         // The end of draw
};

class Drawing : public Gtk::Window
{
    // Custom Titlebar
    MyHeader header;
    
    // Drawing Mode
    DrawMode drawing_mode;

    // Child Widgets
    Gtk::DrawingArea draw_area;
    Gtk::ColorDialogButton color_btn, fill_btn;
    Gtk::CheckButton fill_check;
    Gtk::Label main_label, size_label;
    Gtk::Box left_box, main_box, btn_box;
    Gtk::Button btn_clear, btn_save, btn_exit;
    Gtk::ToggleButton btn_free, btn_line, btn_circle, btn_rectangle;
    Gtk::Scale scale;

    // Color Setting
    Gdk::RGBA m_color, fill_color;
    Cairo::RefPtr<Cairo::ImageSurface> surface;

    // Image Save Dialog
    Glib::RefPtr<Gtk::FileDialog> dialog;
    void dialog_save_file(const Glib::RefPtr<Gio::AsyncResult> &result);

    // Draw Brush Settings
    double brush_size;
    double rel_x, rel_y;
    bool begin = true;

    // Gesture to draw
    Glib::RefPtr<Gtk::GestureDrag> drag;
    Glib::RefPtr<Gtk::GestureClick> press;
    Glib::RefPtr<Gtk::Adjustment> size_adj;
    double start_x, start_y;

    // Signal Handlers

    // Part 1: Drawing signal handlers
    void draw_event(const Cairo::RefPtr<Cairo::Context> &context, int width, int height);

    void draw_brush(double x, double y, DrawProcess process = DrawProcess::Update);

    void draw_fill_color(const Cairo::RefPtr<Cairo::Context> &cr);

    // Part 2: Signal handler for gestures
    void button_press(int n_press, double x, double y);

    void drag_begin(double x, double y);

    void drag_progress(double x, double y);

    void drag_end(double x, double y);

    // Part 3: Signal Hanlders for normal buttons on the UI
    void color_set();

    void btnfree_clicked();

    void btnline_clicked();

    void btncircle_clicked();

    void btnrectangle_clicked();

    void btnclear_clicked();

    void btnsave_clicked();

public:
    Drawing();
};
