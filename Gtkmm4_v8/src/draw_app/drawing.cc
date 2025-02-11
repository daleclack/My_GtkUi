#include "drawing.hh"
#include <algorithm>
#include <iostream>

Drawing::Drawing()
    : fill_check("Enable Fill Color"),
      main_label("Select a color"),
      size_label("Pen Size"),
      left_box(Gtk::Orientation::VERTICAL, 5),
      main_box(Gtk::Orientation::HORIZONTAL, 5),
      btn_box(Gtk::Orientation::VERTICAL, 5),
      btn_clear("Clear Board"),
      btn_save("Save to png"),
      btn_exit("Exit")
{
    // Ininalize window
    set_icon_name("org.gtk.daleclack");
    set_title("Drawing");
    set_default_size(640, 480);

    // Set the buttons grouped and set normal draw mode
    btn_circle.set_group(btn_free);
    btn_line.set_group(btn_free);
    btn_rectangle.set_group(btn_free);
    btn_free.set_active(true);
    // auto group = btn_free.get_group();
    // btn_free.set_mode(false);
    // btn_circle.set_group(group);
    // btn_circle.set_mode(false);
    // btn_line.set_group(group);
    // btn_line.set_mode(false);
    // btn_rectangle.set_group(group);
    // btn_rectangle.set_mode(false);

    // Add images to the button
    btn_free.set_image_from_icon_name("freehand");
    // btn_free.set_always_show_image();
    btn_free.signal_clicked().connect(sigc::mem_fun(*this, &Drawing::btnfree_clicked));
    btn_circle.set_image_from_icon_name("circle");
    // btn_circle.set_always_show_image();
    btn_circle.signal_clicked().connect(sigc::mem_fun(*this, &Drawing::btncircle_clicked));
    btn_line.set_image_from_icon_name("line");
    // btn_line.set_always_show_image();
    btn_line.signal_clicked().connect(sigc::mem_fun(*this, &Drawing::btnline_clicked));
    btn_rectangle.set_image_from_icon_name("rectangle");
    // btn_rectangle.set_always_show_image();
    btn_rectangle.signal_clicked().connect(sigc::mem_fun(*this, &Drawing::btnrectangle_clicked));

    // Left Panel
    left_box.append(btn_free);
    left_box.append(btn_circle);
    left_box.append(btn_line);
    left_box.append(btn_rectangle);
    left_box.set_valign(Gtk::Align::START);

    // Color set panel
    size_adj = Gtk::Adjustment::create(3.0, 1.0, 20.0);
    scale.set_adjustment(size_adj);
    scale.set_value_pos(Gtk::PositionType::BOTTOM);
    btn_box.append(fill_check);
    btn_box.append(fill_btn);
    btn_box.append(main_label);
    btn_box.append(color_btn);
    btn_box.append(size_label);
    btn_box.append(scale);
    btn_box.append(btn_save);
    btn_box.append(btn_clear);
    btn_box.append(btn_exit);
    btn_box.set_halign(Gtk::Align::CENTER);
    btn_box.set_valign(Gtk::Align::CENTER);

    // Add Gesture
    btn_clear.signal_clicked().connect(sigc::mem_fun(*this, &Drawing::btnclear_clicked));
    btn_save.signal_clicked().connect(sigc::mem_fun(*this, &Drawing::btnsave_clicked));
    btn_exit.signal_clicked().connect(sigc::mem_fun(*this, &Drawing::hide));

    drag = Gtk::GestureDrag::create();
    drag->set_button(GDK_BUTTON_PRIMARY);
    drag->signal_drag_begin().connect(sigc::mem_fun(*this, &Drawing::drag_begin));
    drag->signal_drag_update().connect(sigc::mem_fun(*this, &Drawing::drag_progress));
    drag->signal_drag_end().connect(sigc::mem_fun(*this, &Drawing::drag_end));
    draw_area.add_controller(drag);

    press = Gtk::GestureClick::create();
    press->set_button();
    press->signal_pressed().connect(sigc::mem_fun(*this, &Drawing::button_press));
    draw_area.add_controller(press);

    // Create a Surface
    surface = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, 600, 480);

    // Set Default Color
    m_color.set_blue(0);
    m_color.set_green(0);
    m_color.set_red(0);
    m_color.set_alpha(1);
    color_btn.set_rgba(m_color);
    auto color_dialog = Gtk::ColorDialog::create();
    color_btn.set_dialog(color_dialog);
    // color_btn.signal_color_set().connect(sigc::mem_fun(*this, &Drawing::color_set));
    fill_color = m_color;
    fill_btn.set_rgba(fill_color);
    auto fill_dialog = Gtk::ColorDialog::create();
    fill_btn.set_dialog(fill_dialog);
    // fill_btn.signal_color_set().connect(sigc::mem_fun(*this, &Drawing::color_set));

    // Initalial draw
    auto cr = Cairo::Context::create(surface);
    cr->set_source_rgb(1, 1, 1);
    cr->paint();
    cr.reset();
    draw_area.queue_draw();

    // Set Draw mode to default
    drawing_mode = DrawMode::Default;

    // Initalize main widget
    draw_area.set_size_request(600, 480);
    draw_area.set_draw_func(sigc::mem_fun(*this, &Drawing::draw_event));
    // draw_area.signal_draw().connect(sigc::mem_fun(*this, &Drawing::draw_event));
    main_box.append(left_box);
    main_box.append(draw_area);
    main_box.append(btn_box);
    main_box.set_margin(10);
    set_child(main_box);
    // show_all_children();
}

