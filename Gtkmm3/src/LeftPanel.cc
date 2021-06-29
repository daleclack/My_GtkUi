#include "LeftPanel.hh"

LeftPanel::LeftPanel(){
    panel_builder=Gtk::Builder::create_from_file("../res/leftpanel.ui");
    panel_builder->get_widget("btnbox",btnbox);
}

void LeftPanel::add_panel(Gtk::Overlay &overlay){
    btnbox->set_halign(Gtk::ALIGN_START);
    btnbox->set_valign(Gtk::ALIGN_START);
    overlay.add_overlay(*btnbox);
}
