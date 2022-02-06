#include "MyDock.hh"
#include <thread>

MyDock::MyDock(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_Glade)
    : Gtk::Box(cobject),
      ref_builder(ref_Glade),
      launchpad_shown(false)
{
    // Get Widget
    ref_builder->get_widget("finder_box", finder_box);
    ref_builder->get_widget("dock_box", dock_box);
    ref_builder->get_widget("btnlaunch",btnlaunch);
    ref_builder->get_widget("btndraw",btndraw);
    ref_builder->get_widget("btnfiles",btnfiles);
    ref_builder->get_widget("btngame",btngame);
    ref_builder->get_widget("btnedit",btnedit);
    ref_builder->get_widget("btnimage",btnimage);
    ref_builder->get_widget("btnset",btnset);
    ref_builder->get_widget("launchpad_stack",launchpad_stack);
    ref_builder->get_widget("default_page",default_page);
    ref_builder->get_widget("launchpad_page",launchpad_page);
    ref_builder->get_widget("btnabout",btnabout);
    ref_builder->get_widget("btnaud",btnaud);

    // Add Buttons for launchpad
    // launchpad_page->attach(*btnabout,0,0);
    // launchpad_page->attach(*btnaud,1,0);
    // label1.set_label(" ");
    // label1.set_hexpand();
    // label1.set_vexpand();
    // launchpad_page->attach(label1,2,1);

    //Link signals
    btnset->signal_clicked().connect(sigc::mem_fun(*this,&MyDock::btnset_clicked));
    btnlaunch->signal_clicked().connect(sigc::mem_fun(*this,&MyDock::btnlaunch_clicked));
    finder_box->pack_start(finder);

    // Add Style for MyFinder
    provider = Gtk::CssProvider::create();
    provider->load_from_resource("/org/gtk/daleclack/dock_style.css");
    auto style = dock_box->get_style_context();
    style->add_provider(provider, G_MAXUINT);
    auto style1 = launchpad_page->get_style_context();
    style1->add_provider(provider,G_MAXUINT);

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

void MyDock::btnset_clicked()
{
    prefs_win.show_all();
}

MyDock *MyDock::create()
{
    MyDock *dock;

    // Get Widget
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/mydock.ui");
    builder->get_widget_derived("main_box", dock);

    return dock;
}
