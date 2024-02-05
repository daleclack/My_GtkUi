#include "MyPrefs.h"
#include "MyItem.h"
#include "winpe.xpm"
#include "img7.xpm"
#include "image_types.h"
#include <string>
#include <vector>
#include <fstream>
#include "../json_nlohmann/json.hpp"

using json = nlohmann::json;

struct _MyPrefs
{
    GtkWindow parent_instance;
    GtkBuilder *prefs_builder;
    GtkWidget *background, *back_page;
    GtkWidget *stack_box, *combo_box;

    // Widget for window size config
    GtkWidget *label_size;
    GtkWidget *sizes_drop;
    GtkWidget *radio_default, *radio_custom;
    GtkWidget *spin_width, *spin_height;
    GtkWidget *btnGet, *btnapply;

    // Widget for dock position and mode
    GtkWidget *mode_check;
    GtkWidget *radio_left, *radio_right, *radio_bottom;
    GtkWidget *btnapply1;

    // Widget for background selection
    GtkWidget *folders_view, *images_view;
    GtkWidget *btns_box, *views_box;
    GtkWidget *folders_sw, *images_sw;
    GtkWidget *btn_add, *btn_remove;

    // List stores
    GListStore *folders_list, *images_list;
    GFile *file;
    GtkDirectoryList *file_list;
    GtkSingleSelection *folders_select, *image_select;

    // Folders view
    GtkColumnViewColumn *folder_image_column;
    GtkColumnViewColumn *folder_string_column;
    GtkListItemFactory *factory_folder_image;
    GtkListItemFactory *factory_folder_string;

    // Images view
    GtkColumnViewColumn *pics_image_column;
    GtkColumnViewColumn *pics_string_column;
    GtkListItemFactory *factory_pics_image;
    GtkListItemFactory *factory_pics_string;

    GdkPixbuf *pixbuf, *sized;                     // Pixbufs
    int width, height;                             // The Size of window
    int current_folder_index, current_image_index; // Index of images
    DockPos curr_dock_pos;                         // Dock Position
    gboolean panel_mode;                           // Whether panel mode is activated
    DockPos dock_pos;                              // Dock Position
};

G_DEFINE_TYPE(MyPrefs, my_prefs, GTK_TYPE_WINDOW)

static void factory_folder_image_setup(GtkListItemFactory *factory, GtkListItem *item)
{
    // Create image
    GtkWidget *image = gtk_image_new();
    gtk_image_set_pixel_size(GTK_IMAGE(image), 16);
    gtk_list_item_set_child(item, image);
}

static void factory_folder_image_bind(GtkListItemFactory *factory, GtkListItem *item)
{
    // Get Image Widget
    GtkWidget *image = gtk_list_item_get_child(item);

    // Get icon_name
    MyItem *item1 = MY_ITEM(gtk_list_item_get_item(item));
    gtk_image_set_from_icon_name(GTK_IMAGE(image), "prefs_folder");
}

static void factory_folder_string_setup(GtkListItemFactory *factory, GtkListItem *item)
{
    // Add a label
    GtkWidget *label = gtk_label_new("");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_list_item_set_child(item, label);
}

static void factory_folder_string_bind(GtkListItemFactory *factory, GtkListItem *item)
{
    // Set names from label
    GtkWidget *label = gtk_list_item_get_child(item);

    // Get display name
    MyItem *item1 = MY_ITEM(gtk_list_item_get_item(item));
    gtk_label_set_label(GTK_LABEL(label),
                        my_item_get_display_name(item1));
}

