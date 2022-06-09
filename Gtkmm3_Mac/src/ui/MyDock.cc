#include "MyDock.hh"
#include <thread>

MyDock::MyDock(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_Glade)
    : Gtk::Box(cobject),
      ref_builder(ref_Glade),
      launchpad_shown(false)
{
    // Get Widgets
    ref_builder->get_widget("finder_box", finder_box);
    ref_builder->get_widget("dock_box", dock_box);
    ref_builder->get_widget("btnlaunch", btnlaunch);
    ref_builder->get_widget("btndraw", btndraw);
    ref_builder->get_widget("btnfiles", btnfiles);
    ref_builder->get_widget("btngame", btngame);
    ref_builder->get_widget("btnedit", btnedit);
    ref_builder->get_widget("btnimage", btnimage);
    ref_builder->get_widget("btnset", btnset);
    ref_builder->get_widget("btngame24",btngame24);
    ref_builder->get_widget("btncalc",btncalc);
    ref_builder->get_widget("separator_start", separator_start);
    ref_builder->get_widget("separator_end", separator_end);
    ref_builder->get_widget("launchpad_stack", launchpad_stack);
    ref_builder->get_widget("default_page", default_page);
    ref_builder->get_widget("launchpad_page", launchpad_page);
    ref_builder->get_widget("apps_grid",apps_grid);
    ref_builder->get_widget("padaud", padaud);
    ref_builder->get_widget("padgedit", padgedit);
    ref_builder->get_widget("padvlc", padvlc);
    ref_builder->get_widget("padnote", padnote);
    ref_builder->get_widget("padvlc_win32", padvlc_win32);
    ref_builder->get_widget("padset", padset);
    ref_builder->get_widget("paddraw", paddraw);
    ref_builder->get_widget("padfile", padfile);
    ref_builder->get_widget("paddraw", paddraw);
    ref_builder->get_widget("padgame", padgame);
    ref_builder->get_widget("padimage", padimage);
    ref_builder->get_widget("padedit", padedit);
    ref_builder->get_widget("padrun", padrun);
    ref_builder->get_widget("padgame24",padgame24);
    ref_builder->get_widget("padcalc",padcalc);

    // Create window
    game_win = Game::create();
    game24_win = Game24Win::create();
    calc_win = CalcApp::create();

    // Link signals
    btnlaunch->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btnlaunch_clicked));
    padaud->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padaud_clicked));
    padgedit->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padgedit_clicked));
    padvlc->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padvlc_clicked));
    padnote->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padnote_clicked));
    padvlc_win32->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padvlc_win32_clicked));
    padrun->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btnrun_clicked));

    btnset->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btnset_clicked));
    padset->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padset_clicked));
    prefs_win.signal_delete_event().connect(sigc::mem_fun(*this, &MyDock::prefs_win_closed));

    btndraw->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btndraw_clicked));
    paddraw->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::paddraw_clicked));
    draw_app.signal_delete_event().connect(sigc::mem_fun(*this, &MyDock::draw_win_closed));
    draw_app.signal_hide().connect(sigc::mem_fun(*this, &MyDock::draw_win_hide));

    btnfiles->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btnfile_clicked));
    padfile->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padfile_clicked));
    file_app.signal_delete_event().connect(sigc::mem_fun(*this, &MyDock::file_win_closed));

    btngame->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btngame_clicked));
    padgame->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padgame_clicked));
    game_win->signal_delete_event().connect(sigc::mem_fun(*this, &MyDock::game_win_closed));
    game_win->signal_hide().connect(sigc::mem_fun(*this, &MyDock::game_win_hide));

    btngame24->signal_clicked().connect(sigc::mem_fun(*this,&MyDock::btngame24_clicked));
    padgame24->signal_clicked().connect(sigc::mem_fun(*this,&MyDock::padgame24_clicked));
    game24_win->signal_delete_event().connect(sigc::mem_fun(*this,&MyDock::game24_win_closed));
    game24_win->signal_hide().connect(sigc::mem_fun(*this,&MyDock::game24_win_hide));

    btncalc->signal_clicked().connect(sigc::mem_fun(*this,&MyDock::btncalc_clicked));
    padcalc->signal_clicked().connect(sigc::mem_fun(*this,&MyDock::padcalc_clicked));
    calc_win->signal_delete_event().connect(sigc::mem_fun(*this,&MyDock::calc_win_closed));
    calc_win->signal_hide().connect(sigc::mem_fun(*this,&MyDock::calc_win_hide));

    btnimage->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btnimage_clicked));
    padimage->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padimage_clicked));
    image_win.signal_delete_event().connect(sigc::mem_fun(*this, &MyDock::image_win_closed));

    btnedit->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::btnedit_clicked));
    padedit->signal_clicked().connect(sigc::mem_fun(*this, &MyDock::padedit_clicked));
    editor_win.signal_delete_event().connect(sigc::mem_fun(*this, &MyDock::editor_win_closed));

    // Add Finder
    finder_box->pack_start(finder);

    // Add Style for MyFinder
    provider = Gtk::CssProvider::create();
    provider->load_from_resource("/org/gtk/daleclack/dock_style.css");
    apply_style(*dock_box);
    apply_style(*launchpad_page);
    apply_style(*separator_end);
    //apps_grid->foreach(sigc::mem_fun(*this,&MyDock::apply_style));

    show_all_children();
}

