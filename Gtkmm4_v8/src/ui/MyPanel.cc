#include "MyPanel.hh"
#include <iostream>

// void padbtn_clicked(guint id)
// {
//     std::cout << "Clicked button with id: " << id << std::endl;
// }

MyPanel *MyPanel::instance = nullptr;

MyPanel::MyPanel()
{
    // Get the panel widget
    builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/cambalache/mypanel.ui");
    panel_box = builder->get_widget<Gtk::Box>("panel_box");
    btnstart = builder->get_widget<Gtk::Button>("btnstart");
    apps_stack = builder->get_widget<Gtk::Stack>("apps_stack");
    apps_box = builder->get_widget<Gtk::Box>("apps_box");
    apps_sw = builder->get_widget<Gtk::ScrolledWindow>("apps_sw");

    // Get widgets for task manager
    btnfiles = builder->get_widget<Gtk::Button>("btnfiles");
    imagefiles = builder->get_widget<Gtk::Image>("imagefiles");
    btndraw = builder->get_widget<Gtk::Button>("btndraw");
    imagedraw = builder->get_widget<Gtk::Image>("imagedraw");
    btncalc = builder->get_widget<Gtk::Button>("btncalc");
    imagecalc = builder->get_widget<Gtk::Image>("imagecalc");
    btngame = builder->get_widget<Gtk::Button>("btngame");
    imagegame = builder->get_widget<Gtk::Image>("imagegame");
    btnedit = builder->get_widget<Gtk::Button>("btnedit");
    imageedit = builder->get_widget<Gtk::Image>("imageedit");
    btnviewer = builder->get_widget<Gtk::Button>("btnviewer");
    imageviewer = builder->get_widget<Gtk::Image>("imageviewer");
    btnset = builder->get_widget<Gtk::Button>("btnset");
    imageset = builder->get_widget<Gtk::Image>("imageset");
    btngame24 = builder->get_widget<Gtk::Button>("btngame24");
    imagegame24 = builder->get_widget<Gtk::Image>("imagegame24");
    btnmine = builder->get_widget<Gtk::Button>("btnmine");
    imagemine = builder->get_widget<Gtk::Image>("imagemine");
    btnmedia = builder->get_widget<Gtk::Button>("btnmedia");
    imagemedia = builder->get_widget<Gtk::Image>("imagemedia");

    // Bind callback for the Apps Menu
    app_menu.set_callback(padbtn_clicked);
    MyPanel::instance = this;

    // Create Calculator window
    calc_window = CalcApp::create();

    // Create 24 Game window
    game24_window = Game24Win::create();

    // Hide the scrollbar of scrolled window
    auto scrollbar = apps_sw->get_vscrollbar();
    scrollbar->set_visible(false);

    // Add apps view
    apps_box->append(app_menu);

    // Connect signal handlers
    btnstart->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btnstart_clicked));
    btnfiles->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btnfiles_clicked));
    file_window.signal_close_request().connect(sigc::mem_fun(*this, &MyPanel::filewin_closed), true);
    btndraw->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btndraw_clicked));
    btncalc->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btncalc_clicked));
    calc_window->signal_close_request().connect(sigc::mem_fun(*this, &MyPanel::calcwin_closed), true);
    btngame->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btngame_clicked));
    btnedit->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btnedit_clicked));
    btnviewer->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btnviewer_clicked));
    btnset->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btnset_clicked));
    btngame24->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btngame24_clicked));
    btnmine->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btnmine_clicked));
    mine_window.signal_close_request().connect(sigc::mem_fun(*this, &MyPanel::minewin_closed), true);
    btnmedia->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btnmedia_clicked));
    media_window.signal_close_request().connect(sigc::mem_fun(*this, &MyPanel::mediawin_closed), true);
}

void MyPanel::set_prefs_win(MyPrefs *prefs)
{
    prefs_window = prefs;
    prefs_window->signal_close_request().connect(sigc::mem_fun(*this, &MyPanel::setwin_closed), true);
}

void MyPanel::set_parent_window(Gtk::Window &parent)
{
    // Set the parent window for applications
    mine_window.set_transient_for(parent);
    media_window.set_transient_for(parent);
    file_window.set_transient_for(parent);
    calc_window->set_transient_for(parent);
    runner_window.set_transient_for(parent);
}

