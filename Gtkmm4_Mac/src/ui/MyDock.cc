#include "MyDock.hh"
#include <thread>
#include <iostream>

// Dock or Panel Mode
static DockMode mode1;

MyDock::MyDock(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_Glade)
    : Gtk::Box(cobject),
      ref_builder(ref_Glade),
      launchpad_shown(false)
{
    // Get Widgets
    finder_box = ref_builder->get_widget<Gtk::Box>("finder_box");
    dock_box = ref_builder->get_widget<Gtk::Box>("dock_box");
    dock_left = ref_builder->get_widget<Gtk::Box>("dock_left");
    dock_right = ref_builder->get_widget<Gtk::Box>("dock_right");
    dock_bottom = ref_builder->get_widget<Gtk::Box>("dock_bottom");
    icons_box = ref_builder->get_widget<Gtk::Box>("icons_box");
    btnlaunch = ref_builder->get_widget<Gtk::Button>("btnlaunch");
    btndraw = ref_builder->get_widget<Gtk::Button>("btndraw");
    btnfiles = ref_builder->get_widget<Gtk::Button>("btnfiles");
    btngame = ref_builder->get_widget<Gtk::Button>("btngame");
    btnedit = ref_builder->get_widget<Gtk::Button>("btnedit");
    btnimage = ref_builder->get_widget<Gtk::Button>("btnimage");
    btnset = ref_builder->get_widget<Gtk::Button>("btnset");
    btngame24 = ref_builder->get_widget<Gtk::Button>("btngame24");
    btncalc = ref_builder->get_widget<Gtk::Button>("btncalc");
    btnmine = ref_builder->get_widget<Gtk::Button>("btnmine");
    separator_start = ref_builder->get_widget<Gtk::Separator>("separator_start");
    separator_2 = ref_builder->get_widget<Gtk::Separator>("separator_2");
    separator_3 = ref_builder->get_widget<Gtk::Separator>("separator_3");
    separator_end = ref_builder->get_widget<Gtk::Separator>("separator_end");
    icons_sw = ref_builder->get_widget<Gtk::ScrolledWindow>("icons_sw");
    launchpad_stack = ref_builder->get_widget<Gtk::Stack>("launchpad_stack");
    default_page = ref_builder->get_widget<Gtk::Grid>("default_page");
    launchpad_page = ref_builder->get_widget<Gtk::Grid>("launchpad_page");
    apps_grid = ref_builder->get_widget<Gtk::Grid>("apps_grid");
    padaud = ref_builder->get_widget<Gtk::Button>("padaud");
    padgedit = ref_builder->get_widget<Gtk::Button>("padgedit");
    padvlc = ref_builder->get_widget<Gtk::Button>("padvlc");
    padnote = ref_builder->get_widget<Gtk::Button>("padnote");
    padvlc_win32 = ref_builder->get_widget<Gtk::Button>("padvlc_win32");
    padset = ref_builder->get_widget<Gtk::Button>("padset");
    paddraw = ref_builder->get_widget<Gtk::Button>("paddraw");
    padfile = ref_builder->get_widget<Gtk::Button>("padfile");
    paddraw = ref_builder->get_widget<Gtk::Button>("paddraw");
    padgame = ref_builder->get_widget<Gtk::Button>("padgame");
    padimage = ref_builder->get_widget<Gtk::Button>("padimage");
    padedit = ref_builder->get_widget<Gtk::Button>("padedit");
    padrun = ref_builder->get_widget<Gtk::Button>("padrun");
    padgame24 = ref_builder->get_widget<Gtk::Button>("padgame24");
    padcalc = ref_builder->get_widget<Gtk::Button>("padcalc");
    padmine = ref_builder->get_widget<Gtk::Button>("padmine");

    // // Create window
    // game_win = Game::create();
    // game24_win = Game24Win::create();
    // calc_win = CalcApp::create();

    // Link signals

    // These signal handlers not contain a icon on the dock
    btnlaunch->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btnlaunch_clicked));
    padaud->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padaud_clicked));
    padgedit->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padgedit_clicked));
    padvlc->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padvlc_clicked));
    padnote->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padnote_clicked));
    padvlc_win32->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padvlc_win32_clicked));
    // padrun->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btnrun_clicked));

    /*
    These functions contain a icon on the dock
    the first signal for control button on dock
    the next signal for button on launchpad
    last 1 or 2 signals for the window
    */

    // btnset->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btnset_clicked));
    // padset->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padset_clicked));
    // prefs_win.signal_delete_event().connect(sigc::mem_fun(*this, &MyDock::prefs_win_closed));

    // btndraw->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btndraw_clicked));
    // paddraw->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::paddraw_clicked));
    // draw_app.signal_delete_event().connect(sigc::mem_fun(*this, &MyDock::draw_win_closed));
    // draw_app.signal_hide().connect(sigc::mem_fun(*this, &MyDock::draw_win_hide));

    // btnfiles->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btnfile_clicked));
    // padfile->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padfile_clicked));
    // file_app.signal_delete_event().connect(sigc::mem_fun(*this, &MyDock::file_win_closed));

    // btngame->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btngame_clicked));
    // padgame->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padgame_clicked));
    // game_win->signal_delete_event().connect(sigc::mem_fun(*this, &MyDock::game_win_closed));
    // game_win->signal_hide().connect(sigc::mem_fun(*this, &MyDock::game_win_hide));

    // btngame24->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btngame24_clicked));
    // padgame24->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padgame24_clicked));
    // game24_win->signal_delete_event().connect(sigc::mem_fun(*this, &MyDock::game24_win_closed));
    // game24_win->signal_hide().connect(sigc::mem_fun(*this, &MyDock::game24_win_hide));

    // btncalc->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btncalc_clicked));
    // padcalc->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padcalc_clicked));
    // calc_win->signal_delete_event().connect(sigc::mem_fun(*this, &MyDock::calc_win_closed));
    // calc_win->signal_hide().connect(sigc::mem_fun(*this, &MyDock::calc_win_hide));

    // btnimage->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btnimage_clicked));
    // padimage->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padimage_clicked));
    // image_win.signal_delete_event().connect(sigc::mem_fun(*this, &MyDock::image_win_closed));

    // btnedit->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btnedit_clicked));
    // padedit->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padedit_clicked));
    // editor_win.signal_delete_event().connect(sigc::mem_fun(*this, &MyDock::editor_win_closed));

    // btnmine->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btnmine_clicked));
    // padmine->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padmine_clicked));
    // mine_win.signal_delete_event().connect(sigc::mem_fun(*this, &MyDock::mine_win_closed));

    // Add Finder
    finder_box->append(finder);
    finder_box->set_hexpand(true);
    finder.set_hexpand(true);

    // Add Style for MyFinder
    provider = Gtk::CssProvider::create();
    provider->load_from_resource("/org/gtk/daleclack/dock_style.css");
    apply_style(*dock_box);
    apply_style(*launchpad_page);
    apply_style(*separator_end);
    // apps_grid->foreach(sigc::mem_fun(*this,&MyDock::apply_style));

    // Set dock position
    // The dock can be positioned at the left, right and the bottom of the window
    // auto dock_pos = prefs_win.get_dock_pos();
    auto dock_pos = DockPos::POS_LEFT;
    switch (dock_pos)
    {
    case DockPos::POS_LEFT:
        // Set the default size of scrolled window
        icons_sw->set_size_request(52, 340);
        icons_sw->set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
        dock_left->append(*dock_box);
        break;
    case DockPos::POS_RIGHT:
        // Set the default size of scrolled window
        icons_sw->set_size_request(52, 340);
        icons_sw->set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
        dock_right->append(*dock_box);
        break;
    case DockPos::POS_BOTTOM:
        // Change Orientation before pack
        dock_box->set_orientation(Gtk::Orientation::HORIZONTAL);
        icons_box->set_orientation(Gtk::Orientation::HORIZONTAL);
        // Set the default size of scrolled window
        icons_sw->set_size_request(340, 52);
        icons_sw->set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::NEVER);
        // Pack dock to the position
        dock_bottom->append(*dock_box);
        break;
    default:
        dock_right->append(*dock_box);
    }

    // Set Dock or panel mode
    switch (mode1)
    {
    case DockMode::MODE_DOCK:
        dock_box->set_vexpand(false);
        // std::cout << "dock mode" << std::endl;
        break;
    case DockMode::MODE_PANEL:
        if(dock_pos == DockPos::POS_BOTTOM){
            dock_box->set_hexpand();
            dock_box->set_halign(Gtk::Align::FILL);
            icons_sw->set_hexpand();
        }else{
            dock_box->set_vexpand();
            dock_box->set_valign(Gtk::Align::FILL);
            icons_sw->set_vexpand();
        }
        // std::cout << "panel mode" << std::endl;
        break;
    }

    // show_all_children();
}

