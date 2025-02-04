#include "MainWin.hh"

MainWin::MainWin(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_builder1)
    : Glib::ObjectBase("MainWin"),
      Gtk::ApplicationWindow(cobject),
      ref_builder(ref_builder1)
{
    set_default_icon_name("My_GtkUI");
    // Get widgets from the builder
    main_overlay = ref_builder->get_widget<Gtk::Overlay>("main_overlay");
    main_background = ref_builder->get_widget<Gtk::Picture>("main_background");

    // Default background image
    main_background->set_keep_aspect_ratio(false);
    main_background->set_hexpand(true);
    main_background->set_vexpand(true);
    main_background->set_resource("/org/gtk/daleclack/shizuku.png");

    // Add stack to the window
    Gtk::Stack *stack = main_stack.m_stack;
    stack->set_halign(Gtk::Align::FILL);
    stack->set_valign(Gtk::Align::FILL);
    main_overlay->add_overlay(*stack);

    // Initalize the context menu
    menu_builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/mainmenu.xml");
    auto menu = menu_builder->get_object<Gio::MenuModel>("model");
    context_menu.set_menu_model(menu);
    context_menu.set_parent(*main_overlay);
    context_menu.set_has_arrow(false);
    context_menu.present();

    main_stack.create_prefs_win(*this);

    // Add gesture for the right click
    gesture_click = Gtk::GestureClick::create();
    main_overlay->add_controller(gesture_click);
    gesture_click->set_button(GDK_BUTTON_SECONDARY);
    gesture_click->signal_pressed().connect(sigc::mem_fun(*this, &MainWin::gesture_pressed));

    // Add actions
    add_action("about", sigc::mem_fun(*this, &MainWin::about_activate));
    add_action("back", sigc::mem_fun(*this, &MainWin::back_activate));
    add_action("logout", sigc::mem_fun(*this, &MainWin::logout_activate));
    add_action("shutdown", sigc::mem_fun(*this, &MainWin::shutdown_activate));
}

void MainWin::gesture_pressed(int n_press, double x, double y)
{
    // Show the context menu at the position of the right click.
    context_menu.set_pointing_to(Gdk::Rectangle(x, y, 1, 1));
    context_menu.popup();
}

MainWin *MainWin::create()
{
    // Create a new MainWin instance and return it.
    MainWin *main_win = nullptr;

    // Create a new Builder object and load the interface file.
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/cambalache/mainwin.ui");

    // Get the MainWin object from the builder and return it.
    main_win = Gtk::Builder::get_widget_derived<MainWin>(builder, "mainwin");
    return main_win;
}

void MainWin::about_activate()
{
    // Authors information
    const char *authors[] = {
        "Dale Clack",
        "GCR_CMake on github https://github.com/Makman2/GCR_CMake",
        NULL};

    // Version information
    char *version;
    version = g_strdup_printf("8.0 Alpha\nRunning Against: Gtkmm %d.%d.%d",
                              GTKMM_MAJOR_VERSION,
                              GTKMM_MINOR_VERSION,
                              GTKMM_MICRO_VERSION);

    // Get Year information
    time_t t;
    t = time(NULL);
    struct tm *local;
    local = localtime(&t);

    // Copyright
    char *copyright;
    copyright = g_strdup_printf("© 2019—%04d The Xe Project", local->tm_year + 1900);

    // Show the about dialog
    gtk_show_about_dialog(GTK_WINDOW(gobj()),
                          "program-name", "My GtkUI Techinal Alpha",
                          "version", version,
                          "copyright", copyright,
                          "comments", "A program that simulates desktop, inspired by lomiri desktop",
                          "authors", authors,
                          "license-type", GTK_LICENSE_GPL_3_0,
                          "logo-icon-name", "My_GtkUI",
                          "title", "About My GtkUI Mac Version",
                          NULL, nullptr);

    // Free Memory
    g_free(version);
    g_free(copyright);
}

void MainWin::logout_activate()
{
    // Change the page to the logout page.
    main_stack.m_stack->set_visible_child("login_page");
}

void MainWin::shutdown_activate()
{
    // Destroy the main window to quit the application.
    destroy();
}

void MainWin::back_activate()
{
    main_stack.show_prefs_win();
}
