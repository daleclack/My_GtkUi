#include "MyPrefs.hh"
#include "MainWin.hh"
#include "../tomlplusplus/toml.hpp"
#include "image_types.hh"
#include <iostream>

#define INTERNAL_IMAGE_COUNT 6

MyPrefs::MyPrefs(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade)
    : Glib::ObjectBase("MyPrefs"),
      Gtk::Window(cobject),
      ref_builder(refGlade)
{
    // Set window titlebar
    set_title("Preferences");
    header.set_ctrl_window(this);
    // header.set_title_bar(*this);

    // Get widgets from builder
    back_stack = ref_builder->get_widget<Gtk::Stack>("back_stack");
    back_switcher = ref_builder->get_widget<Gtk::StackSwitcher>("back_switcher");
    switch_dark = ref_builder->get_widget<Gtk::Switch>("switch_dark");
    back_frame = ref_builder->get_widget<Gtk::Frame>("back_frame");
    btn_add = ref_builder->get_widget<Gtk::Button>("btn_add");
    btn_remove = ref_builder->get_widget<Gtk::Button>("btn_remove");
    btn_removeall = ref_builder->get_widget<Gtk::Button>("btn_removeall");
    scale_dash = ref_builder->get_widget<Gtk::Scale>("scale_dash");
    scale_finder = ref_builder->get_widget<Gtk::Scale>("scale_finder");
    combo_box = ref_builder->get_widget<Gtk::Box>("combo_box");
    dpi_box = ref_builder->get_widget<Gtk::Box>("dpi_box");
    radio_default = ref_builder->get_widget<Gtk::CheckButton>("radio_default");
    radio_custom = ref_builder->get_widget<Gtk::CheckButton>("radio_custom");
    spin_width = ref_builder->get_widget<Gtk::SpinButton>("spin_width");
    spin_height = ref_builder->get_widget<Gtk::SpinButton>("spin_height");
    btnapply = ref_builder->get_widget<Gtk::Button>("btnapply");
    btnGet = ref_builder->get_widget<Gtk::Button>("btnGet");

    // Add default images list
    images_store = Gtk::StringList::create();
    images_store->append("/org/gtk/daleclack/c182rg_1.png");
    images_store->append("/org/gtk/daleclack/c182rg_2.png");
    images_store->append("/org/gtk/daleclack/final_approach.png");
    images_store->append("/org/gtk/daleclack/img7.png");
    images_store->append("/org/gtk/daleclack/shizuku.png");
    images_store->append("/org/gtk/daleclack/winpe.png");

    // Setup the images grid view
    images_selection = Gtk::SingleSelection::create(images_store);
    images_factory = Gtk::SignalListItemFactory::create();
    images_factory->signal_setup().connect(sigc::mem_fun(*this, &MyPrefs::images_view_setup));
    images_factory->signal_bind().connect(sigc::mem_fun(*this, &MyPrefs::images_view_bind));
    images_view.set_model(images_selection);
    images_view.set_factory(images_factory);
    images_view.set_max_columns(4);

    // Add view to the frame
    images_scroll.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    images_scroll.set_child(images_view);
    back_frame->set_child(images_scroll);

    back_switcher->set_stack(*back_stack);

    // Create size config widget
    size_store = Gtk::StringList::create();
    char buf[20];
    for (int i = 0; i < 13; i++)
    {
        snprintf(buf, sizeof(buf) - 1, "%dx%d", width_values[i], height_values[i]);
        size_store->append(std::string(buf));
    }
    dropdown_size.set_model(size_store);
    combo_box->append(dropdown_size);

    // Add GtkAdjustments for spin buttons
    adj_height = Gtk::Adjustment::create(576, 360, 9999);
    adj_width = Gtk::Adjustment::create(1024, 640, 9999);
    spin_width->set_adjustment(adj_width);
    spin_height->set_adjustment(adj_height);

    // Create dropdown for dpi selection
    dpi_store = Gtk::StringList::create();
    for (int i = 0; i < 7; i++)
    {
        dpi_store->append(dpi_labels[i]);
    }
    dropdown_dpi.set_model(dpi_store);
    dpi_box->append(dropdown_dpi);

    // Bind properties
    Glib::Binding::bind_property(radio_default->property_active(), combo_box->property_sensitive());
    Glib::Binding::bind_property(radio_custom->property_active(), spin_width->property_sensitive());
    Glib::Binding::bind_property(radio_custom->property_active(), spin_height->property_sensitive());

    // Connect signals
    btn_add->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnadd_clicked));
    btn_remove->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnremove_clicked));
    btn_removeall->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnremoveall_clicked));
    btnapply->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::config_save));
    switch_dark->signal_state_set().connect(sigc::mem_fun(*this, &MyPrefs::switch_state_changed), false);
}

