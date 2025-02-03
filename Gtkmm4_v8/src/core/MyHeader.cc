#include "MyHeader.hh"

MyHeader::MyHeader()
{
    // Set style for header bar
    std::vector<Glib::ustring> header_classes = {"mytitlebar"};
    header.set_css_classes(header_classes);

    // Hide default control buttons
    header.set_show_title_buttons(false);

    // Add close button
    img_close.set_from_icon_name("header_close");
    btn_close.set_child(img_close);
    std::vector<Glib::ustring> btnclose_classes = {"ctrl_btn", "close"};
    btn_close.set_css_classes(btnclose_classes);
    header.pack_start(btn_close);

    // Add minimize button
    img_minimize.set_from_icon_name("header_minimize");
    btn_minimize.set_child(img_minimize);
    std::vector<Glib::ustring> btnmini_classes = {"ctrl_btn"};
    btn_minimize.set_css_classes(btnmini_classes);
    header.pack_start(btn_minimize);

    // Add style for titlebar and buttons
    auto css_provider = Gtk::CssProvider::create();
    css_provider->load_from_resource("/org/gtk/daleclack/title_style.css");
    Gtk::StyleProvider::add_provider_for_display(header.get_display(),
                                                 css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Connect signals
    btn_close.signal_clicked().connect(sigc::mem_fun(*this, &MyHeader::ctrl_win_close));
    btn_minimize.signal_clicked().connect(sigc::mem_fun(*this, &MyHeader::ctrl_win_minimize));

    set_dpi_scale(1.25);
}

void MyHeader::set_dpi_scale(double dpi_scale)
{
    // Button size depends on DPI scale
    btn_close.set_size_request(18 * dpi_scale, 18 * dpi_scale);
    btn_minimize.set_size_request(18 * dpi_scale, 18 * dpi_scale);
    btn_close.set_margin(2 * dpi_scale);
    btn_minimize.set_margin(2 * dpi_scale);

    // Image size also depends on DPI scale
    img_close.set_pixel_size(14 * dpi_scale);
    img_minimize.set_pixel_size(14 * dpi_scale);
}

// Set title bar
void MyHeader::set_title_bar(Gtk::Window &window)
{
    window.set_titlebar(header);
}

// Bind controlled window
void MyHeader::set_ctrl_window(Gtk::Window *ctrl_window1)
{
    ctrl_window = ctrl_window1;
}

// Close controlled window
void MyHeader::ctrl_win_close()
{
    ctrl_window->close();
}

// Minimize controlled window
void MyHeader::ctrl_win_minimize()
{
    ctrl_window->set_visible(false);
}