// void MyDock::set_dock_mode(DockMode mode){
//     switch(mode){
//         case DockMode::MODE_DOCK:
//             set_vexpand(false);
//             break;
//         case DockMode::MODE_PANEL:
//             set_vexpand();
//             set_valign(Gtk::Align::FILL);
//             break;
//     }
// }

// Set the style of dock widget
void MyDock::apply_style(Gtk::Widget &widget)
{
    auto style = widget.get_style_context();
    style->add_provider(provider, G_MAXUINT);
}

// Launchpad
void MyDock::btnlaunch_clicked()
{
    if (launchpad_shown)
    {
        launchpad_stack->set_visible_child(*default_page);
        launchpad_shown = false;
    }
    else
    {
        launchpad_stack->set_visible_child(*launchpad_page);
        launchpad_shown = true;
    }
}

void MyDock::mydock_init(Gtk::Window *window, Gtk::Image *background1)
{
    // Initalize Preference window
    // prefs_win.set_background(background1);
    // prefs_win.set_transient_for(*window);
    parent_win = window;
}

/*
Functions for execute outside apps
padxx_clicked() for click in launchpad
The first three functions for use in linux
and the next three functions for windows
*/

void MyDock::padaud_clicked()
{
    // Start audacious app
    std::thread first(system, "audacious");
    first.detach();
    btnlaunch_clicked();
}