static void folder_view_init(MyPrefs *self)
{
    // Create store for folders column view
    self->folders_list = g_list_store_new(my_item_get_type());
    g_list_store_append(self->folders_list,
                        my_item_new("Default Backgrounds", ":0", TRUE));
    g_list_store_append(self->folders_list,
                        my_item_new("User's Home directory", g_get_home_dir(), FALSE));
    g_list_store_append(self->folders_list,
                        my_item_new("User's Picture directory",
                                    g_get_user_special_dir(G_USER_DIRECTORY_PICTURES),
                                    FALSE));
    self->folders_select = gtk_single_selection_new(G_LIST_MODEL(self->folders_list));
    gtk_single_selection_set_selected(self->folders_select, 0);

    // Create factories to renderer the list
    self->folders_view = gtk_column_view_new(GTK_SELECTION_MODEL(self->folders_select));

    // Image column
    self->factory_folder_image = gtk_signal_list_item_factory_new();
    g_signal_connect(self->factory_folder_image, "setup",
                     G_CALLBACK(factory_folder_image_setup), NULL);
    g_signal_connect(self->factory_folder_image, "bind",
                     G_CALLBACK(factory_folder_image_bind), NULL);
    self->folder_image_column = gtk_column_view_column_new(" ",
                                                           self->factory_folder_image);
    gtk_column_view_append_column(GTK_COLUMN_VIEW(self->folders_view),
                                  self->folder_image_column);

    // Folder names column
    self->factory_folder_string = gtk_signal_list_item_factory_new();
    g_signal_connect(self->factory_folder_string, "setup",
                     G_CALLBACK(factory_folder_string_setup), NULL);
    g_signal_connect(self->factory_folder_string, "bind",
                     G_CALLBACK(factory_folder_string_bind), NULL);
    self->folder_string_column = gtk_column_view_column_new("Name",
                                                            self->factory_folder_string);
    gtk_column_view_append_column(GTK_COLUMN_VIEW(self->folders_view),
                                  self->folder_string_column);
}

static void factory_pics_image_setup(GtkListItemFactory *factory, GtkListItem *item)
{
    // Create image
    GtkWidget *image = gtk_image_new();
    gtk_image_set_pixel_size(GTK_IMAGE(image), 16);
    gtk_list_item_set_child(item, image);
}

static void factory_pics_image_bind(GtkListItemFactory *factory, GtkListItem *item)
{
    // Get Image Widget
    GtkWidget *image = gtk_list_item_get_child(item);

    // Get icon_name
    MyItem *item1 = MY_ITEM(gtk_list_item_get_item(item));
    gtk_image_set_from_icon_name(GTK_IMAGE(image), "folder-images");
}

static void factory_pics_string_setup(GtkListItemFactory *factory, GtkListItem *item)
{
    // Add a label
    GtkWidget *label = gtk_label_new("");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_list_item_set_child(item, label);
}

static void factory_pics_string_bind(GtkListItemFactory *factory, GtkListItem *item)
{
    // Set names from label
    GtkWidget *label = gtk_list_item_get_child(item);

    // Get display name
    MyItem *item1 = MY_ITEM(gtk_list_item_get_item(item));
    gtk_label_set_label(GTK_LABEL(label),
                        my_item_get_display_name(item1));
}

static void images_list_default(GListStore *store1)
{
    // if the store is not empty, clear it
    if (g_list_model_get_n_items(G_LIST_MODEL(store1)))
    {
        g_list_store_remove_all(store1);
    }

    // Append default items
    g_list_store_append(store1,
                        my_item_new("final_approach.png", ":1", TRUE));
    g_list_store_append(store1,
                        my_item_new("img7.xpm", ":2", TRUE));
    g_list_store_append(store1,
                        my_item_new("winpe.xpm", ":3", TRUE));
}

static void update_images_list(MyPrefs *prefs1)
{
    GtkDirectoryList *dir_list = prefs1->file_list;
    // if the store is not empty, clear it
    if (g_list_model_get_n_items(G_LIST_MODEL(prefs1->images_list)))
    {
        g_list_store_remove_all(prefs1->images_list);
    }

    // Iterate the objects and add to the list
    for (int i = 0; i < g_list_model_get_n_items(G_LIST_MODEL(dir_list)); i++)
    {
        if (mime_type_supported())
        {
            // Get file info
            GFileInfo *info = G_FILE_INFO(g_list_model_get_item(
                G_LIST_MODEL(dir_list), i));
            const char *content_type = g_file_info_get_content_type(info);

            // Append image file info to the list
            // Check content type
            if (strncmp(content_type, "image/", 6) == 0)
            {
                // Add properties to the list
                const char *name = g_file_info_get_display_name(info);
                char *path = g_file_get_path(prefs1->file);
                char *image_path = g_strdup_printf("%s/%s", path, name);
                g_list_store_append(prefs1->images_list,
                                    my_item_new(name, image_path, FALSE));
                g_free(path);
            }
        }
        else
        {
            // Get file info
            GFileInfo *info = G_FILE_INFO(g_list_model_get_item(
                G_LIST_MODEL(dir_list), i));
            const char *content_type = g_file_info_get_content_type(info);
            char *pattern = g_strdup_printf("*%s", content_type);

            // Append image file info to the list
            for (int i = 0; supported_globs[i] != NULL; i++)
            {
                // Check file pattern on microsoft windows
                if (strncmp(pattern, supported_globs[i], strlen(supported_globs[i])) == 0)
                {
                    // Add properties to the list
                    const char *name = g_file_info_get_display_name(info);
                    char *path = g_file_get_path(prefs1->file);
                    char *image_path = g_strdup_printf("%s/%s", path, name);
                    g_list_store_append(prefs1->images_list,
                                        my_item_new(name, image_path, FALSE));
                    g_free(path);
                    break;
                }
            }
            g_free(pattern);
        }
    }

    // Check weather list is updated
    if (g_list_model_get_n_items(G_LIST_MODEL(prefs1->images_list)) == 0)
    {
        prefs1->current_folder_index = -1;
    }
}

