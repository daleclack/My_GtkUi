#include "Runner.hh"
#include <thread>
#include <iostream>

GtkRunner::GtkRunner():
hbox(Gtk::ORIENTATION_HORIZONTAL,5),
open_btn("...")
{
    //Ininalize window
    set_title("Run a Application");
    set_icon_name("myrunner");
    set_default_size(300,150);
    add_button("OK",Gtk::RESPONSE_OK);
    add_button("Cancel",Gtk::RESPONSE_CANCEL);
    set_default_response(Gtk::RESPONSE_OK);

    //Initalize Box
    hbox.set_halign(Gtk::ALIGN_CENTER);
    hbox.set_valign(Gtk::ALIGN_CENTER);
    hbox.pack_start(app_entry,Gtk::PACK_SHRINK);
    hbox.pack_start(open_btn,Gtk::PACK_SHRINK);
    
    //Initalize entry
    app_entry.set_activates_default();
    app_entry.set_text("vlc");
    
    open_btn.signal_clicked().connect(sigc::mem_fun(*this,&GtkRunner::file_dialog));

    //Add Widgets
    auto pbox=get_content_area();
    pbox->pack_start(hbox);

    show_all_children();
}

void GtkRunner::on_response(int response_id){
    if(response_id==Gtk::RESPONSE_OK){
        //Start The Application
        Glib::ustring program=app_entry.get_text();
        std::thread first(system,program.c_str());
        first.detach();
    }
    hide();
}

void GtkRunner::file_dialog(){
    dialog=Gtk::FileChooserNative::create("Open a program",
            Gtk::FILE_CHOOSER_ACTION_OPEN,"OK","Cancel");

    dialog->signal_response().connect(sigc::mem_fun(*this,&GtkRunner::dialog_response));
    
    auto filter_app=Gtk::FileFilter::create();
    filter_app->set_name("Applications");
    filter_app->add_mime_type("application/*");
    dialog->add_filter(filter_app);

    auto filter_any=Gtk::FileFilter::create();
    filter_any->set_name("Any Files");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);

    dialog->show();
}

void GtkRunner::dialog_response(int response){
    if(response==Gtk::RESPONSE_ACCEPT){
        Glib::ustring filename=dialog->get_filename();
        app_entry.set_text(filename);
    }
    dialog.reset();
}
