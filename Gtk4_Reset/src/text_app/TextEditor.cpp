#include "TextEditor.h"
#include "MyInfoBar.h"
#include "../json_nlohmann/json.hpp"
#include <cstring>
#include <fstream>

using json = nlohmann::json;

struct _TextEditor
{
    GtkApplicationWindow parent_instance;
    GtkWidget *header;
    GtkWidget *main_box;
    GtkWidget *text_view, *scrolled_win;
    GtkTextBuffer *text_buffer;
    GtkWidget *menu_btn, *popover;
    GtkBuilder *menu_builder, *expander_builder;
    // Input keyboard
    GtkWidget *expander;
    GtkWidget *btns[26];
    GtkWidget *btnshift, *btntab, *btn_caps, *btnenter;

    // Info
    MyInfoBar *info_bar;

    // Search Area
    GtkWidget *btnsearch;
    GtkWidget *search_box;
    GtkWidget *search_bar, *search_entry;
    GtkWidget *btn_up, *btn_down;
    GtkTextIter curr_start, curr_end;

    // Others
    int width, height;
    char file_path[PATH_MAX] = {0};
};

G_DEFINE_TYPE(TextEditor, text_editor, GTK_TYPE_APPLICATION_WINDOW)

static void text_dialog_open_file(GObject *dialog, GAsyncResult *result, gpointer data)
{
    GFile *file;
    TextEditor *editor = TEXT_EDITOR(data);
    char *contents;
    gsize length;

    // Get file to open
    file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(dialog), result, NULL);
    if (file != NULL)
    {
        // If file opened, try to load contents
        if (g_file_load_contents(file, NULL, &contents, &length, NULL, NULL))
        {
            gtk_text_buffer_set_text(editor->text_buffer, contents, length);

            // Load path for file opened
            auto path = g_file_get_path(file);
            strncpy(editor->file_path, path, PATH_MAX);
            g_free(path);
        }
        g_object_unref(file);
        g_free(contents);
    }
}

static void text_editor_open_activated(GSimpleAction *action,
                                       GVariant *parmeter,
                                       gpointer data)
{
    TextEditor *editor = TEXT_EDITOR(data);
    // Create GtkFileDialog
    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_open(dialog, GTK_WINDOW(editor), NULL, text_dialog_open_file, editor);
}

static char *text_editor_get_text(TextEditor *editor)
{
    // Get Content of text buffer
    GtkTextIter start_iter, end_iter;
    gtk_text_buffer_get_start_iter(editor->text_buffer, &start_iter);
    gtk_text_buffer_get_end_iter(editor->text_buffer, &end_iter);
    char *content = gtk_text_buffer_get_text(editor->text_buffer, &start_iter, &end_iter, TRUE);
    return content;
}

static void text_dialog_save_file(GObject *dialog, GAsyncResult *result, gpointer data)
{
    GFile *file;
    TextEditor *editor = TEXT_EDITOR(data);

    // Get Text
    char *content = text_editor_get_text(editor);

    // Open the file to save text
    file = gtk_file_dialog_save_finish(GTK_FILE_DIALOG(dialog), result, NULL);
    if (file != NULL)
    {
        char *path = g_file_get_path(file);
        if (g_file_set_contents(path, content, strlen(content), NULL))
        {
            // Update path and show infomation
            strncpy(editor->file_path, path, PATH_MAX);
            my_infobar_show_message(editor->info_bar, "Content Saved!");
        }
    }
}

static void text_editor_save_activated(GSimpleAction *action,
                                       GVariant *parmeter,
                                       gpointer data)
{
    TextEditor *editor = TEXT_EDITOR(data);

    // Check the file for content
    if (strlen(editor->file_path) != 0)
    {
        char *content = text_editor_get_text(editor);
        // Save content to opened file
        if (g_file_set_contents(editor->file_path, content, strlen(content), NULL))
        {
            my_infobar_show_message(editor->info_bar, "Content saved!");
        }
        g_free(content);
    }
    else
    {
        // Open a file dialog to save file
        GtkFileDialog *dialog = gtk_file_dialog_new();
        gtk_file_dialog_save(dialog, GTK_WINDOW(editor), NULL, text_dialog_save_file, editor);
    }
}

