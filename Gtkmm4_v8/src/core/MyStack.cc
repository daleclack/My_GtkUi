#include "MyStack.hh"

MyStack::MyStack()
    : login_box(Gtk::Orientation::VERTICAL, 5),
      login_label("dale"),
      login_button("Login")
{
    // Add the builder
    m_builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/cambalache/overlaywin.ui");

    // Get the stack widget and child widgets
    m_stack = m_builder->get_widget<Gtk::Stack>("main_stack");
    login_page = m_builder->get_widget<Gtk::Overlay>("login_page");
    main_page = m_builder->get_widget<Gtk::Box>("main_page");
    panel_area = m_builder->get_widget<Gtk::Box>("panel_area");
    menu_area = m_builder->get_widget<Gtk::Box>("menu_area");
    topmenu_area = m_builder->get_widget<Gtk::Box>("topmenu_area");

    // Add background image for login page
    login_background.set_resource("/org/gtk/daleclack/winpe.png");
    login_page->set_child(login_background);
    login_background.set_hexpand(true);
    login_background.set_vexpand(true);
    login_background.set_keep_aspect_ratio(false);

    // Create login page widgets
    login_image.set_pixel_size(48);
    login_image.set_from_icon_name("My_GtkUI");
    login_button.set_has_frame(false);
    login_box.append(login_image);
    login_box.append(login_label);
    login_box.append(login_button);
    login_box.set_halign(Gtk::Align::CENTER);
    login_box.set_valign(Gtk::Align::CENTER);
    login_page->add_overlay(login_box);

    //  Add main page widgets
    panel_area->append(*panel1.panel_box);

    // Add menu page widgets
    menu_area->append(*finder1.menu_stack);

    // Add top menu
    topmenu_area->append(finder1.menu_box);

    // Connect signals
    login_button.signal_clicked().connect(sigc::mem_fun(*this, &MyStack::login_button_clicked));
}

void MyStack::login_button_clicked()
{
    m_stack->set_visible_child(*main_page);
}

void MyStack::create_prefs_win(Gtk::Window &parent)
{
    // Create Preferences window
    prefs_window = MyPrefs::create(parent);
    panel1.set_prefs_win(prefs_window);
    panel1.set_parent_window(parent);
    // prefs_window->background_widget_init()
}

MyPrefs *MyStack::get_prefs_win()
{
    // Get the preferences window
    return prefs_window;
}

void MyStack::show_prefs_win()
{
    // Hide the launchpad in panel
    panel1.hide_launchpad();

    // Show the preferences window
    prefs_window->present();
    Gtk::Image *prefs_image = panel1.get_prefs_image();
    prefs_image->set_from_icon_name("my_prefs_running");
}

// Bind the main background widget
void MyStack::set_main_background(Gtk::Picture *background)
{
    // Set the main background widget for the preferences window
    prefs_window->background_widget_init(background);

    // Update theme for the finder
    std::vector<Glib::ustring> finder_classes = {"finder_style"};
    topmenu_area->set_css_classes(finder_classes);
    std::vector<Glib::ustring> dock_classes = {"dock_style"};
    panel1.panel_box->set_css_classes(dock_classes);
    std::vector<Glib::ustring> default_classes = {"default_style"};

    // Load the CSS provider
    auto provider = Gtk::CssProvider::create();
    if (prefs_window->get_dark_mode())
    {
        provider->load_from_resource("/org/gtk/daleclack/style_dark.css");
    }
    else
    {
        provider->load_from_resource("/org/gtk/daleclack/style.css");
    }

    // Apply the style
    Gtk::StyleProvider::add_provider_for_display(topmenu_area->get_display(),
                                                 provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    Gtk::StyleProvider::add_provider_for_display(panel1.panel_box->get_display(),
                                                 provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Apply default style
    panel1.set_internal_style(provider);

    // Update finder configs
    finder1.update_icons(provider, prefs_window->get_dark_mode());
}