static void pics_view_init(MyPrefs *self)
{
    // Create store for pictures view
    self->images_list = g_list_store_new(my_item_get_type());
    images_list_default(self->images_list);

    // Create selection model for view
    self->image_select = gtk_single_selection_new(G_LIST_MODEL(self->images_list));
    gtk_single_selection_set_selected(self->image_select, 0);

    // Create column view
    self->images_view = gtk_column_view_new(GTK_SELECTION_MODEL(self->image_select));

    // Image column
    self->factory_pics_image = gtk_signal_list_item_factory_new();
    g_signal_connect(self->factory_pics_image, "setup",
                     G_CALLBACK(factory_pics_image_setup), NULL);
    g_signal_connect(self->factory_pics_image, "bind",
                     G_CALLBACK(factory_pics_image_bind), NULL);
    self->pics_image_column = gtk_column_view_column_new(" ",
                                                         self->factory_pics_image);
    gtk_column_view_append_column(GTK_COLUMN_VIEW(self->images_view),
                                  self->pics_image_column);

    // Folder names column
    self->factory_pics_string = gtk_signal_list_item_factory_new();
    g_signal_connect(self->factory_pics_string, "setup",
                     G_CALLBACK(factory_pics_string_setup), NULL);
    g_signal_connect(self->factory_pics_string, "bind",
                     G_CALLBACK(factory_pics_string_bind), NULL);
    self->pics_string_column = gtk_column_view_column_new("Name",
                                                          self->factory_pics_string);
    gtk_column_view_append_column(GTK_COLUMN_VIEW(self->images_view),
                                  self->pics_string_column);
}

static void update_resource_image(MyPrefs *prefs, const char *resource_id)
{
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_resource(resource_id, NULL);
    GdkPixbuf *sized = gdk_pixbuf_scale_simple(pixbuf, prefs->width,
                                               prefs->height, GDK_INTERP_BILINEAR);
    GdkTexture *texture = gdk_texture_new_for_pixbuf(sized);
    // gtk_picture_set_content_fit(GTK_PICTURE(prefs->background), GTK_CONTENT_FIT_FILL);
    gtk_picture_set_paintable(GTK_PICTURE(prefs->background), GDK_PAINTABLE(texture));
    // gtk_picture_set_pixbuf(GTK_PICTURE(prefs->background), pixbuf);
    g_object_unref(pixbuf);
    g_object_unref(sized);
}

static void update_internal_image(MyPrefs *prefs, const char **id)
{
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_xpm_data(id);
    GdkPixbuf *sized = gdk_pixbuf_scale_simple(pixbuf, prefs->width,
                                               prefs->height, GDK_INTERP_BILINEAR);
    GdkTexture *texture = gdk_texture_new_for_pixbuf(sized);
    // gtk_picture_set_content_fit(GTK_PICTURE(prefs->background), GTK_CONTENT_FIT_FILL);
    gtk_picture_set_paintable(GTK_PICTURE(prefs->background), GDK_PAINTABLE(texture));
    // gtk_picture_set_pixbuf(GTK_PICTURE(prefs->background), pixbuf);
    g_object_unref(pixbuf);
    g_object_unref(sized);
}

