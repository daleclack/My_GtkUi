#include "winconf.hh"
#include <cstdio>

WinConf::WinConf(Gtk::Window *window){
    confwin=window;
    conf_builder=Gtk::Builder::create_from_resource("/GtkUI/win_size.ui");
    conf_builder->get_widget("")
}

void get_config(int *width,int *height){
    FILE *fp=fopen("winsize.conf","rt+");
    if(fp){
        fscanf(fp,"%d%d",width,height);
    }
}
