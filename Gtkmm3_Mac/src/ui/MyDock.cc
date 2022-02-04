#include "MyDock.hh"

MyDock::MyDock(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_Glade)
    : Gtk::Box(cobject),
      ref_builder(ref_Glade)
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

    //Link signals
    btnset->signal_clicked().connect(sigc::mem_fun(*this,&MyDock::btnset_clicked));
    finder_box->pack_start(finder);

    // Add Style for MyFinder
    provider = Gtk::CssProvider::create();
    provider->load_from_resource("/org/gtk/daleclack/dock_style.css");
    auto style = dock_box->get_style_context();
    style->add_provider(provider, G_MAXUINT);

    show_all_children();
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