static void update_external_image(MyPrefs *prefs, const char *file_name)
{
    GError *error = NULL;
    // Create a pixbuf
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(file_name, &error);
    if (error == NULL)
    {
        // Load the image with pixbuf
        GdkPixbuf *sized = gdk_pixbuf_scale_simple(pixbuf, prefs->width,
                                                   prefs->height, GDK_INTERP_BILINEAR);
        GdkTexture *texture = gdk_texture_new_for_pixbuf(sized);
        gtk_picture_set_paintable(GTK_PICTURE(prefs->background), GDK_PAINTABLE(texture));
        // gtk_picture_set_pixbuf(GTK_PICTURE(prefs->background), sized);
        g_object_unref(sized);
        g_object_unref(pixbuf);
    }
    else
    {
        // if file load failed, load default image
        update_internal_image(prefs, winpe);
    }
}

static void file_dialog_opened(GObject *dialog, GAsyncResult *result, gpointer data)
{
    GFile *file;
    MyPrefs *prefs = MY_PREFS(data);

    // Get the file
    file = gtk_file_dialog_select_folder_finish(GTK_FILE_DIALOG(dialog), result, NULL);
    if (file != NULL)
    {
        // g_print("Dialog Accepted!");
        char *path = g_file_get_path(file);
        char *name = g_file_get_basename(file);
        g_list_store_append(prefs->folders_list,
                            my_item_new(name, path, FALSE));
        g_object_unref(file);
        g_free(path);
        g_free(name);
    }
    else
    {
        // g_print("Dialog Cancelled!");
    }
}

static void btnadd_clicked(GtkWidget *widget, MyPrefs *prefs)
{
    GtkFileDialog *dialog;
    // Create a file dialog
    dialog = gtk_file_dialog_new();
    gtk_file_dialog_select_folder(dialog, GTK_WINDOW(prefs), NULL, file_dialog_opened, prefs);
}

static void btnremove_clicked(GtkWidget *widget, MyPrefs *prefs)
{
    // Remove the selected item, and the default item will not be removed
    guint index = gtk_single_selection_get_selected(prefs->folders_select);
    if (index > 2)
    {
        g_list_store_remove(prefs->folders_list, index);
    }
}

// Load config from the json config file
static void my_prefs_load_config(MyPrefs *self)
{
    // Open config from json file
    std::fstream json_file;
    json_file.open("my_gtkui.json", std::ios_base::in);
    if (json_file.is_open())
    {
        // Load data from json
        json data = json::parse(json_file);
        self->width = data["width"];
        self->height = data["height"];
        self->current_folder_index = data["folder_index"];
        self->current_image_index = data["image_index"];
        self->dock_pos = data["position"];
    }
    else
    {
        // Default window size
        self->width = 1024;
        self->height = 576;
        self->current_folder_index = 0;
        self->current_image_index = 0;
        self->dock_pos = DockPos::Pos_Left;
    }
    json_file.close();
}

// Save configs
static void my_prefs_save_config(MyPrefs *self)
{
    int width = 1280, height = 720;
    // Get size config
    gboolean default_actived;

    // Check the size config type
    default_actived = gtk_check_button_get_active(GTK_CHECK_BUTTON(self->radio_default));

    if (default_actived)
    {
        // Get default sizes
        guint index = gtk_drop_down_get_selected(GTK_DROP_DOWN(self->sizes_drop));
        switch (index)
        {
        case 0:
            width = 640;
            height = 360;
            break;
        case 1:
            width = 800;
            height = 450;
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
        width = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(self->spin_width));
        height = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(self->spin_height));
    }

    // Get the config of backgrounds
    guint folder_index, image_index;
    folder_index = gtk_single_selection_get_selected(
        GTK_SINGLE_SELECTION(self->folders_select));
    image_index = gtk_single_selection_get_selected(
        GTK_SINGLE_SELECTION(self->image_select));

    // Get Dock Position config
    if (gtk_check_button_get_active(
            GTK_CHECK_BUTTON(self->radio_bottom)))
    {
        self->curr_dock_pos = DockPos::Pos_Bottom;
    }

    if (gtk_check_button_get_active(
            GTK_CHECK_BUTTON(self->radio_left)))
    {
        self->curr_dock_pos = DockPos::Pos_Left;
    }

    if (gtk_check_button_get_active(
            GTK_CHECK_BUTTON(self->radio_right)))
    {
        self->curr_dock_pos = DockPos::Pos_Right;
    }

    // Get Panel mode config
    self->panel_mode = gtk_check_button_get_active(
        GTK_CHECK_BUTTON(self->mode_check));

    // Open file for json data
    std::fstream outfile;
    outfile.open("my_gtkui.json", std::ios_base::out);

    if (outfile.is_open())
    {
        // Create json data
        json data = json::parse(R"(
            {
                "background_folders": [],
                "folder_index": 0,
                "height": 576,
                "image_index": 0,
                "panel_mode": 1,
                "position": 0,
                "width": 1024
            }
        )");
        data["folder_index"] = folder_index;
        data["height"] = height;
        data["image_index"] = image_index;
        data["panel_mode"] = self->panel_mode;
        data["position"] = self->curr_dock_pos;
        data["width"] = width;
        outfile << data;
    }
    outfile.close();
}

