#include "ImageApp.hh"
#include <iostream>

ImageApp::ImageApp()
    : main_box(Gtk::Orientation::VERTICAL, 5),
      btnbox(Gtk::Orientation::HORIZONTAL, 5),
      btnopen("Open Image")
{
    header.set_ctrl_window(this);
    // Add Widgets
    set_default_size(800, 450);
    set_icon_name("image_app");
    set_title("Image Viewer");

    // Scrolled Window
    sw.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    sw.set_child(image_area);
    sw.set_expand(true);
    main_box.append(sw);

    // Initalize Scale
    m_adjustment = Gtk::Adjustment::create(1.0, 0.1, 10.0, 0.1, 0.1);
    // scale.set_default_direction(Gtk::TextDirection::LTR);
    scale.set_adjustment(m_adjustment);
    scale.signal_value_changed().connect(sigc::mem_fun(*this, &ImageApp::scale_changed));
    scale.set_hexpand(true);
    scale.set_draw_value(true);
    scale.set_value_pos(Gtk::PositionType::LEFT);

    // Add control widgets
    btnopen.set_has_frame(false);
    btnbox.append(scale);
    btnbox.append(btnopen);
    main_box.append(btnbox);
    btnopen.signal_clicked().connect(sigc::mem_fun(*this, &ImageApp::btnopen_clicked));

    // Add Drag Gesture
    gesture_drag = Gtk::GestureDrag::create();
    gesture_drag->set_button(GDK_BUTTON_PRIMARY);
    gesture_drag->signal_drag_update().connect(sigc::mem_fun(*this, &ImageApp::drag_update));
    gesture_drag->signal_drag_end().connect(sigc::mem_fun(*this, &ImageApp::drag_end));
    image_area.add_controller(gesture_drag);

    gesture_click = Gtk::GestureClick::create();
    gesture_click->set_button(GDK_BUTTON_SECONDARY);
    gesture_click->signal_pressed().connect(sigc::mem_fun(*this, &ImageApp::press));
    image_area.add_controller(gesture_click);

    // Add Menu
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/image_appmenu.xml");
    auto object = builder->get_object<Gio::MenuModel>("model");
    popover.set_menu_model(object);
    popover.set_parent(image_area);

    // Add actions for menu
    add_action("zoom_in", sigc::mem_fun(*this, &ImageApp::image_zoom_in));
    add_action("zoom_out", sigc::mem_fun(*this, &ImageApp::image_zoom_out));
    add_action("zoom_reset", sigc::mem_fun(*this, &ImageApp::image_zoom_reset));

    overlay.set_child(main_box);
    set_child(overlay);
    // show_all_children();
}

void ImageApp::btnopen_clicked()
{
    // Create a file open dialog
    dialog = Gtk::FileDialog::create();
    dialog->set_title("Open a image File");

    dialog->open(sigc::mem_fun(*this, &ImageApp::dialog_open_file));
    // dialog = Gtk::FileChooserNative::create("Open a image File", *this, Gtk::FileChooser::Action::OPEN,
    //                                         "OK", "Cancel");

    // dialog->signal_response().connect(sigc::mem_fun(*this, &ImageApp::dialog_response));

    // dialog->show();
}

void ImageApp::dialog_open_file(const Glib::RefPtr<Gio::AsyncResult> &result)
{
    try
    {
        // Show the image in a drawing area
        // auto file = dialog->get_file();
        auto file = dialog->open_finish(result);
        auto filename = file->get_path();
        auto pixbuf = Gdk::Pixbuf::create_from_file(filename);
        image_area.set_pixbuf(pixbuf);
        pixbuf.reset();
        // Set Adjustments
        if (hadjustment)
        {
            hadjustment.reset();
            vadjustment.reset();
        }
        hadjustment = sw.get_hadjustment();
        vadjustment = sw.get_vadjustment();
        file.reset();
    }
    catch (const Gtk::DialogError &ex)
    {
        std::cout << "No file selected " << ex.what() << std::endl;
    }
    catch (const Glib::Error &ex){
        std::cout << "Error: " << ex.what() << std::endl;
    }


    dialog.reset();
}

void ImageApp::scale_changed()
{
    // Get Value of scale widget and scale image
    double value = scale.get_value();
    // g_print("%f\n", value);
    image_area.scale_draw(value);
}

void ImageApp::press(int n_press, double x, double y)
{
    // Set Popover to the position of mouse and show
    popover.set_pointing_to(Gdk::Rectangle(x, y, 1, 1));
    popover.popup();
}

void ImageApp::drag_begin(double x, double y)
{
}

void ImageApp::drag_update(double x, double y)
{
    // g_print("drag updated\n");
    move_to(x, y);
}

void ImageApp::drag_end(double x, double y)
{
    // g_print("drag ended\n");
    move_to(x, y);
}

void ImageApp::move_to(double x, double y)
{
    // g_print("%f %f\n",x,y);
    // Get Current Value
    double h_value = hadjustment->get_value();
    double v_value = vadjustment->get_value();
    // Get Max Value for movement
    double hmaxvalue = hadjustment->get_upper(); //- hadjustment->get_page_size();
    double vmaxvalue = hadjustment->get_upper(); //- vadjustment->get_page_size();
    h_value -= x;
    v_value -= y;
    // Before movement, make sure the value is vaild
    if (h_value < 0)
    {
        h_value = 0;
    }
    if (h_value > hmaxvalue)
    {
        h_value = hmaxvalue;
    }
    // Before movement, make sure the value is vaild
    if (v_value < 0)
    {
        v_value = 0;
    }
    if (v_value > vmaxvalue)
    {
        v_value = vmaxvalue;
    }
    // Perform movement
    hadjustment->set_value(h_value);
    vadjustment->set_value(v_value);
}

void ImageApp::image_zoom_in()
{
    // Scale 0.1 More
    double value = scale.get_value();
    value += 0.1;
    scale.set_value(value);
    image_area.scale_draw(value);
}

void ImageApp::image_zoom_out()
{
    // Scale 0.1 Less
    double value = scale.get_value();
    value -= 0.1;
    scale.set_value(value);
    image_area.scale_draw(value);
}

void ImageApp::image_zoom_reset()
{
    // Scale as 1:1
    scale.set_value(1.0);
    image_area.scale_draw(1.0);
}
