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
    ref_builder->get_widget("btnlaunch",btnlaunch);
    ref_builder->get_widget("btndraw",btndraw);
    ref_builder->get_widget("btnfiles",btnfiles);
    ref_builder->get_widget("btngame",btngame);
    ref_builder->get_widget("btnedit",btnedit);
    ref_builder->get_widget("btnimage",btnimage);
    ref_builder->get_widget("btnset",btnset);
    ref_builder->get_widget("separator_start",separator_start);
    ref_builder->get_widget("separator_end",separator_end);
    ref_builder->get_widget("launchpad_stack",launchpad_stack);
    ref_builder->get_widget("default_page",default_page);
    ref_builder->get_widget("launchpad_page",launchpad_page);
    ref_builder->get_widget("padaud",padaud);
    ref_builder->get_widget("padgedit",padgedit);
    ref_builder->get_widget("padvlc",padvlc);
    ref_builder->get_widget("padnote",padnote);
    ref_builder->get_widget("padvlc_win32",padvlc_win32);
    ref_builder->get_widget("padset",padset);

    // Link signals
    btnlaunch->signal_clicked().connect(sigc::mem_fun(*this,&MyDock::btnlaunch_clicked));
    padaud->signal_clicked().connect(sigc::mem_fun(*this,&MyDock::padaud_clicked));
    padgedit->signal_clicked().connect(sigc::mem_fun(*this,&MyDock::padgedit_clicked));
    padvlc->signal_clicked().connect(sigc::mem_fun(*this,&MyDock::padvlc_clicked));
    padnote->signal_clicked().connect(sigc::mem_fun(*this,&MyDock::padnote_clicked));
    padvlc_win32->signal_clicked().connect(sigc::mem_fun(*this,&MyDock::padvlc_win32_clicked));
    btnset->signal_clicked().connect(sigc::mem_fun(*this,&MyDock::btnset_clicked));
    padset->signal_clicked().connect(sigc::mem_fun(*this,&MyDock::padset_clicked));
    prefs_win.signal_delete_event().connect(sigc::mem_fun(*this,&MyDock::prefs_win_closed));
    finder_box->pack_start(finder);

    // Add Style for MyFinder
    provider = Gtk::CssProvider::create();
    provider->load_from_resource("/org/gtk/daleclack/dock_style.css");
    auto style = dock_box->get_style_context();
    style->add_provider(provider, G_MAXUINT);
    auto style1 = launchpad_page->get_style_context();
    style1->add_provider(provider,G_MAXUINT);
    auto style2 = separator_end->get_style_context();
    style2->add_provider(provider,G_MAXUINT);

    show_all_children();
}

void MyDock::btnlaunch_clicked(){
    if(launchpad_shown){
        launchpad_stack->set_visible_child(*default_page);
        launchpad_shown = false;
    }else{
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

void MyDock::padaud_clicked(){
    std::thread first(system,"audacious");
    first.detach();
    btnlaunch_clicked();
}

void MyDock::padgedit_clicked(){
    std::thread second(system,"gedit");
    second.detach();
    btnlaunch_clicked();
}

void MyDock::padvlc_clicked(){
    std::thread third(system,"vlc");
    third.detach();
    btnlaunch_clicked();
}

void MyDock::padvlc_win32_clicked(){
    std::thread fourth(system,"start notepad");
    fourth.detach();
    btnlaunch_clicked();
}

void MyDock::padnote_clicked(){
    std::thread fifth(system,"start ..\\vlc\\vlc.exe");
    fifth.detach();
    btnlaunch_clicked();
}

bool MyDock::prefs_win_closed(GdkEventAny * event){
    btnset->set_image_from_icon_name("my_prefs",Gtk::ICON_SIZE_DIALOG);
    prefs_win.hide();
    return true;
}

void MyDock::btnset_clicked()
{
    btnset->set_image_from_icon_name("my_prefs_running",Gtk::ICON_SIZE_DIALOG);
    window_ctrl(prefs_win);
}

void MyDock::padset_clicked(){
    btnset->set_image_from_icon_name("my_prefs_running",Gtk::ICON_SIZE_DIALOG);
    window_ctrl(prefs_win,false);
}

void MyDock::window_ctrl(Gtk::Window &window, bool on_dock){
    auto gdk_win = window.get_window();
    if(gdk_win){
        auto state = gdk_win->get_state();
        switch(state){
            case Gdk::WINDOW_STATE_WITHDRAWN:
                window.show_all();
                break;
            case Gdk::WINDOW_STATE_ICONIFIED:
                window.set_transient_for(*parent_win);
                window.deiconify();
                break;
            default:
                if(on_dock){
                    window.unset_transient_for();
                    window.iconify();
                    break;
                }        
        }
    }else{
        window.show_all();
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