void Drawing::btnsave_clicked()
{
    // Create a dialog
    dialog = Gtk::FileDialog::create();
    dialog->set_title("Save to png file");

    // Create Filters
    auto filters = Gio::ListStore<Gtk::FileFilter>::create();
    auto filter_png = Gtk::FileFilter::create();
    filter_png->set_name("Png files");
    filter_png->add_pattern("*.png");
    filters->append(filter_png);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any Files");
    filter_any->add_pattern("*");
    filters->append(filter_any);
    dialog->set_filters(filters);

    dialog->save(sigc::mem_fun(*this, &Drawing::dialog_save_file));

    // dialog->show();
}

void Drawing::dialog_save_file(const Glib::RefPtr<Gio::AsyncResult> &result)
{
    try
    {
        // Get file name
        auto file = dialog->save_finish(result);
        std::string filename = file->get_path();

        // Check extension of the file
        size_t length = filename.length();
        std::string extension = filename.substr(length - 3, length - 1);
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        if (extension != "png")
        {
            filename += ".png";
        }

        // Write surface data to the file
        surface->write_to_png(filename);
    }
    catch (Gtk::DialogError &e)
    {
        std::cout << "No file selected" << e.what() << std::endl;
    }
    catch (Glib::Error &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void Drawing::btnfree_clicked()
{
    // Mode1: Free Drawing
    drawing_mode = DrawMode::Default;
}

void Drawing::btnline_clicked()
{
    // Mode2: Draw line, click for start point and end point
    drawing_mode = DrawMode::Line;
}

void Drawing::btncircle_clicked()
{
    // Mode3: Draw circle, click for radius
    drawing_mode = DrawMode::Circle;
}

void Drawing::btnrectangle_clicked()
{
    // Mode4: Draw a rectangle
    drawing_mode = DrawMode::Rectangle;
}

void Drawing::btnclear_clicked()
{
    if (surface)
    {
        // Clear the content in draw area
        auto cr = Cairo::Context::create(surface);
        cr->set_source_rgb(1, 1, 1);
        cr->paint();
        cr.reset();
        draw_area.queue_draw();
    }
}

// Signal Handlers
void Drawing::draw_event(const Cairo::RefPtr<Cairo::Context> &context, int width, int height)
{
    // Show context
    context->set_source(surface, 0, 0);
    context->paint();
    // return true;
}

void Drawing::draw_brush(double x, double y, DrawProcess process)
{
    // Update color
    m_color = color_btn.get_rgba();

    brush_size = scale.get_value();
    auto cr = Cairo::Context::create(surface);

    // Create Draw Brush with specificed size
    cr->set_line_width(brush_size * 2);

    switch (drawing_mode)
    {
    case DrawMode::Default:
        // Use Line for main drawing
        if (process == DrawProcess::Begin)
        {
            rel_x = x;
            rel_y = y;
        }
        else
        {
            cr->move_to(rel_x - 0.1, rel_y - 0.1);
            cr->line_to(x, y);
            rel_x = x;
            rel_y = y;
        }

        // Set Color
        cr->set_source_rgba(m_color.get_red(), m_color.get_green(),
                            m_color.get_blue(), m_color.get_alpha());

        // Fill Color and Delete the brush
        cr->stroke();
        cr.reset();
        break;
    case DrawMode::Line:
        cr->move_to(rel_x, rel_y);
        cr->line_to(x, y);

        // Set Color
        cr->set_source_rgba(m_color.get_red(), m_color.get_green(),
                            m_color.get_blue(), m_color.get_alpha());

        // Fill Color and Delete the brush
        cr->stroke();
        cr.reset();
        break;
    case DrawMode::Circle:
        // Fill Color and Delete the brush
        if (fill_check.get_active())
        {
            cr->arc(rel_x, rel_y, sqrt((x - rel_x) * (x - rel_x) + (y - rel_y) * (y - rel_y)), 0.0, 2 * G_PI);
            // Fill Color
            draw_fill_color(cr);
        }

        cr->arc(rel_x, rel_y, sqrt((x - rel_x) * (x - rel_x) + (y - rel_y) * (y - rel_y)), 0.0, 2 * G_PI);

        cr->set_source_rgba(m_color.get_red(), m_color.get_green(),
                            m_color.get_blue(), m_color.get_alpha());
        cr->stroke();
        cr.reset();
        break;
    case DrawMode::Rectangle:
        // Fill Color and Delete the brush
        if (fill_check.get_active())
        {
            // Fill Color
            cr->rectangle(rel_x, rel_y, abs(x - rel_x), abs(y - rel_y));
            draw_fill_color(cr);
        }
        cr->rectangle(rel_x, rel_y, abs(x - rel_x), abs(y - rel_y));

        cr->set_source_rgba(m_color.get_red(), m_color.get_green(),
                            m_color.get_blue(), m_color.get_alpha());
        cr->stroke();
        cr.reset();
        break;
    }
    draw_area.queue_draw();
}

void Drawing::draw_fill_color(const Cairo::RefPtr<Cairo::Context> &cr)
{
    // Update fill color
    fill_color = fill_btn.get_rgba();

    // Fill Color
    cr->set_source_rgba(fill_color.get_red(), fill_color.get_green(),
                        fill_color.get_blue(), fill_color.get_alpha());
    cr->fill();
}

void Drawing::button_press(int n_press, double x, double y)
{
    auto button = press->get_current_button();
    // std::cout<<button<<std::endl;
    switch (button)
    {
    case 1:
        if (drawing_mode != DrawMode::Default)
        {
            if (begin)
            {
                begin = !begin;
                rel_x = x;
                rel_y = y;
            }
            else
            {
                begin = !begin;
                draw_brush(x, y, DrawProcess::End);
            }
        }
        break;
    case 3:
        btnclear_clicked();
        break;
    }
}

void Drawing::drag_begin(double x, double y)
{
    start_x = x;
    start_y = y;
    draw_brush(x, y, DrawProcess::Begin);
}

void Drawing::drag_progress(double x, double y)
{
    // Progress and end
    draw_brush(x + start_x, y + start_y, DrawProcess::Update);
}

void Drawing::drag_end(double x, double y)
{
    // Progress and end
    draw_brush(x + start_x, y + start_y, DrawProcess::End);
}

void Drawing::color_set()
{
    m_color = color_btn.get_rgba();
    fill_color = fill_btn.get_rgba();
}
