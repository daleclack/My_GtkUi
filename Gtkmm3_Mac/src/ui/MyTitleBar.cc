#include "MyTitleBar.hh"

MyTitleBar::MyTitleBar()
{
    // Load Css Provider
    provider = Gtk::CssProvider::create();
    provider->load_from_resource("/org/gtk/daleclack/title_style.css");

    // Add Buttons
    btn_close.set_image_from_icon_name("window-close-symbolic");
    btn_close.set_hexpand(false);
    btn_close.set_vexpand(false);
    btn_mini.set_image_from_icon_name("window-minimize-symbolic");
    btn_mini.set_hexpand(false);
    btn_mini.set_vexpand(false);
    header.pack_start(btn_close);
    header.pack_start(btn_mini);
    btn_close.signal_clicked().connect(sigc::mem_fun(*this, &MyTitleBar::ctrl_window_close));
    btn_mini.signal_clicked().connect(sigc::mem_fun(*this, &MyTitleBar::ctrl_window_hide));

    // Add title widget
    header.set_custom_title(title_label);

    // Add class for header bar
    auto style1 = header.get_style_context();
    style1->add_class("mytitlebar");
    style1->add_provider(provider, G_MAXUINT);

    // Add class for buttons
    auto style2 = btn_close.get_style_context();
    style2->add_class("ctrl_btn");
    style2->add_provider(provider, G_MAXUINT);

    auto style3 = btn_mini.get_style_context();
    style3->add_class("ctrl_btn");
    style3->add_provider(provider, G_MAXUINT);

    header.show_all();
}

void MyTitleBar::ctrl_window_close()
{
    ctrl_win->close();
}

void MyTitleBar::ctrl_window_hide()
{
    ctrl_win->hide();
}

void MyTitleBar::set_title(const Glib::ustring &title)
{
    title_label.set_label(title);
}

void MyTitleBar::set_ctrl_win(Gtk::Window *window)
{
    window->set_titlebar(header);
    ctrl_win = window;
}

void MyTitleBar::pack_end(Gtk::Widget &widget)
{
    header.pack_end(widget);
}

void MyTitleBar::pack_start(Gtk::Widget &widget)
{
    header.pack_start(widget);
}
