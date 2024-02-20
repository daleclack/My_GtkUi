#include "MyPrefs.hh"
#include "winpe.xpm"
#include "img7.xpm"
#include "image_types.hh"
#include <iostream>
#include <fstream>

MyPrefs::MyPrefs()
    : main_box(Gtk::Orientation::VERTICAL, 10),
      views_box(Gtk::Orientation::HORIZONTAL, 5),
      btnbox(Gtk::Orientation::HORIZONTAL, 5),
      width(1024),
      height(576),
      dock_pos(DockPos::POS_LEFT)
{
    /*Step 1: Initalize widget that without Gtk::Builder*/

    // Initalize Window
    set_title("Window Preferences");
    set_default_size(800, 450);

    // Load config file
    load_winsize_config();

    // // Initalize Stores
    // folders_store = Gtk::ListStore::create(n_columns);
    // folders_store->set_default_sort_func(sigc::mem_fun(*this, &MyPrefs::sort_func));
    // folders_store->set_sort_column(-1, Gtk::SORT_ASCENDING);
    // folders_view.set_model(folders_store);
    // folder_selection = folders_view.get_selection();
    // folder_selection->signal_changed().connect(sigc::mem_fun(*this, &MyPrefs::folders_view_changed));

    // images_store = Gtk::ListStore::create(n_columns);
    // images_store->set_default_sort_func(sigc::mem_fun(*this, &MyPrefs::sort_func));
    // images_store->set_sort_column(-1, Gtk::SORT_ASCENDING);
    // images_view.set_model(images_store);
    // image_selection = images_view.get_selection();
    // image_selection->signal_changed().connect(sigc::mem_fun(*this, &MyPrefs::images_view_changed));

    // Load Pixbufs
    auto pixbuf = Gdk::Pixbuf::create_from_resource("/org/gtk/daleclack/folder.svg");
    folder_pixbuf = pixbuf->scale_simple(24, 24, Gdk::InterpType::BILINEAR);
    pixbuf.reset();
    pixbuf = Gdk::Pixbuf::create_from_resource("/org/gtk/daleclack/folder-images.svg");
    image_pixbuf = pixbuf->scale_simple(24, 24, Gdk::InterpType::BILINEAR);
    pixbuf.reset();
    pixbuf = Gdk::Pixbuf::create_from_resource("/org/gtk/daleclack/image_file.svg");
    imagefile_pixbuf = pixbuf->scale_simple(24, 24, Gdk::InterpType::BILINEAR);
    pixbuf.reset();

    // // Add Default Value for folders view
    // auto row = *(folders_store->append());
    // row[n_columns.m_col_path] = "";
    // row[n_columns.m_col_name] = "Default Backgrounds";
    // row[n_columns.m_col_pixbuf] = folder_pixbuf;
    // row[n_columns.m_col_internal] = true;

    // row = *(folders_store->append());
    // row[n_columns.m_col_path] = Glib::get_home_dir();
    // row[n_columns.m_col_name] = "User's Home";
    // row[n_columns.m_col_pixbuf] = folder_pixbuf;
    // row[n_columns.m_col_internal] = false;

    // row = *(folders_store->append());
    // row[n_columns.m_col_path] = Glib::get_user_special_dir(Glib::USER_DIRECTORY_PICTURES);
    // row[n_columns.m_col_name] = "User's Pictures Directory";
    // row[n_columns.m_col_pixbuf] = image_pixbuf;
    // row[n_columns.m_col_internal] = false;

    // // Append folders from config file
    // if(!back_list.empty()){
    //     for(auto iter = back_list.begin(); iter != back_list.end(); iter++){
    //         // Get pair value from vector
    //         std::pair<std::string, std::string> temp;
    //         temp = *iter;

    //         // Append folder to the folders store
    //         auto row = *(folders_store->append());
    //         row[n_columns.m_col_path] = temp.second;
    //         row[n_columns.m_col_name] = temp.first;
    //         row[n_columns.m_col_pixbuf] = folder_pixbuf;
    //         row[n_columns.m_col_internal] = false;
    //     }
    // }

    // // Append Column for the folders view
    // folders_view.append_column(" ", n_columns.m_col_pixbuf);
    // folders_view.append_column("Name", n_columns.m_col_name);

    // // Default Value for images view
    // default_folders_view();
    // images_view.append_column(" ", n_columns.m_col_pixbuf);
    // images_view.append_column("Images", n_columns.m_col_name);

    // Add Views
    main_box.append(views_box);
    sw_folders.set_child(folders_view);
    views_box.append(sw_folders);
    sw_images.set_child(images_view);
    views_box.append(sw_images);

    // Allow Selection
    has_selection = true;

    // Add Control Buttons
    btnadd.set_image_from_icon_name("list-add");
    btnadd.signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnadd_clicked));
    btnremove.set_image_from_icon_name("list-remove");
    btnremove.signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnremove_clicked));
    btnbox.append(btnadd);
    btnbox.append(btnremove);
    main_box.append(btnbox);

    // Add Main Box to window
    const int margin_value = 15;
    main_box.set_hexpand();
    main_box.set_vexpand();
    main_box.set_margin_top(margin_value);
    main_box.set_margin_bottom(margin_value);
    main_box.set_margin_start(margin_value);
    main_box.set_margin_end(margin_value);

    /*Step 2: Initalize widgets from glade xml file ('.ui' file)*/

    // Get Widgets for multi pages
    stackbuilder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/prefs_stack.ui");
    stack_box = stackbuilder->get_widget<Gtk::Box>("stack_box");
    back_page = stackbuilder->get_widget<Gtk::Box>("back_page");
    combo_box = stackbuilder->get_widget<Gtk::Box>("combo_box");
    radio_default = stackbuilder->get_widget<Gtk::CheckButton>("radio_default");
    radio_custom = stackbuilder->get_widget<Gtk::CheckButton>("radio_custom");
    spin_width = stackbuilder->get_widget<Gtk::SpinButton>("spin_width");
    spin_height = stackbuilder->get_widget<Gtk::SpinButton>("spin_height");
    btnapply = stackbuilder->get_widget<Gtk::Button>("btnapply");
    btnGet = stackbuilder->get_widget<Gtk::Button>("btnGet");
    label_size = stackbuilder->get_widget<Gtk::Label>("label_size");
    mode_check = stackbuilder->get_widget<Gtk::CheckButton>("mode_check");
    radio_left = stackbuilder->get_widget<Gtk::CheckButton>("radio_left");
    radio_right = stackbuilder->get_widget<Gtk::CheckButton>("radio_right");
    radio_bottom = stackbuilder->get_widget<Gtk::CheckButton>("radio_bottom");
    btnapply1 = stackbuilder->get_widget<Gtk::Button>("btnapply1");

    // Create Dropdown selecter
    sizes_list = stackbuilder->get_object<Gtk::StringList>("sizes_list");
    combo_default.set_model(sizes_list);
    combo_default.set_selected(2);
    combo_box->append(combo_default);

    // Initalize radio buttons
    radio_default->set_active();
    radio_default->signal_toggled().connect(sigc::mem_fun(*this, &MyPrefs::radiobutton_toggled));
    radio_custom->signal_toggled().connect(sigc::mem_fun(*this, &MyPrefs::radiobutton_toggled));

    // Initalize other widgets
    btnapply->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnapply_clicked));
    btnGet->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnGet_clicked));

    // Initalize Dock Preferences
    btnapply1->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnapply1_clicked));

    // Initalize Label
    char *size_str = g_strdup_printf("Current Config: %d x %d", width, height);
    label_size->set_label(size_str);
    g_free(size_str);

    // Change the default status of widgets
    mode_check->set_active(panel_mode);
    switch (dock_pos)
    {
    case DockPos::POS_LEFT:
        radio_left->set_active();
        break;
    case DockPos::POS_RIGHT:
        radio_right->set_active();
        break;
    case DockPos::POS_BOTTOM:
        radio_bottom->set_active();
        break;
    }

    back_page->append(main_box);
    set_child(*stack_box);
    // show_all_children();
}