// Scan the selection of two column views
static gboolean scan_func(gpointer data)
{
    MyPrefs *prefs = MY_PREFS(data);
    // Get the selection of folders view
    // The model and item of folders view
    auto folder_model = gtk_column_view_get_model(GTK_COLUMN_VIEW(prefs->folders_view));
    guint folder_item_index = gtk_single_selection_get_selected(GTK_SINGLE_SELECTION(folder_model));
    auto folder_item = gtk_single_selection_get_selected_item(GTK_SINGLE_SELECTION(folder_model));

    // File name and properties
    const char *folder_name = my_item_get_path(MY_ITEM(folder_item));
    gboolean is_internal = my_item_get_internal(MY_ITEM(folder_item));

    // Check weather the selection changed
    if (folder_item_index != prefs->current_folder_index || g_list_model_get_n_items(G_LIST_MODEL(prefs->images_list)) == 0)
    {
        if (is_internal)
        {
            // Update image list to default
            images_list_default(prefs->images_list);
        }
        else
        {
            // Update image list by the folder selection
            prefs->file = g_file_new_for_path(folder_name);
            gtk_directory_list_set_file(prefs->file_list, prefs->file);
            update_images_list(prefs);
            g_object_unref(prefs->file);
        }
        prefs->current_folder_index = folder_item_index;
        // Force reload image
        prefs->current_image_index = -1;
    }

    // Get the selection of images view
    // Check weather the list of images is empty
    if (g_list_model_get_n_items(G_LIST_MODEL(prefs->images_list)) == 0)
    {
        return TRUE;
    }

    // The model and item of images view
    auto model = gtk_column_view_get_model(GTK_COLUMN_VIEW(prefs->images_view));
    auto item = gtk_single_selection_get_selected_item(GTK_SINGLE_SELECTION(model));
    guint image_item_index = gtk_single_selection_get_selected(GTK_SINGLE_SELECTION(model));

    // File name and properties
    const char *file_name = my_item_get_path(MY_ITEM(item));
    is_internal = my_item_get_internal(MY_ITEM(item));

    // Check weather the selection changed
    if (image_item_index != prefs->current_image_index)
    {
        // Update image
        if (is_internal)
        {
            // For image which is internal
            switch (file_name[1])
            {
            case '1':
                update_resource_image(prefs, "/org/gtk/daleclack/final_approach.png");
                break;
            case '2':
                update_internal_image(prefs, img7);
                break;
            case '3':
                update_internal_image(prefs, winpe);
                break;
            }
            prefs->current_image_index = image_item_index;
            my_prefs_save_config(prefs);
        }
        else
        {
            // For image which is outside
            update_external_image(prefs, file_name);
            prefs->current_image_index = image_item_index;
            my_prefs_save_config(prefs);
        }
    }
    return TRUE;
}