static void text_editor_saveas_activated(GSimpleAction *action,
                                         GVariant *parmeter,
                                         gpointer data)
{
    // Open a file dialog to save file
    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_save(dialog, GTK_WINDOW(data), NULL, text_dialog_save_file, data);
}

static void text_editor_copy_activated(GSimpleAction *action,
                                       GVariant *parmeter,
                                       gpointer data)
{
    TextEditor *editor = TEXT_EDITOR(data);

    // Get ClipBoard
    GdkClipboard *clipboard = gtk_widget_get_clipboard(editor->text_view);

    // Get Selected text
    gtk_text_buffer_copy_clipboard(editor->text_buffer, clipboard);
    my_infobar_show_message(editor->info_bar, "Text Copied!");
}

static void text_editor_cut_activated(GSimpleAction *action,
                                      GVariant *parmeter,
                                      gpointer data)
{
    TextEditor *editor = TEXT_EDITOR(data);

    // Get ClipBoard
    GdkClipboard *clipboard = gtk_widget_get_clipboard(editor->text_view);

    // Get Selected text
    gtk_text_buffer_cut_clipboard(editor->text_buffer, clipboard, TRUE);
    my_infobar_show_message(editor->info_bar, "Text Cutted!");
}

static void text_editor_paste_activated(GSimpleAction *action,
                                        GVariant *parmeter,
                                        gpointer data)
{
    TextEditor *editor = TEXT_EDITOR(data);

    // Get ClipBoard
    GdkClipboard *clipboard = gtk_widget_get_clipboard(editor->text_view);

    // Get Selected text
    GtkTextIter insert_iter;
    gtk_text_buffer_paste_clipboard(editor->text_buffer, clipboard, NULL, TRUE);
    my_infobar_show_message(editor->info_bar, "Text Pasted!");
}

static void text_editor_close_activated(GSimpleAction *action,
                                        GVariant *parmeter,
                                        gpointer data)
{
    TextEditor *editor = TEXT_EDITOR(data);
    gtk_text_buffer_set_text(editor->text_buffer, "", 0);
    gtk_window_close(GTK_WINDOW(data));
}

static void text_editor_about_activated(GSimpleAction *action,
                                        GVariant *parmeter,
                                        gpointer data)
{
}

static void text_editor_btn_clicked(GtkButton *btn, TextEditor *self)
{
    char tmp[2] = {0};
    // Get label
    const char *label = gtk_button_get_label(btn);

    // Check Caps Lock or Shift
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self->btnshift)) || gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self->btn_caps)))
    {
        tmp[0] = label[0];
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(self->btnshift), FALSE);
    }
    else
    {
        tmp[0] = label[0] + 32;
    }

    // Insert text
    gtk_text_buffer_insert_at_cursor(self->text_buffer, tmp, 1);
}

static void text_editor_btnenter_clicked(GtkButton *btn, TextEditor *self)
{
    // Insert text
    gtk_text_buffer_insert_at_cursor(self->text_buffer, "\n", 1);
}

static void text_editor_btntab_clicked(GtkButton *btn, TextEditor *self)
{
    // Insert text
    gtk_text_buffer_insert_at_cursor(self->text_buffer, "\t", 1);
}

static void text_editor_search_backward(GtkWidget *widget, TextEditor *self)
{
    GtkTextIter start_iter, match_start, match_end;

    // Get text to search
    const char *search_text = gtk_editable_get_text(GTK_EDITABLE(self->search_entry));
    start_iter = self->curr_start;
    if (gtk_text_iter_backward_search(&start_iter, search_text, GTK_TEXT_SEARCH_CASE_INSENSITIVE,
                                     &match_start, &match_end, NULL))
    {
        // Select text when search success
        self->curr_start = match_start;
        self->curr_end = match_end;
        gtk_text_buffer_select_range(self->text_buffer, &match_start, &match_end);
        gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(self->text_view), &match_start,
                                     0.0, FALSE, 0.0, 0.0);
    }
}