void MyPrefs::btnadd_clicked()
{
    // Create a dialog
    dialog = Gtk::FileChooserNative::create("Add a folder", *this,
                                            Gtk::FileChooser::Action::SELECT_FOLDER, "OK", "Cancel");

    dialog->signal_response().connect(sigc::mem_fun(*this, &MyPrefs::dialog_response));

    dialog->show();
}

void MyPrefs::dialog_response(int response_id)
{
    if (response_id == Gtk::ResponseType::ACCEPT)
    {
        // Get File Basename and Path
        auto file = dialog->get_file();
        auto basename = file->get_basename();
        auto filename = file->get_path();

        // // Add to list
        // auto row = *(folders_store->append());
        // row[n_columns.m_col_path] = filename;
        // row[n_columns.m_col_name] = basename;
        // row[n_columns.m_col_pixbuf] = folder_pixbuf;

        // Add for json file
        save_config_file();

        file.reset();
    }
    dialog.reset();
}

void MyPrefs::btnremove_clicked()
{
    // // Get the selection and remove the selected item
    // auto iter = folder_selection->get_selected();
    // auto row = *iter;

    // // The internal item should not be deleted
    // if (iter && !(row[n_columns.m_col_internal]))
    // {
    //     folders_store->erase(iter);
    // }
}