void MyDock::set_dock_mode(DockMode mode){
    switch(mode){
        case DockMode::MODE_DOCK:
            set_vexpand(false);
            break;
        case DockMode::MODE_PANEL:
            set_vexpand();
            set_valign(Gtk::ALIGN_FILL);
            break;
    }
}

void MyDock::apply_style(Gtk::Widget &widget){
    auto style = widget.get_style_context();
    style->add_provider(provider,G_MAXUINT);
}

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
    prefs_win.set_background(background1);
    prefs_win.set_transient_for(*window);
    parent_win = window;
}

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
    std::thread fourth(system, "start notepad");
    fourth.detach();
    btnlaunch_clicked();
}

void MyDock::padnote_clicked()
{
    // Start Notepad on windows
    std::thread fifth(system, "start ..\\vlc\\vlc.exe");
    fifth.detach();
    btnlaunch_clicked();
}

bool MyDock::prefs_win_closed(GdkEventAny *event)
{
    // Handle the "closed" signal of preference window
    btnset->set_image_from_icon_name("my_prefs", Gtk::ICON_SIZE_DIALOG);
    prefs_win.hide();
    return true;
}

void MyDock::btnset_clicked()
{
    // Show settings window (preferences)
    btnset->set_image_from_icon_name("my_prefs_running", Gtk::ICON_SIZE_DIALOG);
    window_ctrl(prefs_win);
}

void MyDock::padset_clicked()
{
    // Function for button on launchpad
    btnset->set_image_from_icon_name("my_prefs_running", Gtk::ICON_SIZE_DIALOG);
    window_ctrl(prefs_win, false);
    btnlaunch_clicked();
}

//Signal Handlers for drawing app window

bool MyDock::draw_win_closed(GdkEventAny *event)
{
    btndraw->set_image_from_icon_name("drawing_app", Gtk::ICON_SIZE_DIALOG);
    draw_app.hide();
    return true;
}

void MyDock::draw_win_hide(){
    btndraw->set_image_from_icon_name("drawing_app", Gtk::ICON_SIZE_DIALOG);
    draw_app.hide();
}

void MyDock::btndraw_clicked()
{
    btndraw->set_image_from_icon_name("drawing_app_running", Gtk::ICON_SIZE_DIALOG);
    window_ctrl(draw_app);
}

void MyDock::paddraw_clicked()
{
    btndraw->set_image_from_icon_name("drawing_app_running", Gtk::ICON_SIZE_DIALOG);
    window_ctrl(draw_app, false);
    btnlaunch_clicked();
}

// Signal Handlers for file window

bool MyDock::file_win_closed(GdkEventAny *event)
{
    btnfiles->set_image_from_icon_name("file-app", Gtk::ICON_SIZE_DIALOG);
    file_app.hide();
    return true;
}

void MyDock::btnfile_clicked()
{
    btnfiles->set_image_from_icon_name("file-app_running", Gtk::ICON_SIZE_DIALOG);
    window_ctrl(file_app);
}

void MyDock::padfile_clicked()
{
    btnfiles->set_image_from_icon_name("file-app_running", Gtk::ICON_SIZE_DIALOG);
    window_ctrl(file_app, false);
    btnlaunch_clicked();
}

// Signal Handlers for game window

bool MyDock::game_win_closed(GdkEventAny *event)
{
    btngame->set_image_from_icon_name("game", Gtk::ICON_SIZE_DIALOG);
    game_win->hide();
    return true;
}

void MyDock::game_win_hide()
{
    btngame->set_image_from_icon_name("game", Gtk::ICON_SIZE_DIALOG);
    game_win->hide();
}

