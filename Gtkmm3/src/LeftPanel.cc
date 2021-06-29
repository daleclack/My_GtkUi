#include "LeftPanel.hh"

LeftPanel::LeftPanel(Gtk::Window *parent1){
    panel_builder=Gtk::Builder::create_from_resource("/GtkUI/leftpanel.ui");
    panel_builder->get_widget("btnbox",btnbox);
    parent=parent1;
}

void LeftPanel::add_panel(Gtk::Overlay &overlay){
    btnbox->set_halign(Gtk::ALIGN_START);
    btnbox->set_valign(Gtk::ALIGN_START);
    overlay.add_overlay(*btnbox);
}