void my_prefs_first_load(MyPrefs *self)
{
    gtk_widget_set_size_request(self->background, self->width, self->height);
    // Get the selection of folders view
    // The model and item of folders view
    gtk_single_selection_set_selected(
        self->folders_select, self->current_folder_index);
    auto folder_item = gtk_single_selection_get_selected_item(self->folders_select);

    // File name and properties
    const char *folder_name = my_item_get_path(MY_ITEM(folder_item));
    gboolean is_internal = my_item_get_internal(MY_ITEM(folder_item));
    // g_print("%s", folder_name);

    if (is_internal)
    {
        // Update image list to default
        images_list_default(self->images_list);
    }
    else
    {
        // Update image list by the folder selection
        self->file = g_file_new_for_path(folder_name);
        gtk_directory_list_set_file(self->file_list, self->file);
        update_images_list(self);
        g_object_unref(self->file);
    }

    // Get the selected image item
    gtk_single_selection_set_selected(
        self->image_select, self->current_image_index);
    auto item = gtk_single_selection_get_selected_item(self->image_select);

    // File name and properties
    if (item != NULL)
    {
        const char *file_name = my_item_get_path(MY_ITEM(item));
        is_internal = my_item_get_internal(MY_ITEM(item));
        // Update image
        if (is_internal)
        {
            // For image which is internal
            switch (file_name[1])
            {
            case '1':
                update_resource_image(self, "/org/gtk/daleclack/final_approach.png");
                break;
            case '2':
                update_internal_image(self, img7);
                break;
            case '3':
                update_internal_image(self, winpe);
                break;
            }
        }
        else
        {
            // For image which is outside
            update_external_image(self, file_name);
        }
    }
}

// static void my_prefs_close_request(GtkWindow *self, gpointer user_data)
// {
//     gtk_widget_set_visible(GTK_WIDGET(self), FALSE);
// }

// Apply config ==> Save the config file
static void btnapply_clicked(GtkWidget *widget, MyPrefs *self)
{
    my_prefs_save_config(self);
}