void MyDock::btngame_clicked()
{
    btngame->set_image_from_icon_name("game_running", Gtk::ICON_SIZE_DIALOG);
    window_ctrl(*game_win);
}

void MyDock::padgame_clicked()
{
    btngame->set_image_from_icon_name("game_running", Gtk::ICON_SIZE_DIALOG);
    window_ctrl(*game_win, false);
    btnlaunch_clicked();
}

// Signal Handlers for game24 window

bool MyDock::game24_win_closed(GdkEventAny *event){
    btngame24->set_image_from_icon_name("24game",Gtk::ICON_SIZE_DIALOG);
    game24_win->hide();
    return true;
}

void MyDock::btngame24_clicked(){
    btngame24->set_image_from_icon_name("24game_running",Gtk::ICON_SIZE_DIALOG);
    window_ctrl(*game24_win);
}

void MyDock::padgame24_clicked(){
    btngame24->set_image_from_icon_name("24game_running",Gtk::ICON_SIZE_DIALOG);
    window_ctrl(*game24_win,false);
    btnlaunch_clicked();
}

void MyDock::game24_win_hide(){
    btngame24->set_image_from_icon_name("24game",Gtk::ICON_SIZE_DIALOG);
    game24_win->hide();
}

// Signal Handlers for Calc App

bool MyDock::calc_win_closed(GdkEventAny *event){
    btncalc->set_image_from_icon_name("calcapp",Gtk::ICON_SIZE_DIALOG);
    calc_win->hide();
    return true;
}

void MyDock::btncalc_clicked(){
    btncalc->set_image_from_icon_name("calcapp_running",Gtk::ICON_SIZE_DIALOG);
    window_ctrl(*calc_win);
}

void MyDock::padcalc_clicked(){
    btncalc->set_image_from_icon_name("calcapp_running",Gtk::ICON_SIZE_DIALOG);
    window_ctrl(*calc_win);
    btnlaunch_clicked();
}

void MyDock::calc_win_hide(){
    btncalc->set_image_from_icon_name("calcapp",Gtk::ICON_SIZE_DIALOG);
    calc_win->hide();
}

// Signal Handlers for image app window

bool MyDock::image_win_closed(GdkEventAny *event)
{
    btnimage->set_image_from_icon_name("image_app", Gtk::ICON_SIZE_DIALOG);
    image_win.hide();
    return true;
}

void MyDock::btnimage_clicked()
{
    btnimage->set_image_from_icon_name("image_app_running", Gtk::ICON_SIZE_DIALOG);
    window_ctrl(image_win);
}

void MyDock::padimage_clicked()
{
    btnimage->set_image_from_icon_name("image_app_running", Gtk::ICON_SIZE_DIALOG);
    window_ctrl(image_win, false);
    btnlaunch_clicked();
}

// Signal Handlers for editor window

bool MyDock::editor_win_closed(GdkEventAny *event)
{
    btnedit->set_image_from_icon_name("my_textedit", Gtk::ICON_SIZE_DIALOG);
    editor_win.hide();
    return true;
}

void MyDock::btnedit_clicked()
{
    btnedit->set_image_from_icon_name("my_textedit_running", Gtk::ICON_SIZE_DIALOG);
    window_ctrl(editor_win);
}

void MyDock::padedit_clicked()
{
    btnedit->set_image_from_icon_name("my_textedit_running", Gtk::ICON_SIZE_DIALOG);
    window_ctrl(editor_win, false);
    btnlaunch_clicked();
}

// Signal Handler for run window

void MyDock::btnrun_clicked()
{
    runner1.show();
    btnlaunch_clicked();
}

void MyDock::window_ctrl(Gtk::Window &window, bool on_dock)
{
    auto gdk_win = window.get_window();
    if (gdk_win)
    {
        auto state = gdk_win->get_state();
        switch (state)
        {
        case Gdk::WINDOW_STATE_WITHDRAWN:
            window.present();
            break;
        case Gdk::WINDOW_STATE_ICONIFIED:
            window.set_transient_for(*parent_win);
            window.deiconify();
            break;
        default:
            if (on_dock)
            {
                window.unset_transient_for();
                window.iconify();
                break;
            }
        }
    }
    else
    {
        window.set_transient_for(*parent_win);
        window.present();
    }
}

MyDock *MyDock::create()
{
    MyDock *dock;

    // Get Widget
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/mydock.ui");
    builder->get_widget_derived("main_box", dock);

    return dock;
}
