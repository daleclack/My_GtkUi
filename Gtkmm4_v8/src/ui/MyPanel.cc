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
    btntrash = builder->get_widget<Gtk::Button>("btntrash");
    imagetrash = builder->get_widget<Gtk::Image>("imagetrash");
    imagestart = builder->get_widget<Gtk::Image>("imagestart");


    // Bind callback for the Apps Menu
    app_menu.set_callback(padbtn_clicked);
    MyPanel::instance = this;

    // Create Calculator window
    calc_window = CalcApp::create();

    // Create Guess game window
    game_window = Game::create();

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
    draw_window.signal_close_request().connect(sigc::mem_fun(*this, &MyPanel::drawwin_closed), true);
    btncalc->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btncalc_clicked));
    calc_window->signal_close_request().connect(sigc::mem_fun(*this, &MyPanel::calcwin_closed), true);
    btngame->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btngame_clicked));
    game_window->signal_close_request().connect(sigc::mem_fun(*this, &MyPanel::gamewin_closed), true);
    btnedit->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btnedit_clicked));
    text_window.signal_close_request().connect(sigc::mem_fun(*this, &MyPanel::editwin_closed), true);
    btnviewer->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btnviewer_clicked));
    image_window.signal_close_request().connect(sigc::mem_fun(*this, &MyPanel::viewerwin_closed), true);
    btnset->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btnset_clicked));
    btngame24->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btngame24_clicked));
    game24_window->signal_close_request().connect(sigc::mem_fun(*this, &MyPanel::game24win_closed), true);
    btnmine->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btnmine_clicked));
    mine_window.signal_close_request().connect(sigc::mem_fun(*this, &MyPanel::minewin_closed), true);
    btnmedia->signal_clicked().connect(sigc::mem_fun(*this, &MyPanel::btnmedia_clicked));
    media_window.signal_close_request().connect(sigc::mem_fun(*this, &MyPanel::mediawin_closed), true);
}

// Bind preferences window and bind the callback for icon size change
void MyPanel::set_prefs_win(MyPrefs *prefs)
{
    prefs_window = prefs;
    prefs_window->signal_close_request().connect(sigc::mem_fun(*this, &MyPanel::setwin_closed), true);
    prefs_window->set_icon_callback(MyPanel::iconsize_changed);
};

void MyPanel::set_parent_window(Gtk::Window &parent)
{
    // Set the parent window for applications
    mine_window.set_transient_for(parent);
    media_window.set_transient_for(parent);
    file_window.set_transient_for(parent);
    calc_window->set_transient_for(parent);
    runner_window.set_transient_for(parent);
    game_window->set_transient_for(parent);
    game24_window->set_transient_for(parent);
    text_window.set_transient_for(parent);
    image_window.set_transient_for(parent);
    draw_window.set_transient_for(parent);
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

// Set DPI configuration for icons
void MyPanel::set_icon_size(guint icon_size)
{
    imagestart->set_pixel_size(icon_size);
    imagefiles->set_pixel_size(icon_size);
    imagedraw->set_pixel_size(icon_size);
    imagecalc->set_pixel_size(icon_size);
    imagegame->set_pixel_size(icon_size);
    imageedit->set_pixel_size(icon_size);
    imageviewer->set_pixel_size(icon_size);
    imageset->set_pixel_size(icon_size);
    imagegame24->set_pixel_size(icon_size);
    imagemine->set_pixel_size(icon_size);
    imagemedia->set_pixel_size(icon_size);
    imagetrash->set_pixel_size(icon_size);
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
    // Show or hide the drawing window
    window_ctrl(draw_window);
    imagedraw->set_from_icon_name("drawing_app_running");
}

bool MyPanel::drawwin_closed()
{
    // Hide the drawing window and reset its icon
    draw_window.set_visible(false);
    imagedraw->set_from_icon_name("drawing_app");
    return true;
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
    // Show or hide the guess game window
    window_ctrl(*game_window);
    imagegame->set_from_icon_name("game_running");
}

bool MyPanel::gamewin_closed()
{
    // Hide the guess game window and reset its icon
    game_window->set_visible(false);
    imagegame->set_from_icon_name("game");
    return true;
}


void MyPanel::btnedit_clicked()
{
    // Show or hide the text editor window
    window_ctrl(text_window);
    imageedit->set_from_icon_name("my_textedit_running");
}

bool MyPanel::editwin_closed()
{
    // Hide the text editor window and reset its icon
    text_window.save_config();
    text_window.set_visible(false);
    imageedit->set_from_icon_name("my_textedit");
    return true;
}


void MyPanel::btnviewer_clicked()
{
    // Show or hide the text viewer window
    window_ctrl(image_window);
    imageviewer->set_from_icon_name("image_app_running");
}

bool MyPanel::viewerwin_closed()
{
    // Hide the text viewer window and reset its icon
    image_window.set_visible(false);
    imageviewer->set_from_icon_name("image_app");
    return true;
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

// Callback function for icon size change
void MyPanel::iconsize_changed(guint size)
{
    instance->set_icon_size(size);
    // instance->app_menu.set_icon_size(size);
}

// Callback function for pad button click
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
        instance->draw_window.present();
        instance->imagedraw->set_from_icon_name("drawing_app_running");
        break;
    case 4: // My Finder
        instance->file_window.present();
        instance->imagefiles->set_from_icon_name("file-app_running");
        break;
    case 5: // Guess Game
        instance->game_window->present();
        instance->imagegame->set_from_icon_name("game_running");
        break;
    case 6: // Image Viewer
        instance->image_window.present();
        instance->imageviewer->set_from_icon_name("image_app_running");
        break;
    case 8: // App runner
        instance->runner_window.present();
        break;
    case 9: // Text Editor
        instance->text_window.present();
        instance->imageedit->set_from_icon_name("my_textedit_running");
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