static void my_prefs_init(MyPrefs *self)
{
    // Options strings for dropdown
    const char *strings[] = {
        "640x360",
        "800x450",
        "1024x576",
        "1280x720",
        "1366x768",
        NULL};

    // Load config
    my_prefs_load_config(self);

    // Initalize window
    gtk_window_set_default_size(GTK_WINDOW(self), 800, 450);
    gtk_window_set_icon_name(GTK_WINDOW(self), "My_GtkUI");
    gtk_window_set_title(GTK_WINDOW(self), "Preferences");

    // Create builder
    self->prefs_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/prefs_stack.ui");

    // Get Objects
    self->stack_box = GTK_WIDGET(gtk_builder_get_object(self->prefs_builder, "stack_box"));
    self->back_page = GTK_WIDGET(gtk_builder_get_object(self->prefs_builder, "back_page"));
    self->combo_box = GTK_WIDGET(gtk_builder_get_object(self->prefs_builder, "combo_box"));
    self->mode_check = GTK_WIDGET(gtk_builder_get_object(self->prefs_builder, "mode_check"));
    self->label_size = GTK_WIDGET(gtk_builder_get_object(self->prefs_builder, "label_size"));
    self->radio_custom = GTK_WIDGET(gtk_builder_get_object(self->prefs_builder, "radio_custom"));
    self->radio_default = GTK_WIDGET(gtk_builder_get_object(self->prefs_builder, "radio_default"));
    self->spin_height = GTK_WIDGET(gtk_builder_get_object(self->prefs_builder, "spin_height"));
    self->spin_width = GTK_WIDGET(gtk_builder_get_object(self->prefs_builder, "spin_width"));
    self->btnGet = GTK_WIDGET(gtk_builder_get_object(self->prefs_builder, "btnGet"));
    self->btnapply = GTK_WIDGET(gtk_builder_get_object(self->prefs_builder, "btnapply"));
    self->radio_bottom = GTK_WIDGET(gtk_builder_get_object(self->prefs_builder, "radio_bottom"));
    self->radio_left = GTK_WIDGET(gtk_builder_get_object(self->prefs_builder, "radio_left"));
    self->radio_right = GTK_WIDGET(gtk_builder_get_object(self->prefs_builder, "radio_right"));
    self->btnapply1 = GTK_WIDGET(gtk_builder_get_object(self->prefs_builder, "btnapply1"));

    // Initalize folder and image views and append to the box
    gtk_widget_set_margin_bottom(self->back_page, 20);
    gtk_widget_set_margin_end(self->back_page, 20);
    gtk_widget_set_margin_start(self->back_page, 20);
    gtk_widget_set_margin_top(self->back_page, 20);
    folder_view_init(self);
    pics_view_init(self);
    self->views_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    self->btns_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    self->btn_add = gtk_button_new_from_icon_name("list-add");
    self->btn_remove = gtk_button_new_from_icon_name("list-remove");

    // Pack views
    self->folders_sw = gtk_scrolled_window_new();
    gtk_widget_set_hexpand(self->folders_view, TRUE);
    gtk_widget_set_vexpand(self->folders_view, TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(self->folders_sw),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(self->folders_sw), self->folders_view);
    self->images_sw = gtk_scrolled_window_new();
    gtk_widget_set_hexpand(self->images_view, TRUE);
    gtk_widget_set_vexpand(self->images_view, TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(self->images_sw),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(self->images_sw), self->images_view);
    gtk_box_append(GTK_BOX(self->views_box), self->folders_sw);
    gtk_box_append(GTK_BOX(self->views_box), self->images_sw);
    gtk_box_append(GTK_BOX(self->back_page), self->views_box);
    gtk_box_append(GTK_BOX(self->btns_box), self->btn_add);
    gtk_box_append(GTK_BOX(self->btns_box), self->btn_remove);
    gtk_box_append(GTK_BOX(self->back_page), self->btns_box);

    // Create file list
    self->file = g_file_new_for_path(g_get_home_dir());
    self->file_list = gtk_directory_list_new(
        "standard::name,standard::display-name,standard::icon,standard::size,standard::content-type", self->file);
    g_object_unref(self->file);

    // Add a dropdown for default sizes config
    self->sizes_drop = gtk_drop_down_new_from_strings(strings);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(self->sizes_drop), 2);
    gtk_box_append(GTK_BOX(self->combo_box), self->sizes_drop);

    char *size = g_strdup_printf("Current Config: %d x %d", self->width, self->height);
    gtk_label_set_label(GTK_LABEL(self->label_size), size);
    g_free(size);

    // Set default mode for check button
    gtk_check_button_set_active(GTK_CHECK_BUTTON(self->mode_check), TRUE);

    // Default value for dock position
    switch (self->dock_pos)
    {
    case DockPos::Pos_Bottom:
        gtk_check_button_set_active(GTK_CHECK_BUTTON(self->radio_bottom), TRUE);
        break;
    case DockPos::Pos_Left:
        gtk_check_button_set_active(GTK_CHECK_BUTTON(self->radio_left), TRUE);
        break;
    case DockPos::Pos_Right:
        gtk_check_button_set_active(GTK_CHECK_BUTTON(self->radio_right), TRUE);
        break;
    }

    // Set Child
    gtk_window_set_child(GTK_WINDOW(self), self->stack_box);

    // Update Default selection of images

    // Default values to load the internal image
    // This value for the first scan
    // self->current_folder_index = -1;
    // self->current_image_index = -1;

    // Bind properties for radio buttons and other widgets
    g_object_bind_property(self->radio_custom, "active",
                           self->spin_height, "sensitive", G_BINDING_DEFAULT);
    g_object_bind_property(self->radio_custom, "active",
                           self->spin_width, "sensitive", G_BINDING_DEFAULT);
    g_object_bind_property(self->radio_custom, "active",
                           self->btnGet, "sensitive", G_BINDING_DEFAULT);
    g_object_bind_property(self->radio_default, "active",
                           self->sizes_drop, "sensitive", G_BINDING_DEFAULT);

    // Connect signals
    g_signal_connect(self->btn_add, "clicked", G_CALLBACK(btnadd_clicked), self);
    g_signal_connect(self->btn_remove, "clicked", G_CALLBACK(btnremove_clicked), self);
    g_signal_connect(self->btnapply, "clicked", G_CALLBACK(btnapply_clicked), self);
    g_signal_connect(self->btnapply1, "clicked", G_CALLBACK(btnapply_clicked), self);
}

static void my_prefs_class_init(MyPrefsClass *klass)
{
}

void my_prefs_start_scan(MyPrefs *self)
{
    // Add timer to scan the list
    g_timeout_add(1, scan_func, self);
}

// Get Dock position
DockPos my_prefs_get_dock_pos(MyPrefs *self)
{
    return self->dock_pos;
}

MyPrefs *my_prefs_new(GtkWidget *back)
{
    // Create Prefs widget
    MyPrefs *prefs_win = MY_PREFS(g_object_new(my_prefs_get_type(), NULL));

    // The size for the background widget
    prefs_win->background = back;
    gtk_widget_set_size_request(back, prefs_win->width, prefs_win->height);
    gtk_picture_set_content_fit(GTK_PICTURE(back), GTK_CONTENT_FIT_FILL);
    return prefs_win;
}