void MyPrefs::config_load()
{
    // Load toml file
    int selected = 0;
    std::fstream infile;
    infile.open("config.toml", std::ios_base::in);
    if (infile.is_open())
    {
        auto toml = toml::parse(infile);

        // Get config values
        auto wallpapers = toml["main_config"]["file_paths"].as_array();
        selected = toml["main_config"]["selected_image"].value_or(0);
        auto icon_size = toml["main_config"]["icon_size"].as_integer()->get();
        auto size_selected = toml["main_config"]["size_selected"].as_integer()->get();
        bool dark_mode = toml["main_config"]["dark_mode"].as_boolean()->get();
        int dpi_set = toml["main_config"]["dpi_set"].as_integer()->get();
        auto finder_size = toml["main_config"]["finder_size"].as_integer()->get();
        if (size_selected < 0)
        {
            radio_custom->set_active();
            width = toml["main_config"]["width"].as_integer()->get();
            height = toml["main_config"]["height"].as_integer()->get();
        }
        else
        {
            radio_default->set_active();
            dropdown_size.set_selected(size_selected);
            width = width_values[size_selected];
            height = height_values[size_selected];
        }

        // Load dark mode config
        switch_dark->set_active(dark_mode);

        // Load all custom images paths
        for (int i = 0; i < wallpapers->size(); ++i)
        {
            auto string = wallpapers->at(i).as_string()->get();
            images_store->append(string);
        }

        // Load current selected image
        images_selection->set_selected(selected);

        // Load icon size
        scale_dash->set_value((double)icon_size);
        scale_finder->set_value((double)finder_size);

        // Update DPI Settings
        dropdown_dpi.set_selected(dpi_set);
    }
    else
    {
        // Default selection
        images_selection->set_selected(0);
        scale_dash->set_value(48);
        radio_default->set_active();
        dropdown_size.set_selected(3);
        switch_dark->set_active(false);
        dropdown_dpi.set_selected(0);
        scale_finder->set_value(16);
    }

    // Update image
    if (selected < INTERNAL_IMAGE_COUNT)
    {
        auto res_name = images_store->get_string(selected);
        background_widget->set_resource(res_name);
    }
    else
    {
        auto path = images_store->get_string(selected);
        background_widget->set_filename(path);
    }

    infile.close();
}

void MyPrefs::config_save()
{
    // Create toml object
    static constexpr std::string_view source = R"()";
    auto toml = toml::parse(source);

    // // Get dark mode config
    // bool dark_mode = switch_dark->get_active();

    // Get all custom images paths
    toml::array file_paths;
    for (int i = INTERNAL_IMAGE_COUNT; i < images_store->get_n_items(); ++i)
    {
        file_paths.push_back(std::string(images_store->get_string(i)));
    }

    // Get current selected image
    guint selected = images_selection->get_selected();

    // Get icon size config
    int icon_size = (int)(scale_dash->get_value());
    int finder_size = (int)(scale_finder->get_value());

    // Get size config
    int size_selected = -1;
    if (radio_default->get_active())
    {
        size_selected = dropdown_size.get_selected();
        width = width_values[size_selected];
        height = height_values[size_selected];
    }
    else
    {
        width = spin_width->get_value();
        height = spin_height->get_value();
        size_selected = -1;
    }

    // Get DPI Config
    int dpi_selected = dropdown_dpi.get_selected();

    // Add a toml table for main configuration
    toml::table main_cfg;
    main_cfg.insert_or_assign("file_paths", file_paths);
    main_cfg.insert_or_assign("finder_size", finder_size);
    main_cfg.insert_or_assign("selected_image", selected);
    main_cfg.insert_or_assign("icon_size", icon_size);
    main_cfg.insert_or_assign("size_selected", size_selected);
    main_cfg.insert_or_assign("width", width);
    main_cfg.insert_or_assign("height", height);
    main_cfg.insert_or_assign("dark_mode", dark_mode);
    main_cfg.insert_or_assign("dpi_set", dpi_selected);
    toml.insert_or_assign("main_config", main_cfg);

    // Save the data to file
    std::fstream outfile;
    outfile.open("config.toml", std::ios_base::out);
    if (outfile.is_open())
    {
        outfile << toml;
    }
    outfile.close();
}

bool MyPrefs::switch_state_changed(bool state)
{
    // Update dark mode config
    dark_mode = state;
    config_save();
    switch_dark->set_state(dark_mode);
    return true;
}

