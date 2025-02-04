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
    header.set_title_bar(*this);

    // Get widgets from builder
    back_stack = ref_builder->get_widget<Gtk::Stack>("back_stack");
    back_switcher = ref_builder->get_widget<Gtk::StackSwitcher>("back_switcher");
    switch_dark = ref_builder->get_widget<Gtk::Switch>("switch_dark");
    back_frame = ref_builder->get_widget<Gtk::Frame>("back_frame");
    btn_add = ref_builder->get_widget<Gtk::Button>("btn_add");
    btn_remove = ref_builder->get_widget<Gtk::Button>("btn_remove");
    btn_removeall = ref_builder->get_widget<Gtk::Button>("btn_removeall");

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

    // Connect signals
    btn_add->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnadd_clicked));
    btn_remove->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnremove_clicked));
    btn_removeall->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnremoveall_clicked));
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

void MyPrefs::set_background_widget(Gtk::Picture *picture)
{
    background_widget = picture;
}

void MyPrefs::image_btn_clicked(PrefsBtn *btn)
{
    // Get image from the button
    auto paintable = btn->get_paintable();
    background_widget->set_paintable(paintable);

    // Update selection of image
    images_selection->set_selected(btn->get_image_id());
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

MyPrefs *MyPrefs::create(Gtk::Window &parent)
{
    // Create a builder and get window from builder
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/cambalache/background.ui");
    MyPrefs *prefs_win = Gtk::Builder::get_widget_derived<MyPrefs>(builder, "prefs_win");

    // Make the window transient for main window
    prefs_win->set_transient_for(parent);
    return prefs_win;
}
