#include "MyWin.hh"
#include "winpe.xpm"
#include "image_types.hh"

MyWin::MyWin():
main_box(Gtk::ORIENTATION_VERTICAL)
{
    //Initalize Window
    set_icon_name("org.gtk.daleclack");
    set_default_size(800,450);
    overlay.add_overlay(draw_area);
    default_background();
    overlay.add(background);

    //Initalize menu
    menu_builder=Gtk::Builder::create_from_file("../res/appmenu.xml");
    auto object=menu_builder->get_object("app-menu");
    auto gmenu=Glib::RefPtr<Gio::Menu>::cast_dynamic(object);

    //Menu actions
    add_action("quit",sigc::mem_fun(*this,&MyWin::win_quit));
    add_action("default",sigc::mem_fun(*this,&MyWin::default_background));
    add_action("back",sigc::mem_fun(*this,&MyWin::back_dialog));
    add_action("about",sigc::mem_fun(*this,&MyWin::about_dialog));

    //Set Popover Menu
    popover.bind_model(gmenu);
    popover.set_relative_to(draw_area);

    //Set Gesture
    gesture=Gtk::GestureMultiPress::create(overlay);
    gesture->set_button(GDK_BUTTON_SECONDARY);
    gesture->signal_pressed().connect(sigc::mem_fun(*this,&MyWin::btn_pressed));

    //Add Stack
    main_stack.add_stack(main_box);
    
    //Final Initalize
    overlay.add_overlay(main_box);
    add(overlay);
    show_all_children();
}

void MyWin::btn_pressed(int n_press,double x,double y){
    //When mouse pressed,show a menu
    Gdk::Rectangle point{(int)x,(int)y,1,1};
    popover.set_pointing_to(point);
    popover.popup();
}

void MyWin::default_background(){
    //Default background
    Glib::RefPtr<Gdk::Pixbuf> pixbuf=Gdk::Pixbuf::create_from_xpm_data(winpe);
    Glib::RefPtr<Gdk::Pixbuf> sized=pixbuf->scale_simple(800,450,Gdk::INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(background.gobj(),sized->gobj());

    //Free Memory of pixbufs
    pixbuf.reset();
    sized.reset();
}

void MyWin::back_dialog(){
    //Initalize dialog
    dialog=Gtk::FileChooserNative::create("Open a image file",*this,Gtk::FILE_CHOOSER_ACTION_OPEN,
                                          "OK","Cancel");
    dialog->signal_response().connect(sigc::mem_fun(*this,&MyWin::change_background));
    //Add Filter
    auto filter=Gtk::FileFilter::create();
    filter->set_name("Image Files");

    //Windows doesn't support mime types
    if(mime_type_supported()){
        filter->add_mime_type("image/*");
    }else{
        for(int i=0 ; supported_globs[i] != NULL && supported_globs != NULL ; i++){
            Glib::ustring glob = supported_globs[i];
            filter->add_pattern(glob);
        }
    }

    dialog->add_filter(filter);

    auto filter2=Gtk::FileFilter::create();
    filter2->set_name("All Files");
    filter2->add_pattern("*");
    dialog->add_filter(filter2);

    dialog->show();
    filter.reset();
    filter2.reset();
}

void MyWin::change_background(int response){
    Glib::ustring filename;
    if(response==Gtk::RESPONSE_ACCEPT){
        filename=dialog->get_filename();
        //Default background
        Glib::RefPtr<Gdk::Pixbuf> pixbuf=Gdk::Pixbuf::create_from_file(filename.c_str());
        Glib::RefPtr<Gdk::Pixbuf> sized=pixbuf->scale_simple(800,450,Gdk::INTERP_BILINEAR);
        gtk_image_set_from_pixbuf(background.gobj(),sized->gobj());

        //Free Memory of pixbufs
        pixbuf.reset();
        sized.reset();
    }
    dialog.reset();
}

void MyWin::about_dialog(){
    char *version;
    Gtk::AboutDialog about_dialog(true);
    about_dialog.set_transient_for(*this);
    //Create Comments
    version=g_strdup_printf("3.0\nRunning against Gtkmm %d.%d.%d",
                            GTKMM_MAJOR_VERSION,
                            GTKMM_MINOR_VERSION,
                            GTKMM_MICRO_VERSION);
    about_dialog.set_program_name("My Gtk UI");
    about_dialog.set_version(version);
    about_dialog.set_logo_icon_name("org.gtk.daleclack");
    about_dialog.set_copyright("© 2019—2021 The Xe Project");
    about_dialog.set_comments("A test program for menu and background in gtkmm");
    about_dialog.set_title("About Gtk UI");
    about_dialog.run();
    g_free(version);
}

void MyWin::win_quit(){
    hide();
}

MyWin::~MyWin(){
}
