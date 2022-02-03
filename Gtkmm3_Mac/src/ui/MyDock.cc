#include "MyDock.hh"

MyDock::MyDock(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> ref_Glade)
:Gtk::Box(cobject),
ref_builder(ref_Glade)
{
    ref_builder->get_widget("finder_box",finder_box);
    finder_box->pack_start(finder);
    show_all_children();
}

MyDock * MyDock::create(){
    MyDock * dock;

    // Get Widget
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/mydock.ui");
    builder->get_widget_derived("main_box",dock);

    return dock;
}
