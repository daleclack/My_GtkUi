#include "MyWin.hh"

int main(int argc,char ** argv){
    // Create a application and window, start the application
    auto app = Gtk::Application::create(argc,argv,"org.gtk.daleclack");
    MyWin window;
    return app->run(window);
}