static void text_editor_search_forward(GtkWidget *widget, TextEditor *self)
{
    GtkTextIter start_iter, match_start, match_end;

    // Get text to search
    const char *search_text = gtk_editable_get_text(GTK_EDITABLE(self->search_entry));
    start_iter = self->curr_end;
    if (gtk_text_iter_forward_search(&start_iter, search_text, GTK_TEXT_SEARCH_CASE_INSENSITIVE,
                                     &match_start, &match_end, NULL))
    {
        // Select text when search success
        self->curr_start = match_start;
        self->curr_end = match_end;
        gtk_text_buffer_select_range(self->text_buffer, &match_start, &match_end);
        gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(self->text_view), &match_start,
                                     0.0, FALSE, 0.0, 0.0);
    }
}

static void text_editor_search_changed(GtkSearchEntry *entry, TextEditor *self)
{
    GtkTextIter start_iter, match_start, match_end;

    // Get text to search
    const char *search_text = gtk_editable_get_text(GTK_EDITABLE(entry));
    gtk_text_buffer_get_start_iter(self->text_buffer, &start_iter);
    if (gtk_text_iter_forward_search(&start_iter, search_text, GTK_TEXT_SEARCH_CASE_INSENSITIVE,
                                     &match_start, &match_end, NULL))
    {
        // Select text when search success
        self->curr_start = match_start;
        self->curr_end = match_end;
        gtk_text_buffer_select_range(self->text_buffer, &match_start, &match_end);
        gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(self->text_view), &match_start,
                                     0.0, FALSE, 0.0, 0.0);
    }
}