void MyPrefs::images_view_setup(const Glib::RefPtr<Gtk::ListItem> &item)
{
    auto image_btn = Gtk::make_managed<PrefsBtn>();
    item->set_child(*image_btn);
}

void MyPrefs::images_view_bind(const Glib::RefPtr<Gtk::ListItem> &item)
{
    // Get position
    auto position = item->get_position();

    // Get the image path
    auto path = images_store->get_string(position);
    auto image_btn = dynamic_cast<PrefsBtn *>(item->get_child());
    if (position < INTERNAL_IMAGE_COUNT)
    {
        image_btn->set_image_from_resource(path);
    }
    else
    {
        image_btn->set_image_from_file(path);
    }
    image_btn->signal_clicked().connect(sigc::bind(
        sigc::mem_fun(*this, &MyPrefs::image_btn_clicked), image_btn));
    image_btn->set_image_id(position);
}

// Initialize background widget, set the picture and size of the widget
void MyPrefs::background_widget_init(Gtk::Picture *picture)
{
    background_widget = picture;

    // Load config
    config_load();

    // Update window size
    picture->set_size_request(width, height);
}

void MyPrefs::image_btn_clicked(PrefsBtn *btn)
{
    // Get image from the button
    auto paintable = btn->get_paintable();
    background_widget->set_paintable(paintable);

    // Update selection of image
    images_selection->set_selected(btn->get_image_id());

    // Update config
    config_save();
}

void MyPrefs::btnadd_clicked()
{
    // Open a file chooser to open image
    auto dialog = Gtk::FileDialog::create();

    // Add filter
    auto filters = Gio::ListStore<Gtk::FileFilter>::create();
    auto filter_image = Gtk::FileFilter::create();
    for (auto &ext : supported_globs)
    {
        filter_image->add_pattern(ext);
    }
    filter_image->set_name("Image Files");
    filters->append(filter_image);

    // Add filter for all files
    auto filter_all = Gtk::FileFilter::create();
    filter_all->add_pattern("*");
    filter_all->set_name("All Files");
    filters->append(filter_all);
    dialog->set_filters(filters);

    dialog->open(sigc::bind(
        sigc::mem_fun(*this, &MyPrefs::file_dialog_finish), dialog));
}

void MyPrefs::file_dialog_finish(const Glib::RefPtr<Gio::AsyncResult> &result,
                                 const Glib::RefPtr<Gtk::FileDialog> &file_dialog)
{
    // Get the file path
    try
    {
        auto file = file_dialog->open_finish(result);
        auto path = file->get_path();
        images_store->append(path);
        config_save();
    }
    // If no file is selected, catch the exception and output information
    catch (const Gtk::DialogError &ex)
    {
        std::cout << "No file selected." << "\n";
    }
    // Other exceptions
    catch (const Glib::Error &err)
    {
        std::cout << "Unexpected exception. " << err.what() << std::endl;
    }
}

// Remove selected image expect internal images
void MyPrefs::btnremove_clicked()
{
    auto selected = images_selection->get_selected();
    if (selected < INTERNAL_IMAGE_COUNT)
        return;

    images_store->remove(selected);
}

// Remove all user images
void MyPrefs::btnremoveall_clicked()
{
    for (auto i = INTERNAL_IMAGE_COUNT - 1; i < images_store->get_n_items(); ++i)
    {
        images_store->remove(i);
    }
}

void MyPrefs::set_icon_callback(pfun icon_callback)
{
    // This signal should be connected after the widget initialized
    icon_size_callback = icon_callback;
    scale_dash->signal_value_changed().connect(sigc::mem_fun(*this, &MyPrefs::scale_dash_changed));
}

void MyPrefs::scale_dash_changed()
{
    config_load();
    // Update config when icon size changed
    config_save();

    // Update icon size
    icon_size_callback((guint)scale_dash->get_value());
}

void MyPrefs::set_finder_callback(pfun finder_callback)
{
    // This signal should be connected after the widget initialized
    finder_size_callback = finder_callback;
    scale_finder->signal_value_changed().connect(sigc::mem_fun(*this, &MyPrefs::scale_finder_changed));
}

void MyPrefs::scale_finder_changed()
{
    config_load();
    // Update config when icon size changed
    config_save();

    // Update icon size
    finder_size_callback((guint)scale_finder->get_value());
}

MyPrefs *MyPrefs::create(Gtk::Window &parent)
{
    // Create a builder and get window from builder
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/cambalache/background.ui");
    MyPrefs *prefs_win = Gtk::Builder::get_widget_derived<MyPrefs>(builder, "prefs_win");

    // Make the window transient for main window
    prefs_win->set_transient_for(parent);
    return prefs_win;
}
