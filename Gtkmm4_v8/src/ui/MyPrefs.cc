#include "MyPrefs.hh"
#include "MainWin.hh"

MyPrefs::MyPrefs(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade)
    : Glib::ObjectBase("MyPrefs"), 
    Gtk::Window(cobject),
    ref_builder(refGlade)
{
    // Set window titlebar
    header.set_ctrl_window(this);
    header.set_title_bar(*this);

    // Get widgets from builder
    back_stack = ref_builder->get_widget<Gtk::Stack>("back_stack");
    back_switcher = ref_builder->get_widget<Gtk::StackSwitcher>("back_switcher");

    back_switcher->set_stack(*back_stack);
}

bool MyPrefs::on_close_request()
{
    // Hide window when closing the window
    set_visible(false);
    return true;
}

MyPrefs *MyPrefs::create(Gtk::Window &parent)
{
    // Create a builder and get window from builder
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/cambalache/background.ui");
    MyPrefs* prefs_win = Gtk::Builder::get_widget_derived<MyPrefs>(builder, "prefs_win");

    // Make the window transient for main window
    prefs_win->set_transient_for(parent);
    // prefs_win->main_win = win; // Save main window to control
    return prefs_win;
}