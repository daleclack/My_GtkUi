#include "winconf.hh"
#include <cstdio>

WinConf::WinConf(Gtk::Window *window){
    //Ininalize the Config Dialog
    confwin=window;
    conf_builder=Gtk::Builder::create_from_resource("/GtkUI/win_size.ui");
    conf_builder->get_widget("dialog",dialog);
    conf_builder->get_widget("width_spin",width_spin);
    conf_builder->get_widget("height_spin",height_spin);
    conf_builder->get_widget("btnGet",btnget);
    conf_builder->get_widget("btn_default",btn_default);
    dialog->set_transient_for(*window);
    btnget->signal_clicked().connect(sigc::mem_fun(*this,&WinConf::get_size));
    dialog->signal_response().connect(sigc::mem_fun(*this,&WinConf::dialog_response));
    btn_default->signal_clicked().connect(sigc::mem_fun(*this,&WinConf::default_size));
}

void WinConf::get_size(){
    //Get Current Size
    confwin->get_size(width,height);
    width_spin->set_value(width);
    height_spin->set_value(height);
}

void WinConf::default_size(){
    //Reset the size to default
    width_spin->set_value(800);
    height_spin->set_value(450);
}

void WinConf::show_dialog(){
    dialog->show_all();
}

void WinConf::dialog_response(int response){
    if(response==Gtk::RESPONSE_OK){
        //Get Config in Dialog
        width=width_spin->get_value_as_int();
        height=height_spin->get_value_as_int();
        //Write the Config to a file
        FILE *fp=fopen("winsize.conf","wt+");
        if(fp){
            fprintf(fp,"%d %d",width,height);
            fclose(fp);
        }
    }
    dialog->hide();
}

void WinConf::get_config(int *width1,int *height1){
    //Get Config from a file named "winsize.conf"
    FILE *fp=fopen("winsize.conf","rt+");
    if(fp){
        fscanf(fp,"%d%d",&width,&height);
    }
    confwin->set_default_size(width,height);
    *width1=width;
    *height1=height;
}
