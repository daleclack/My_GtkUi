#include "MyDock.hh"

MyDock::MyDock(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> ref_Glade)
:Gtk::Box(cobject),
ref_builder(ref_Glade)
{
    // Get Widget
    ref_builder->get_widget("finder_box",finder_box);
    ref_builder->get_widget("dock_box",dock_box);
    finder_box->pack_start(finder);

    // Add Style for MyFinder
    provider = Gtk::CssProvider::create();
    provider->load_from_resource("/org/gtk/daleclack/style.css");
    auto style = dock_box->get_style_context();
    style->add_provider(provider, G_MAXUINT);

    show_all_children();
}

MyDock * MyDock::create(){
    MyDock * dock;

    // Get Widget
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/mydock.ui");
    builder->get_widget_derived("main_box",dock);

    return dock;
}