void MyPrefs::folders_view_changed()
{
    // // Get Selected Folder
    // auto row = *(folder_selection->get_selected());
    // if (row[n_columns.m_col_internal])
    // {
    //     // The default folder
    //     default_folders_view();
    // }
    // else
    // {
    //     // User defined folder
    //     std::string path = row[n_columns.m_col_path];
    //     update_images_view(path);
    // }
}

void MyPrefs::default_folders_view()
{
    // // Clear the store
    // images_store->clear();

    // // Add Default values
    // auto row = *(images_store->append());
    // row[n_columns.m_col_path] = ":1";
    // row[n_columns.m_col_name] = "winpe.xpm";
    // row[n_columns.m_col_internal] = true;
    // row[n_columns.m_col_pixbuf] = imagefile_pixbuf;

    // row = *(images_store->append());
    // row[n_columns.m_col_path] = ":2";
    // row[n_columns.m_col_name] = "img7.xpm";
    // row[n_columns.m_col_internal] = true;
    // row[n_columns.m_col_pixbuf] = imagefile_pixbuf;

    // // Default path
    // path = ":1";
}

int MyPrefs::sort_func(const Gtk::TreeModel::iterator &a, const Gtk::TreeModel::iterator &b)
{
    // Glib::ustring name_a, name_b;

    // // Get file names
    // auto row_a = *(a);
    // auto row_b = *(b);
    // name_a = row_a[n_columns.m_col_name];
    // name_b = row_b[n_columns.m_col_name];

    // // Proform sort process
    // if (name_a[0] != '.' && name_b[0] == '.')
    // {
    //     return 1;
    // }
    // if (name_a[0] == '.' && name_b[0] != '.')
    // {
    //     return -1;
    // }
    // else
    // {
    //     return g_utf8_collate(name_a.c_str(), name_b.c_str());
    // }
    return -1;
}

bool MyPrefs::icasecompare(const std::string &a, const std::string &b)
{
    if (a.length() == b.length())
    {
        return std::equal(a.begin(), a.end(), b.begin(),
                          [](char a, char b)
                          {
                              return tolower(a) == tolower(b);
                          });
    }
    else
    {
        return false;
    }
}