void MyPanel::set_internal_style(const Glib::RefPtr<Gtk::CssProvider> &provider)
{
    // Set css classes for widgets
    std::vector<Glib::ustring> default_classes = {"default_style"};
    auto child = apps_sw->get_child();
    child->set_css_classes(default_classes);
    apps_box->set_css_classes(default_classes);

    // Apply the CSS provider to the widget
    Gtk::StyleProvider::add_provider_for_display(child->get_display(),
                                                 provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    Gtk::StyleProvider::add_provider_for_display(apps_box->get_display(),
                                                 provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

Gtk::Image *MyPanel::get_prefs_image()
{
    // Get image widget for preferences button
    return imageset;
}

void MyPanel::hide_launchpad()
{
    // Hide the launchpad by change page
    apps_stack->set_visible_child("page1");
}

void MyPanel::window_ctrl(Gtk::Window &window)
{
    // Show or hide the window based on its current state
    if (window.is_visible())
    {
        window.set_visible(false);
    }
    else
    {
        window.present();
    }
}

void MyPanel::btnstart_clicked()
{
    // Change the visible page in the stack
    auto child_name = apps_stack->get_visible_child_name();
    if (child_name == "page1")
    {
        apps_stack->set_visible_child("page2");
    }
    else
    {
        apps_stack->set_visible_child("page1");
    }
}

void MyPanel::btnfiles_clicked()
{
    // Show or hide the file manager window
    window_ctrl(file_window);
    imagefiles->set_from_icon_name("file-app_running");
}

bool MyPanel::filewin_closed()
{
    // Hide the file manager window and reset its icon
    file_window.set_visible(false);
    imagefiles->set_from_icon_name("file-app");
    return true;
}


void MyPanel::btndraw_clicked()
{
}

void MyPanel::btncalc_clicked()
{
    // Show or hide the calculator window
    window_ctrl(*calc_window);
    imagecalc->set_from_icon_name("calcapp_running");
}

bool MyPanel::calcwin_closed()
{
    // Hide the calculator window and reset its icon
    calc_window->set_visible(false);
    imagecalc->set_from_icon_name("calcapp");
    return true;
}

void MyPanel::btngame_clicked()
{
}

void MyPanel::btnedit_clicked()
{
}

void MyPanel::btnviewer_clicked()
{
}

void MyPanel::btnset_clicked()
{
    // Show the preferences window
    // prefs_window->present();
    window_ctrl(*prefs_window);
    imageset->set_from_icon_name("my_prefs_running");
}

bool MyPanel::setwin_closed()
{
    prefs_window->set_visible(false);
    imageset->set_from_icon_name("my_prefs");
    return true;
}

void MyPanel::btngame24_clicked()
{
    window_ctrl(*game24_window);
    imagegame24->set_from_icon_name("24game_running");
}

bool MyPanel::game24win_closed()
{
    // Hide the 24 Game window and reset its icon
    game24_window->set_visible(false);
    imagegame24->set_from_icon_name("24game");
    return true;
}

void MyPanel::btnmine_clicked()
{
    window_ctrl(mine_window);
    imagemine->set_from_icon_name("mines_app_running");
}

bool MyPanel::minewin_closed()
{
    mine_window.set_visible(false);
    imagemine->set_from_icon_name("mines_app");
    return true;
}

void MyPanel::btnmedia_clicked()
{
    window_ctrl(media_window);
    imagemedia->set_from_icon_name("media-app_running");
}

bool MyPanel::mediawin_closed()
{
    media_window.set_visible(false);
    imagemedia->set_from_icon_name("media-app");
    return true;
}


void MyPanel::padbtn_clicked(guint id)
{
    // Hide the app menu when a button is clicked
    instance->apps_stack->set_visible_child("page1");

    // Launch the application based on the button ID
    switch (id)
    {
    case 0: // 24 Game
        instance->game24_window->present();
        instance->imagegame24->set_from_icon_name("24game_running");
        break;
    case 2: // Calculator
        instance->calc_window->present();
        instance->imagecalc->set_from_icon_name("calcapp_running");
        break;
    case 3: // Drawing
        break;
    case 4: // My Finder
        instance->file_window.present();
        instance->imagefiles->set_from_icon_name("file-app_running");
        break;
    case 5: // Guess Game
        break;
    case 6: // Image Viewer
        break;
    case 8: // App runner
        instance->runner_window.present();
        break;
    case 9: // Text Editor
        break;
    case 10: // Media Player
        instance->media_window.present();
        instance->imagemedia->set_from_icon_name("media-app_running");
        break;
    case 11: // MineSweeper
        instance->mine_window.present();
        instance->imagemine->set_from_icon_name("mines_app_running");
        break;
    default:
        break;
    }
    // std::cout << "Clicked button with id: " << id << std::endl;
    // if (instance != nullptr)
    //     std::cout << "Instance is not null" << std::endl;
}