static void text_editor_init(TextEditor *self)
{
    // Load window config from json file
    int width = 800, height = 450;
    std::ifstream json_file("text_config.json");
    if (json_file.is_open())
    {
        json data = json::parse(json_file);
        width = data["width"];
        height = data["height"];
    }
    json_file.close();

    // Use headerbar for title and more info
    self->header = gtk_header_bar_new();
    gtk_window_set_title(GTK_WINDOW(self), "Text editor");
    gtk_window_set_titlebar(GTK_WINDOW(self), self->header);
    gtk_window_set_default_size(GTK_WINDOW(self), width, height);

    // Add Actions for menu
    GActionEntry entries[] =
        {
            {"text_open", text_editor_open_activated, NULL, NULL, NULL},
            {"text_save", text_editor_save_activated, NULL, NULL, NULL},
            {"text_saveas", text_editor_saveas_activated, NULL, NULL, NULL},
            {"text_copy", text_editor_copy_activated, NULL, NULL, NULL},
            {"text_cut", text_editor_cut_activated, NULL, NULL, NULL},
            {"text_paste", text_editor_paste_activated, NULL, NULL, NULL},
            {"text_close", text_editor_close_activated, NULL, NULL, NULL},
            {"text_about", text_editor_about_activated, NULL, NULL, NULL}};
    g_action_map_add_action_entries(G_ACTION_MAP(self),
                                    entries, G_N_ELEMENTS(entries), self);

    // Add menu button
    self->menu_btn = gtk_menu_button_new();
    gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(self->menu_btn), "open-menu");
    gtk_header_bar_pack_end(GTK_HEADER_BAR(self->header), self->menu_btn);
    gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(self->header), TRUE);

    // Add a menu
    self->menu_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/text_menu.xml");
    GMenuModel *model = G_MENU_MODEL(gtk_builder_get_object(self->menu_builder, "text_menu"));
    self->popover = gtk_popover_menu_new_from_model(model);
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(self->menu_btn), self->popover);
    gtk_widget_set_halign(self->popover, GTK_ALIGN_END);
    g_object_unref(model);

    // Create box for layouts
    self->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    self->scrolled_win = gtk_scrolled_window_new();

    // Add Search Button
    self->btnsearch = gtk_toggle_button_new();
    gtk_button_set_icon_name(GTK_BUTTON(self->btnsearch), "find");
    gtk_header_bar_pack_end(GTK_HEADER_BAR(self->header), self->btnsearch);

    // Add a search bar
    self->search_bar = gtk_search_bar_new();
    self->search_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    self->btn_up = gtk_button_new_from_icon_name("up");
    self->btn_down = gtk_button_new_from_icon_name("down");
    self->search_entry = gtk_search_entry_new();
    gtk_box_append(GTK_BOX(self->search_box), self->search_entry);
    gtk_box_append(GTK_BOX(self->search_box), self->btn_up);
    gtk_box_append(GTK_BOX(self->search_box), self->btn_down);
    gtk_search_bar_set_child(GTK_SEARCH_BAR(self->search_bar), self->search_box);
    gtk_box_append(GTK_BOX(self->main_box), self->search_bar);
    g_object_bind_property(self->btnsearch, "active",
                           self->search_bar, "search-mode-enabled", G_BINDING_DEFAULT);
    g_signal_connect(self->search_entry, "search-changed",
                     G_CALLBACK(text_editor_search_changed), self);
    g_signal_connect(self->btn_up, "clicked", G_CALLBACK(text_editor_search_backward), self);
    g_signal_connect(self->btn_down, "clicked", G_CALLBACK(text_editor_search_forward), self);

    // Add info area
    self->info_bar = my_infobar_new();
    gtk_box_append(GTK_BOX(self->main_box), GTK_WIDGET(self->info_bar));

    // Create text view
    self->text_view = gtk_text_view_new();
    self->text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));
    gtk_widget_set_hexpand(self->text_view, TRUE);
    gtk_widget_set_vexpand(self->text_view, TRUE);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(self->scrolled_win), self->text_view);
    gtk_box_append(GTK_BOX(self->main_box), self->scrolled_win);

    // Get Expander for input keyboard
    self->expander_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/expander.ui");
    self->expander = GTK_WIDGET(gtk_builder_get_object(self->expander_builder, "key_expand"));
    gtk_box_append(GTK_BOX(self->main_box), self->expander);

    // Get Buttons for keyboard and link signal
    self->btn_caps = GTK_WIDGET(gtk_builder_get_object(self->expander_builder, "btn_caps"));
    self->btnenter = GTK_WIDGET(gtk_builder_get_object(self->expander_builder, "btnenter"));
    self->btnshift = GTK_WIDGET(gtk_builder_get_object(self->expander_builder, "btnshift"));
    self->btntab = GTK_WIDGET(gtk_builder_get_object(self->expander_builder, "btntab"));
    for (int i = 0; i < 26; i++)
    {
        char name[10];
        sprintf(name, "btn%d", i);
        (self->btns)[i] = GTK_WIDGET(gtk_builder_get_object(self->expander_builder, name));
        g_signal_connect((self->btns)[i], "clicked", G_CALLBACK(text_editor_btn_clicked), self);
    }
    g_signal_connect(self->btnenter, "clicked", G_CALLBACK(text_editor_btnenter_clicked), self);
    g_signal_connect(self->btntab, "clicked", G_CALLBACK(text_editor_btntab_clicked), self);

    gtk_window_set_child(GTK_WINDOW(self), self->main_box);
}

static void text_editor_class_init(TextEditorClass *klass)
{
}

TextEditor *text_editor_new()
{
    return TEXT_EDITOR(g_object_new(text_editor_get_type(), NULL));
}

void text_editor_save_config(TextEditor *self)
{
    // Create json raw data
    json data = json::parse(R"({
        "width":800,
        "height":450
    })");

    // Override config in json file
    data["width"] = gtk_widget_get_width(self->main_box);
    data["height"] = gtk_widget_get_height(self->main_box);

    // Output json data to file
    std::fstream outfile;
    outfile.open("text_config.json", std::ios_base::out);
    if (outfile.is_open())
    {
        outfile << data;
    }
    outfile.close();
}