void MyPrefs::update_images_view(std::string &folder_path)
{
    // // Unselect everything
    // has_selection = false;

    // // Clear the store
    // images_store->clear();

    // // Add Files into store
    // try
    // {
    //     Glib::Dir dir1(folder_path);
    //     bool is_dir, file_valid;
    //     Glib::ustring display_name;
    //     std::string filename, pathname;

    //     do
    //     {
    //         // Get File Name
    //         filename = dir1.read_name();

    //         // Get Path for a file
    //         pathname = Glib::build_filename(folder_path, filename);
    //         is_dir = Glib::file_test(pathname, Glib::FILE_TEST_IS_DIR);
    //         display_name = Glib::filename_to_utf8(filename);

    //         // Filter the file
    //         // Get Pattern of the file
    //         file_valid = false;
    //         size_t pos = filename.find_last_of('.');
    //         if (pos != std::string::npos)
    //         {
    //             std::string pattern = "*" + filename.substr(pos);
    //             for (int i = 0; supported_globs[i] != NULL; i++)
    //             {
    //                 std::string glob = std::string(supported_globs[i]);
    //                 if (icasecompare(glob, pattern))
    //                 {
    //                     file_valid = true;
    //                     break;
    //                 }
    //             }
    //         }

    //         if (!is_dir && file_valid)
    //         {
    //             // Add item to store
    //             auto row = *(images_store->append());
    //             row[n_columns.m_col_path] = pathname;
    //             row[n_columns.m_col_name] = display_name;
    //             row[n_columns.m_col_internal] = false;
    //             row[n_columns.m_col_pixbuf] = imagefile_pixbuf;
    //         }
    //     } while (filename != "");

    //     has_selection = true;
    // }
    // catch (Glib::Error &ex)
    // {
    //     std::cout << ex.what() << std::endl;
    // }
}

void MyPrefs::images_view_changed()
{
    // Set the background as selected
    // if (has_selection)
    // {
    //     auto row = *(image_selection->get_selected());
    //     if (row[n_columns.m_col_internal])
    //     {
    //         path = row[n_columns.m_col_path];
    //         switch (path[1])
    //         {
    //         case '1':
    //             // path = "default_1";
    //             set_background_internal(winpe);
    //             break;
    //         case '2':
    //             // path = "default_2";
    //             set_background_internal(img7);
    //             break;
    //         }
    //     }
    //     else
    //     {
    //         path = row[n_columns.m_col_path];
    //         set_background_file();
    //     }
    //     save_config_file();
    // }
}

void MyPrefs::set_background_internal(const char *const *data)
{
    // Set a internal background
    auto pixbuf = Gdk::Pixbuf::create_from_xpm_data(data);
    auto sized = pixbuf->scale_simple(width, height, Gdk::InterpType::BILINEAR);
    // gtk_image_set_from_pixbuf(background1->gobj(), sized->gobj());
    background1->set_pixbuf(sized);
    pixbuf.reset();
    sized.reset();
    background_internal = true;
}

void MyPrefs::set_background_file()
{
    // Set Background from a file
    try{
        auto pixbuf = Gdk::Pixbuf::create_from_file(path);
        auto sized = pixbuf->scale_simple(width, height, Gdk::InterpType::BILINEAR);
        // gtk_image_set_from_pixbuf(background1->gobj(), sized->gobj());
        background1->set_pixbuf(sized);
        pixbuf.reset();
        sized.reset();
        background_internal = false;
    }
    // When error occurs, set default background
    catch(Glib::Error &ex){
        set_background_internal(winpe);
    }
}

void MyPrefs::update_background_size()
{
}

void MyPrefs::set_background(Gtk::Picture *back)
{
    // Link background widget to the class
    background1 = back;
    // std::cout << path << std::endl;

    // Set the background from a file or xpm data
    switch (path[1])
    {
        case '1':
            // path = "default_1";
            set_background_internal(winpe);
            break;
        case '2':
            // path = "default_2";
            set_background_internal(img7);
            break;
        default:
            set_background_file();
    }
}

