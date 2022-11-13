#pragma once

#include <gtkmm.h>
#include <string>
#include <fstream>
#include "../json_nlohmann/json.hpp"

using json = nlohmann::json;

enum class DockMode{
    MODE_DOCK,
    MODE_PANEL
};

enum class DockPos{
    POS_LEFT,
    POS_RIGHT,
    POS_BOTTOM
};

class MyPrefs : public Gtk::Window
{
public:
    MyPrefs();
    void set_background(Gtk::Image *back);
    void load_winsize_config();
    void update_background_size();

protected:
    class ModelColumns : public Gtk::TreeModelColumnRecord
    {
    public:
        ModelColumns()
        {
            add(m_col_pixbuf);
            add(m_col_path);
            add(m_col_name);
            add(m_col_internal);
        }
        Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> m_col_pixbuf;
        Gtk::TreeModelColumn<std::string> m_col_path;
        Gtk::TreeModelColumn<Glib::ustring> m_col_name;
        Gtk::TreeModelColumn<bool> m_col_internal;
    };

    ModelColumns n_columns;
    Glib::RefPtr<Gtk::ListStore> folders_store, images_store;
    Glib::RefPtr<Gtk::TreeSelection> folder_selection, image_selection;
    bool has_selection;

private:
    // Background widget and properties
    int width, height;
    bool panel_mode;
    DockPos dock_pos;
    Gtk::Image *background1;
    std::string path;
    bool background_internal;

    //Page switcher and another page
    Glib::RefPtr<Gtk::Builder> stackbuilder;
    Gtk::Box *stack_box, *back_page, *winsize_page;
    Gtk::RadioButton *radio_default, *radio_custom;
    Gtk::ComboBoxText *combo_default;
    Gtk::SpinButton *spin_width, *spin_height;
    Gtk::Button *btnapply, *btnGet;
    Gtk::Label *label_size;

    // Dock Preferences
    Gtk::Button *btnapply1;
    Gtk::CheckButton *mode_check;
    Gtk::RadioButton *radio_left, *radio_right, *radio_bottom;

    // Child Widgets
    Gtk::TreeView folders_view, images_view;
    Gtk::ScrolledWindow sw_folders, sw_images;
    Gtk::Box main_box, views_box, btnbox;
    Gtk::Button btnadd, btnremove;

    // Folder Open Dialog
    Glib::RefPtr<Gdk::Pixbuf> folder_pixbuf, image_pixbuf, imagefile_pixbuf;
    Glib::RefPtr<Gtk::FileChooserNative> dialog;
    void dialog_response(int response_id);

    // Sort for ListStore
    int sort_func(const Gtk::TreeModel::iterator &a, const Gtk::TreeModel::iterator &b);
    bool icasecompare(const std::string &a, const std::string &b);

    // Signal Handlers
    void save_config_file();

    // Background page buttons
    void btnadd_clicked();
    void btnremove_clicked();

    // Folder and images view
    void folders_view_changed();
    void images_view_changed();

    // Default settings
    void default_folders_view();

    // Update the background
    void update_images_view(std::string &folder_path);
    void set_background_internal(const char *const *data);
    void set_background_file();

    // Resoulution page
    void radiobutton_toggled();
    void btnapply_clicked();
    void btnGet_clicked();

    // Dock Preferences
    void btnapply1_clicked();
};

//Read Config from file without use the MyPrefs class
static inline void get_size_config(int &width, int &height, bool &panel_mode){
    std::ifstream jsonfile("config.json");
    if(jsonfile.is_open()){
        json data = json::parse(jsonfile);
        height = data["height"];
        width = data["width"];
        panel_mode = data["panel_mode"];
    }else{
        height = 720;
        width = 1280;
        panel_mode = false;
    }
}
