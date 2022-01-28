#include "MyWin.hh"
#include "winpe.xpm"

MyWin::MyWin()
    : menubox(Gtk::ORIENTATION_HORIZONTAL, 0)
{
    // Initalize window
    set_default_size(1024, 576);
    set_icon_name("My_GtkUI");
    set_title("My GtkUI macOS Version");

    // Add background
    auto pixbuf = Gdk::Pixbuf::create_from_xpm_data(winpe);
    auto sized = pixbuf->scale_simple(1024, 576, Gdk::INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(m_background.gobj(), sized->gobj());
    m_overlay.add(m_background);

    // Add Action for menus
    add_action("logout", sigc::mem_fun(*this, &MyWin::logout_activated));
    add_action("quit", sigc::mem_fun(*this, &MyWin::quit_activated));
    add_action("about",sigc::mem_fun(*this,&MyWin::about_activated));

    // Add Stack
    m_overlay.add_overlay(*(main_stack.stack));

    // Add context menu
    auto menubuilder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/mainmenu.xml");
    auto object = menubuilder->get_object("model");
    auto gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
    context_menu.bind_model(gmenu);
    context_menu.set_relative_to(m_overlay);

    // Add Gesture for control
    gesture = Gtk::GestureMultiPress::create(m_overlay);
    gesture->set_button(GDK_BUTTON_SECONDARY);
    gesture->signal_pressed().connect(sigc::mem_fun(*this, &MyWin::press));

    add(m_overlay);
    show_all_children();
}

void MyWin::press(int n_press, double x, double y)
{
    // Show Menu
    context_menu.set_pointing_to(Gdk::Rectangle((int)x, (int)y, 1, 1));
    context_menu.popup();
}

void MyWin::logout_activated()
{
    main_stack.logout();
}

void MyWin::about_activated()
{
    // Authors information
    const char *authors[] = {
        "Dale Clack",
        "GCR_CMake on github https://github.com/Makman2/GCR_CMake",
        NULL};

    //Version information
    char *version;
    version = g_strdup_printf("5.0\nRunning Against: Gtkmm %d.%d.%d",
                              GTKMM_MAJOR_VERSION,
                              GTKMM_MINOR_VERSION,
                              GTKMM_MICRO_VERSION);

    // Get Year information
    time_t t;
    t=time(NULL);
    struct tm * local;
    local = localtime(&t);

    //Copyright
    char * copyright;
    copyright = g_strdup_printf("© 2019—%04d The Xe Project",local->tm_year+1900);

    //Show the about dialog
    gtk_show_about_dialog(GTK_WINDOW(gobj()),
                          "program-name", "My_GtkUI",
                          "version", version,
                          "copyright", copyright,
                          "comments", "A program that simulates desktop (Mac Version)",
                          "authors", authors,
                          "license-type", GTK_LICENSE_GPL_3_0,
                          "logo-icon-name", "My_GtkUI",
                          "title", "About My GtkUI Mac Version",
                          NULL);
    
    //Free Memory
    g_free(version);
    g_free(copyright);
}

void MyWin::quit_activated()
{
    hide();
}
