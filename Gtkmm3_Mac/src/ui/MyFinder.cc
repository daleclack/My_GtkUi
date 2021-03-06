#include "MyFinder.hh"
#include <ctime>

MyFinder::MyFinder()
{
    // Add a menubar
    auto menubuilder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/menubar.xml");

    // Add widgets to menubox
    btnlogo.set_image_from_icon_name("My_GtkUI", Gtk::ICON_SIZE_BUTTON);
    btnlogo.set_relief(Gtk::RELIEF_NONE);
    btnlogo.set_use_popover(false);
    btnlogo.set_menu_model(get_menu(menubuilder, "main_menu"));
    pack_start(btnlogo, Gtk::PACK_SHRINK);

    btntitle.set_label("MyFinder");
    btntitle.set_relief(Gtk::RELIEF_NONE);
    btntitle.set_use_popover(false);
    btntitle.set_menu_model(get_menu(menubuilder, "title_menu"));
    pack_start(btntitle, Gtk::PACK_SHRINK);

    btnfile.set_label("File");
    btnfile.set_relief(Gtk::RELIEF_NONE);
    btnfile.set_use_popover(false);
    btnfile.set_menu_model(get_menu(menubuilder, "menu_file"));
    pack_start(btnfile, Gtk::PACK_SHRINK);

    btnedit.set_label("Edit");
    btnedit.set_relief(Gtk::RELIEF_NONE);
    btnedit.set_use_popover(false);
    btnedit.set_menu_model(get_menu(menubuilder, "menu_edit"));
    pack_start(btnedit, Gtk::PACK_SHRINK);

    btnshow.set_label("Show");
    btnshow.set_relief(Gtk::RELIEF_NONE);
    btnshow.set_use_popover(false);
    btnshow.set_menu_model(get_menu(menubuilder, "menu_show"));
    pack_start(btnshow, Gtk::PACK_SHRINK);

    btngoto.set_label("Go to");
    btngoto.set_relief(Gtk::RELIEF_NONE);
    btngoto.set_use_popover(false);
    btngoto.set_menu_model(get_menu(menubuilder, "menu_goto"));
    pack_start(btngoto, Gtk::PACK_SHRINK);

    btnwin.set_label("Windows");
    btnwin.set_relief(Gtk::RELIEF_NONE);
    btnwin.set_use_popover(false);
    btnwin.set_menu_model(get_menu(menubuilder, "menu_win"));
    pack_start(btnwin, Gtk::PACK_SHRINK);

    btnhelp.set_label("Help");
    btnhelp.set_relief(Gtk::RELIEF_NONE);
    btnhelp.set_use_popover(false);
    btnhelp.set_menu_model(get_menu(menubuilder, "menu_help"));
    pack_start(btnhelp, Gtk::PACK_SHRINK);

    // Add menu to window
    set_halign(Gtk::ALIGN_FILL);
    set_valign(Gtk::ALIGN_START);

    // Add a timer
    time_popover.add(calender);
    calender.show();
    calender.set_margin_bottom(5);
    calender.set_margin_end(5);
    calender.set_margin_start(5);
    calender.set_margin_end(5);

    // Menu button
    menu_button.set_image_from_icon_name("open-menu");
    menu_button.set_relief(Gtk::RELIEF_NONE);
    pack_end(menu_button, Gtk::PACK_SHRINK);

    // Find Button
    find_button.set_image_from_icon_name("finder-find");
    find_button.set_relief(Gtk::RELIEF_NONE);
    pack_end(find_button, Gtk::PACK_SHRINK);

    // Timer
    timer_button.set_label("2022/1/23 18:32");
    timer_button.set_relief(Gtk::RELIEF_NONE);
    timer_button.set_popover(time_popover);
    pack_end(timer_button, Gtk::PACK_SHRINK);

    mytimer = Glib::signal_timeout().connect(sigc::mem_fun(*this, &MyFinder::time_out), 1000);

    // finder-battery Button
    battery_button.set_image_from_icon_name("finder-battery");
    battery_button.set_relief(Gtk::RELIEF_NONE);
    pack_end(battery_button, Gtk::PACK_SHRINK);

    // NetWork Button
    network_button.set_image_from_icon_name("finder-wifi");
    network_button.set_relief(Gtk::RELIEF_NONE);
    pack_end(network_button, Gtk::PACK_SHRINK);

    // Audio Button
    audio_button.set_image_from_icon_name("finder_audio");
    audio_button.set_relief(Gtk::RELIEF_NONE);
    pack_end(audio_button, Gtk::PACK_SHRINK);

    // Monitor Button
    screen_button.set_image_from_icon_name("finder-computer");
    screen_button.set_relief(Gtk::RELIEF_NONE);
    pack_end(screen_button, Gtk::PACK_SHRINK);

    // Add Style for MyFinder
    provider = Gtk::CssProvider::create();
    provider->load_from_resource("/org/gtk/daleclack/style.css");
    auto style = get_style_context();
    style->add_provider(provider, G_MAXUINT);
}

bool MyFinder::time_out()
{

    char time_string[57];

    // Get local time
    time_t t;
    struct tm *local;
    t = time(NULL);
    local = localtime(&t);

    // Format the time to show
    sprintf(time_string, "%04d/%d/%d %02d:%02d:%02d", local->tm_year + 1900, local->tm_mon + 1,
            local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);

    timer_button.set_label(time_string);

    return true;
}

Glib::RefPtr<Gio::Menu> MyFinder::get_menu(Glib::RefPtr<Gtk::Builder> &builder, const Glib::ustring &id)
{
    auto object = builder->get_object(id);
    auto gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
    return gmenu;
}