void MyPrefs::radiobutton_toggled()
{
    // Change sensitive state of buttons
    bool mode = radio_default->get_active();
    combo_default.set_sensitive(mode);
    spin_height->set_sensitive(!mode);
    spin_width->set_sensitive(!mode);
    btnGet->set_sensitive(!mode);
}

void MyPrefs::save_config_file()
{
    // // Refresh the data of background folders list
    // back_list.clear();

    // // Initalize data from store of folders
    // auto child = folders_store->children();
    // for (auto iter = child.begin(); iter != child.end(); iter++)
    // {
    //     auto row = *iter;
    //     // Save custom directories of background
    //     // The default directories will be ignored
    //     if (!row[n_columns.m_col_internal] 
    //         && row.get_value(n_columns.m_col_name) != "User's Pictures Directory" 
    //         && row.get_value(n_columns.m_col_name) != "User's Home")
    //     {
    //         // Use a pair to save the basename and path
    //         std::pair<std::string, std::string> temp;
    //         temp.first = row[n_columns.m_col_name];
    //         temp.second = row[n_columns.m_col_path];
    //         back_list.push_back(temp);
    //     }
    // }

    // Open the file for configs
    json data = json::parse(R"(
        {
            "background": "",
            "background_folders": [],
            "height": 720,
            "panel_mode": false,
            "position": 0,
            "width": 1280
        }
    )");
    std::fstream outfile;
    outfile.open("config.json", std::ios_base::out);
    if (outfile.is_open())
    {
        data["width"] = width;
        data["height"] = height;
        data["panel_mode"] = panel_mode;
        data["position"] = dock_pos;
        data["background"] = path;
        data["background_folders"] = back_list;
        outfile << data;
        outfile.close();
    }
}

void MyPrefs::btnapply_clicked()
{
    // Get Config
    if (radio_default->get_active())
    {
        int mode = combo_default.get_selected();
        switch (mode)
        {
        case 0:
            width = 640;
            height = 360;
            break;
        case 1:
            width = 800;
            height = 576;
            break;
        case 2:
            width = 1024;
            height = 576;
            break;
        case 3:
            width = 1280;
            height = 720;
            break;
        case 4:
            width = 1366;
            height = 768;
            break;
        }
    }
    else
    {
        width = spin_width->get_value_as_int();
        height = spin_height->get_value_as_int();
    }

    // Save the config to json file
    save_config_file();
}

void MyPrefs::btnapply1_clicked()
{
    // Whether panel mode is enabled
    panel_mode = mode_check->get_active();

    // The dock position
    if (radio_left->get_active())
    {
        dock_pos = DockPos::POS_LEFT;
    }
    if (radio_right->get_active())
    {
        dock_pos = DockPos::POS_RIGHT;
    }
    if (radio_bottom->get_active())
    {
        dock_pos = DockPos::POS_BOTTOM;
    }

    // std::cout << dock_pos << std::endl;
    save_config_file();
}

void MyPrefs::btnGet_clicked()
{
    // Get Current Window Size
    width = background1->get_width();
    height = background1->get_height();
    spin_width->set_value(width);
    spin_height->set_value(height);
}

DockPos MyPrefs::get_dock_pos()
{
    // Get config for dock position
    return dock_pos;
}

void MyPrefs::load_winsize_config()
{
    // Read the config
    std::ifstream jsonfile("config.json");
    if (jsonfile.is_open())
    {
        json data = json::parse(jsonfile);
        try
        {
            height = data["height"];
            width = data["width"];
            panel_mode = data["panel_mode"];
            dock_pos = data["position"];
            back_list = data["background_folders"];
            path = data["background"];
        }
        catch (nlohmann::detail::type_error &error)
        {
            // Default config
            height = 720;
            width = 1280;
            panel_mode = false;
            dock_pos = DockPos::POS_LEFT;
            path = ":1";
        }
    }
    else
    {
        // Default config
        height = 720;
        width = 1280;
        panel_mode = false;
        dock_pos = DockPos::POS_LEFT;
        path = ":1";
    }
}