void MyDock::padgedit_clicked()
{
    // Start gedit app
    std::thread second(system, "gedit");
    second.detach();
    btnlaunch_clicked();
}

void MyDock::padvlc_clicked()
{
    // Start vlc app
    std::thread third(system, "vlc");
    third.detach();
    btnlaunch_clicked();
}

void MyDock::padvlc_win32_clicked()
{
    // Start vlc on windows
    std::thread fourth(system, "start ..\\vlc\\vlc.exe");
    fourth.detach();
    btnlaunch_clicked();
}

void MyDock::padnote_clicked()
{
    // Start Notepad on windows
    std::thread fifth(system, "start notepad");
    fifth.detach();
    btnlaunch_clicked();
}

// bool MyDock::prefs_win_closed(GdkEventAny *event)
// {
//     // Handle the "closed" signal of preference window
//     btnset->set_image_from_icon_name("my_prefs", Gtk::ICON_SIZE_DIALOG);
//     prefs_win.hide();
//     return true;
// }

// void MyDock::btnset_clicked()
// {
//     // Show settings window (preferences)
//     btnset->set_image_from_icon_name("my_prefs_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(prefs_win);
// }

// void MyDock::padset_clicked()
// {
//     // Function for button on launchpad
//     btnset->set_image_from_icon_name("my_prefs_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(prefs_win, false);
//     btnlaunch_clicked();
// }

// // Signal Handlers for drawing app window

// bool MyDock::draw_win_closed(GdkEventAny *event)
// {
//     btndraw->set_image_from_icon_name("drawing_app", Gtk::ICON_SIZE_DIALOG);
//     draw_app.hide();
//     return true;
// }

// void MyDock::draw_win_hide()
// {
//     btndraw->set_image_from_icon_name("drawing_app", Gtk::ICON_SIZE_DIALOG);
//     draw_app.hide();
// }

// void MyDock::btndraw_clicked()
// {
//     btndraw->set_image_from_icon_name("drawing_app_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(draw_app);
// }

// void MyDock::paddraw_clicked()
// {
//     btndraw->set_image_from_icon_name("drawing_app_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(draw_app, false);
//     btnlaunch_clicked();
// }

// // Signal Handlers for file window

// bool MyDock::file_win_closed(GdkEventAny *event)
// {
//     btnfiles->set_image_from_icon_name("file-app", Gtk::ICON_SIZE_DIALOG);
//     file_app.hide();
//     return true;
// }

// void MyDock::btnfile_clicked()
// {
//     btnfiles->set_image_from_icon_name("file-app_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(file_app);
// }

// void MyDock::padfile_clicked()
// {
//     btnfiles->set_image_from_icon_name("file-app_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(file_app, false);
//     btnlaunch_clicked();
// }

// Signal Handlers for game window

// bool MyDock::game_win_closed(GdkEventAny *event)
// {
//     btngame->set_image_from_icon_name("game", Gtk::ICON_SIZE_DIALOG);
//     game_win->hide();
//     return true;
// }

// void MyDock::game_win_hide()
// {
//     btngame->set_image_from_icon_name("game", Gtk::ICON_SIZE_DIALOG);
//     game_win->hide();
// }

// void MyDock::btngame_clicked()
// {
//     btngame->set_image_from_icon_name("game_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(*game_win);
// }

// void MyDock::padgame_clicked()
// {
//     btngame->set_image_from_icon_name("game_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(*game_win, false);
//     btnlaunch_clicked();
// }

// Signal Handlers for game24 window

// bool MyDock::game24_win_closed(GdkEventAny *event)
// {
//     btngame24->set_image_from_icon_name("24game", Gtk::ICON_SIZE_DIALOG);
//     game24_win->hide();
//     return true;
// }

// void MyDock::btngame24_clicked()
// {
//     btngame24->set_image_from_icon_name("24game_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(*game24_win);
// }

