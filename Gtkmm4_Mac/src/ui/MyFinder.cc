#include "MyFinder.hh"
#include <ctime>

MyFinder::MyFinder()
    : label_title("My_GtkUI"),
      label_file("File"),
      label_edit("Edit"),
      label_show("Show"),
      label_goto("Go to"),
      label_win("Windows"),
      label_help("Help")
{
    // Add a menubar
    auto menubuilder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/menubar.xml");

    // Add widgets to menubox
    btnlogo.set_icon_name("My_GtkUI");
    btnlogo.set_has_frame(false);
    // btnlogo.set_use_popover(false);
    btnlogo.set_menu_model(get_menu(menubuilder, "main_menu"));
    append(btnlogo);

    // btntitle.set_label("MyFinder");
    btntitle.set_child(label_title);
    btntitle.set_has_frame(false);
    // btntitle.set_use_popover(false);
    btntitle.set_always_show_arrow(false);
    btntitle.set_menu_model(get_menu(menubuilder, "title_menu"));
    append(btntitle);

    // btnfile.set_label("File");
    btnfile.set_child(label_file);
    btnfile.set_has_frame(false);
    btnfile.set_always_show_arrow(false);
    // btnfile.set_use_popover(false);
    btnfile.set_menu_model(get_menu(menubuilder, "menu_file"));
    append(btnfile);

    // btnedit.set_label("Edit");
    btnedit.set_child(label_edit);
    btnedit.set_has_frame(false);
    btnedit.set_always_show_arrow(false);
    // btnedit.set_use_popover(false);
    btnedit.set_menu_model(get_menu(menubuilder, "menu_edit"));
    append(btnedit);

    // btnshow.set_label("Show");
    btnshow.set_child(label_show);
    btnshow.set_has_frame(false);
    btnshow.set_always_show_arrow(false);
    // btnshow.set_use_popover(false);
    btnshow.set_menu_model(get_menu(menubuilder, "menu_show"));
    append(btnshow);

    // btngoto.set_label("Go to");
    btngoto.set_child(label_goto);
    btngoto.set_has_frame(false);
    btngoto.set_always_show_arrow(false);
    // btngoto.set_use_popover(false);
    btngoto.set_menu_model(get_menu(menubuilder, "menu_goto"));
    append(btngoto);

    // btnwin.set_label("Windows");
    btnwin.set_child(label_win);
    btnwin.set_has_frame(false);
    btnwin.set_always_show_arrow(false);
    // btnwin.set_use_popover(false);
    btnwin.set_menu_model(get_menu(menubuilder, "menu_win"));
    append(btnwin);

    // btnhelp.set_label("Help");
    btnhelp.set_child(label_help);
    btnhelp.set_has_frame(false);
    btnhelp.set_always_show_arrow(false);
    // btnhelp.set_use_popover(false);
    btnhelp.set_menu_model(get_menu(menubuilder, "menu_help"));
    append(btnhelp);

    // Add menu to window
    set_halign(Gtk::Align::FILL);
    set_valign(Gtk::Align::START);

    // Add a separator
    separator.set_hexpand(true);
    separator.set_halign(Gtk::Align::FILL);
    append(separator);

    // Monitor Button
    screen_button.set_image_from_icon_name("finder-computer");
    screen_button.set_has_frame(false);
    append(screen_button);

    // Audio Button
    audio_button.set_image_from_icon_name("finder_audio");
    audio_button.set_has_frame(false);
    append(audio_button);

    // NetWork Button
    network_button.set_image_from_icon_name("finder-wifi");
    network_button.set_has_frame(false);
    append(network_button);

    // finder-battery Button
    battery_button.set_image_from_icon_name("finder-battery");
    battery_button.set_has_frame(false);
    append(battery_button);

    // Add a timer
    time_popover.set_child(calender);
    calender.show();
    calender.set_margin_bottom(5);
    calender.set_margin_end(5);
    calender.set_margin_start(5);
    calender.set_margin_end(5);

    // Timer
    timer_label.set_label("2023/5/12 16:40");
    timer_button.set_child(timer_label);
    timer_button.set_has_frame(false);
    timer_button.set_popover(time_popover);
    timer_button.set_always_show_arrow(false);
    append(timer_button);

    mytimer = Glib::signal_timeout().connect(sigc::mem_fun(*this, &MyFinder::time_out), 1000);

    // Find Button
    find_button.set_image_from_icon_name("finder-find");
    find_button.set_has_frame(false);
    append(find_button);

    // Menu button
    menu_button.set_image_from_icon_name("open-menu");
    menu_button.set_has_frame(false);
    append(menu_button);

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

    timer_label.set_label(time_string);

    return true;
}

Glib::RefPtr<Gio::Menu> MyFinder::get_menu(Glib::RefPtr<Gtk::Builder> &builder, const Glib::ustring &id)
{
    auto object = builder->get_object<Gio::Menu>(id);
    return object;
}
