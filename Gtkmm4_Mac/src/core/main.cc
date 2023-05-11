#include "MyWin.hh"

int main(int argc,char ** argv){
    // Create a application and window, start the application
    auto app = Gtk::Application::create("org.gtk.daleclack");
    // MyWin window;
    return app->make_window_and_run<MyWin>(argc, argv);
}