// void MyDock::padgame24_clicked()
// {
//     btngame24->set_image_from_icon_name("24game_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(*game24_win, false);
//     btnlaunch_clicked();
// }

// void MyDock::game24_win_hide()
// {
//     btngame24->set_image_from_icon_name("24game", Gtk::ICON_SIZE_DIALOG);
//     game24_win->hide();
// }

// Signal Handlers for Calc App

// bool MyDock::calc_win_closed(GdkEventAny *event)
// {
//     btncalc->set_image_from_icon_name("calcapp", Gtk::ICON_SIZE_DIALOG);
//     calc_win->hide();
//     return true;
// }

// void MyDock::btncalc_clicked()
// {
//     btncalc->set_image_from_icon_name("calcapp_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(*calc_win);
// }

// void MyDock::padcalc_clicked()
// {
//     btncalc->set_image_from_icon_name("calcapp_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(*calc_win);
//     btnlaunch_clicked();
// }

// void MyDock::calc_win_hide()
// {
//     btncalc->set_image_from_icon_name("calcapp", Gtk::ICON_SIZE_DIALOG);
//     calc_win->hide();
// }

// Signal Handlers for image app window

// bool MyDock::image_win_closed(GdkEventAny *event)
// {
//     btnimage->set_image_from_icon_name("image_app", Gtk::ICON_SIZE_DIALOG);
//     image_win.hide();
//     return true;
// }

// void MyDock::btnimage_clicked()
// {
//     btnimage->set_image_from_icon_name("image_app_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(image_win);
// }

// void MyDock::padimage_clicked()
// {
//     btnimage->set_image_from_icon_name("image_app_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(image_win, false);
//     btnlaunch_clicked();
// }

// // Signal Handlers for editor window

// bool MyDock::editor_win_closed(GdkEventAny *event)
// {
//     btnedit->set_image_from_icon_name("my_textedit", Gtk::ICON_SIZE_DIALOG);
//     editor_win.hide();
//     return true;
// }

// void MyDock::btnedit_clicked()
// {
//     btnedit->set_image_from_icon_name("my_textedit_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(editor_win);
// }

// void MyDock::padedit_clicked()
// {
//     btnedit->set_image_from_icon_name("my_textedit_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(editor_win, false);
//     btnlaunch_clicked();
// }

// Signal Handler for minesweeper window

// bool MyDock::mine_win_closed(GdkEventAny *event)
// {
//     btnmine->set_image_from_icon_name("mines_app", Gtk::ICON_SIZE_DIALOG);
//     mine_win.hide();
//     return true;
// }

// void MyDock::btnmine_clicked()
// {
//     btnmine->set_image_from_icon_name("mines_app_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(mine_win);
// }

// void MyDock::padmine_clicked()
// {
//     btnmine->set_image_from_icon_name("mines_app_running", Gtk::ICON_SIZE_DIALOG);
//     window_ctrl(mine_win, false);
//     btnlaunch_clicked();
// }

// Signal Handler for run window

// void MyDock::btnrun_clicked()
// {
//     runner1.show();
//     btnlaunch_clicked();
// }

// void MyDock::window_ctrl(Gtk::Window &window, bool on_dock)
// {
//     // Get the GdkWindow object to get the state of a window
//     auto gdk_win = window.get_window();
//     if (gdk_win)
//     {
//         /*
//         The State of GdkWindow (GdkSurface for Gtk4)
//         Gdk::WINDOW_STATE_WITHDRAWN: The window is not shown
//         Gdk::WINDOW_STATE_ICONIFIED: The window is minimized
//         the default mode for first launch
//         */
//         auto state = gdk_win->get_state();
//         switch (state)
//         {
//         case Gdk::WINDOW_STATE_WITHDRAWN:
//             window.present();
//             break;
//         case Gdk::WINDOW_STATE_ICONIFIED:
//             window.set_transient_for(*parent_win);
//             window.deiconify();
//             break;
//         default:
//             if (on_dock)
//             {
//                 window.unset_transient_for();
//                 window.iconify();
//                 break;
//             }
//         }
//     }
//     else
//     {
//         window.set_transient_for(*parent_win);
//         window.present();
//     }
// }

MyDock *MyDock::create(DockMode mode)
{
    MyDock *dock;

    mode1 = mode;
    // Get Widget
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/mydock.ui");
    dock = Gtk::Builder::get_widget_derived<MyDock>(builder, "main_box");

    // switch (mode)
    // {
    // case DockMode::MODE_DOCK:
    //     std::cout << "dock mode" << std::endl;
    //     break;
    // case DockMode::MODE_PANEL:
    //     std::cout << "panel mode" << std::endl;
    //     break;
    // default:
    //     std::cout << "undefined" << std::endl;
    //     break;
    // }

    return dock;
}
