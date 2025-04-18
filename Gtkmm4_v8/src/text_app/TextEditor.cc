#include "TextEditor.hh"
#include "text_types.hh"
#include "../tomlplusplus/toml.hpp"
#include <fstream>
#include <iostream>
#include <string>

TextEditor::TextEditor()
    : vbox(Gtk::Orientation::VERTICAL, 5),
      hbox(Gtk::Orientation::HORIZONTAL, 5),
      searchbox(Gtk::Orientation::HORIZONTAL, 5),
      infobar(Gtk::Orientation::HORIZONTAL, 5),
      info_ok("OK"),
      file_opened(false)
{
    // Load window config from config file
    int width = 800, height = 450;
    std::fstream cfg_file("text_config.toml");
    if (cfg_file.is_open())
    {
        auto toml = toml::parse(cfg_file);
        width = toml.get("width")->as_integer()->get();
        height = toml.get("height")->as_integer()->get();
    }
    cfg_file.close();
    // std::ifstream json_file("text_config.json");
    // if (json_file.is_open())
    // {
    //     json data = json::parse(json_file);
    //     width = data["width"];
    //     height = data["height"];
    // }
    // json_file.close();

    // Initalize Window
    set_default_size(width, height);
    set_icon_name("my_textedit");
    set_title("Simple Text Editor");

    // Initalize HeaderBar
    menubtn.set_icon_name("open-menu");
    search_button.set_image_from_icon_name("finder-find");
    header.pack_end(menubtn);
    header.pack_end(search_button);
    header.set_ctrl_window(this);

    // Add a menu
    menu_builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/text_menu.xml");
    auto object = menu_builder->get_object<Gio::MenuModel>("text_menu");
    popover.set_menu_model(object);
    menubtn.set_popover(popover);

    // Initalize Text Buffers
    buffer1 = textview1.get_buffer();
    buffer1->signal_changed().connect(sigc::mem_fun(*this, &TextEditor::buffer1_changed));

    // Pack Widgets
    textview1.set_expand(true);
    sw1.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    sw1.set_child(textview1);
    hbox.append(sw1);

    // Add actions and signal handlers
    add_action("text_open", sigc::mem_fun(*this, &TextEditor::btnopen_clicked));
    add_action("text_save", sigc::mem_fun(*this, &TextEditor::btnsave_clicked));
    add_action("text_saveas", sigc::mem_fun(*this, &TextEditor::btnsaveas_clicked));
    add_action("text_copy", sigc::mem_fun(*this, &TextEditor::btncopy_clicked));
    add_action("text_paste", sigc::mem_fun(*this, &TextEditor::btnpaste_clicked));
    add_action("text_close", sigc::mem_fun(*this, &TextEditor::btnclose_clicked));
    add_action("text_about", sigc::mem_fun(*this, &TextEditor::about_activated));

    // Add searchbar and search up and down buttons
    search_up.set_image_from_icon_name("filewin-go-up");
    search_down.set_image_from_icon_name("filewin-go-down");

    // Bind property and signals
    search_binding = Glib::Binding::bind_property(search_button.property_active(),
                                                  searchbar.property_search_mode_enabled(),
                                                  Glib::Binding::Flags::BIDIRECTIONAL);
    search_entry.signal_changed().connect(sigc::mem_fun(*this, &TextEditor::search_entry_changed));
    search_up.signal_clicked().connect(sigc::mem_fun(*this, &TextEditor::search_backward));
    search_down.signal_clicked().connect(sigc::mem_fun(*this, &TextEditor::search_forward));

    // Pack widgets
    searchbox.append(search_entry);
    searchbox.append(search_up);
    searchbox.append(search_down);
    searchbar.set_child(searchbox);
    vbox.append(searchbar);

    // A InfoBar
    infobar.append(label1);
    infobar.append(info_ok);
    infobar.set_halign(Gtk::Align::CENTER);
    info_ok.signal_clicked().connect(sigc::mem_fun(*this, &TextEditor::infobar_response));
    infobar.set_visible(false);
    vbox.append(infobar);
    vbox.append(hbox);

    // Save config when the window is closed
    // signal_delete_event().connect(sigc::mem_fun(*this, &TextEditor::window_delete_event));

    // Add Intergated keyboard
    expend_builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/expander.ui");
    expender = expend_builder->get_widget<Gtk::Expander>("key_expand");
    btnshift = expend_builder->get_widget<Gtk::ToggleButton>("btnshift");
    btncaps = expend_builder->get_widget<Gtk::ToggleButton>("btn_caps");
    btntab = expend_builder->get_widget<Gtk::Button>("btntab");
    btnenter = expend_builder->get_widget<Gtk::Button>("btnenter");
    vbox.append(*expender);

    // Get alphabet buttons
    for (int i = 0; i < 26; i++)
    {
        char name[10];
        sprintf(name, "btn%d", i);
        btns[i] = expend_builder->get_widget<Gtk::Button>(name);
        btns[i]->signal_clicked().connect(sigc::bind(
            sigc::mem_fun(*this, &TextEditor::key_pressed),
            btns[i]));
    }
    btntab->signal_clicked().connect(sigc::mem_fun(*this, &TextEditor::btntab_clicked));
    btnenter->signal_clicked().connect(sigc::mem_fun(*this, &TextEditor::btnenter_clicked));

    // Show everything
    set_child(vbox);
    // add(vbox);
    // show_all_children();
    infobar.hide();
}

void TextEditor::key_pressed(Gtk::Button *button)
{
    auto label = button->get_label();
    Glib::ustring::size_type pos = 0, len = 1;
    char buf[2];
    if (btncaps->get_active() || btnshift->get_active())
    {
        btnshift->set_active(false);
    }
    else
    {
        sprintf(buf, "%c", label[0] + 32);
        label.replace(pos, len, buf);
    }
    // std::cout << label << std::endl;
    buffer1->insert_at_cursor(label);
}

void TextEditor::btntab_clicked()
{
    buffer1->insert_at_cursor("\t");
}

void TextEditor::btnenter_clicked()
{
    buffer1->insert_at_cursor("\n");
}

bool TextEditor::save_config()
{
    // Get Size of text window
    int width = get_width();
    int height = get_height();

    // Create toml data
    toml::table tbl;
    tbl.insert_or_assign("width", width);
    tbl.insert_or_assign("height", height);

    // Save size config to toml file
    std::fstream outfile;
    outfile.open("text_config.toml", std::ios_base::out);
    if (outfile.is_open())
    {
        outfile << tbl;
    }
    outfile.close();

    return true;
}

// bool TextEditor::window_delete_event(GdkEventAny *event)
// {
//     // Create json raw data
//     json data = json::parse(R"({
//         "width":800,
//         "height":450
//     })");

//     // Override config in json file
//     data["width"] = sw1.get_width();
//     data["height"] = sw1.get_height();

//     // Output json data to file
//     std::fstream outfile;
//     outfile.open("config.json", std::ios_base::out);
//     if (outfile.is_open())
//     {
//         outfile << data;
//     }
//     outfile.close();
//     return false;
// }

void TextEditor::btnopen_clicked()
{
    // Create a dialog
    dialog = Gtk::FileDialog::create();
    dialog->set_title("Open a text file");

    // Add Filters
    auto filters = Gio::ListStore<Gtk::FileFilter>::create();
    auto filter = Gtk::FileFilter::create();
    filter->set_name("Text Files");
    if (mimetype_supported())
    {
        filter->add_mime_type("text/*");
    }
    else
    {
        for (int i = 0; text_globs[i] != NULL; i++)
        {
            const char *glob = text_globs[i];
            filter->add_pattern(glob);
        }
    }
    filters->append(filter);
    // dialog->add_filter(filter);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any Files");
    filter_any->add_pattern("*");
    filters->append(filter_any);
    // dialog->add_filter(filter_any);

    dialog->open(sigc::mem_fun(*this, &TextEditor::dialog_open_file));
    // dialog->show();
}

void TextEditor::dialog_open_file(const Glib::RefPtr<Gio::AsyncResult> &result)
{
    try
    {
        // Open the file and load contents
        auto file = dialog->open_finish(result);
        curr_filename = file->get_path();
        file_opened = true;
        char *contents;
        gsize length;
        if (file->load_contents(contents, length))
        {
            buffer1->set_text(contents);
        }
    }
    catch (const Gtk::DialogError &ex)
    {
        std::cout << "No file selected." << ex.what() << std::endl;
    }
    catch (const Glib::Error &ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    dialog.reset();
}

void TextEditor::btnsave_clicked()
{
    if (file_opened)
    {
        // Get Text
        Glib::ustring text;
        text = buffer1->get_text();
        // Set file opened to true to use save mode
        file_opened = true;
        // Save to a file
        std::ofstream outfile;
        outfile.open(curr_filename, std::ios_base::out);
        outfile << text;
        outfile.close();
    }
}

void TextEditor::btnsaveas_clicked()
{
    // Create a dialog
    dialog = Gtk::FileDialog::create();
    dialog->set_title("Save text to a file");
    // dialog->signal_response().connect(sigc::mem_fun(*this, &TextEditor::savedialog_response));

    // Add Filters
    auto filters = Gio::ListStore<Gtk::FileFilter>::create();
    auto filter = Gtk::FileFilter::create();
    filter->set_name("Text Files");
    if (mimetype_supported())
    {
        filter->add_mime_type("text/*");
    }
    else
    {
        for (int i = 0; text_globs[i] != NULL; i++)
        {
            const char *glob = text_globs[i];
            filter->add_pattern(glob);
        }
    }
    filters->append(filter);
    // dialog->add_filter(filter);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any Files");
    filter_any->add_pattern("*");
    filters->append(filter_any);
    // dialog->add_filter(filter_any);

    dialog->save(sigc::mem_fun(*this, &TextEditor::dialog_save_file));
    // dialog->show();
}

void TextEditor::dialog_save_file(const Glib::RefPtr<Gio::AsyncResult> &result)
{
    try
    {
        // Get FileName
        auto file = dialog->save_finish(result);
        std::string filename = file->get_path();

        // Get Text
        Glib::ustring text;
        text = buffer1->get_text();

        // Save to a file
        std::ofstream outfile;
        outfile.open(filename, std::ios_base::out);
        outfile << text;
        outfile.close();
    }
    catch (const Gtk::DialogError &ex)
    {
        std::cout << "No file selected." << ex.what() << std::endl;
    }
    catch (const Glib::Error &ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    dialog.reset();
}

void TextEditor::buffer1_changed()
{
    // When the text changed,enable the copy button
}

void TextEditor::search_entry_changed()
{
    // Get Text to search
    const Glib::ustring text = search_entry.get_text();

    Gtk::TextIter start, end;
    // If get text to search, select the text and storage the position
    if (text.length() != 0)
    {
        if (buffer1->begin().forward_search(text, Gtk::TextSearchFlags::CASE_INSENSITIVE, start, end))
        {
            curr_iter_up = start;
            curr_iter_down = end;
            buffer1->select_range(start, end);
            textview1.scroll_to(start);
        }
    }
}

void TextEditor::search_forward()
{
    // Get Text to search
    const Glib::ustring search_text = search_entry.get_text();

    Gtk::TextIter start, end;
    // Get Text to search, down to the end of text
    if (search_text.length() != 0)
    {
        if (curr_iter_down.forward_search(search_text, Gtk::TextSearchFlags::CASE_INSENSITIVE, start, end))
        {
            curr_iter_up = start;
            curr_iter_down = end;
            buffer1->select_range(start, end);
            textview1.scroll_to(start);
        }
    }
}

void TextEditor::search_backward()
{
    // Get Text to search, up to the start of text
    const Glib::ustring search_text = search_entry.get_text();

    Gtk::TextIter start, end;
    // Get Text to search
    if (search_text.length() != 0)
    {
        if (curr_iter_up.backward_search(search_text, Gtk::TextSearchFlags::CASE_INSENSITIVE, start, end))
        {
            curr_iter_up = start;
            curr_iter_down = end;
            buffer1->select_range(start, end);
            textview1.scroll_to(start);
        }
    }
}

void TextEditor::btncopy_clicked()
{
    // Get Text
    Glib::ustring text;
    Gtk::TextBuffer::iterator start, end;
    if (buffer1->get_selection_bounds(start, end))
    {
        text = buffer1->get_text(start, end);
    }
    else
    {
        text = buffer1->get_text();
    }

    // Get Clipboard and set text
    auto refClipboard = textview1.get_display()->get_clipboard();
    // auto refClipboard = Gtk::Clipboard::get();
    refClipboard->set_text(text);

    // Show InfoBar
    label1.set_label("The Text is copyed");
    infobar.set_visible(true);
}

void TextEditor::btnpaste_clicked()
{
    // Get ClipBoard
    auto refClipboard = textview1.get_display()->get_clipboard();

    // auto refClipboard = Gtk::Clipboard::get();
    refClipboard->read_text_async(sigc::bind(
        sigc::mem_fun(*this, &TextEditor::clipboard_receive), refClipboard));
    // refClipboard->request_text(sigc::mem_fun(*this, &TextEditor::clipboard_receive));
}

void TextEditor::clipboard_receive(const Glib::RefPtr<Gio::AsyncResult> &result,
                                   const Glib::RefPtr<Gdk::Clipboard> &clipboard)
{
    try
    {
        Glib::ustring text;
        text = clipboard->read_text_finish(result);
        if (buffer1->insert_interactive_at_cursor(text))
        {
            // Show InfoBar
            label1.set_label("The Text is Pasted at cursor position");
            infobar.set_visible(true);
        }
        else
        {
            // Show InfoBar
            label1.set_label("Text Paste Error!");
            infobar.set_visible(true);
        }
    }
    catch (const Glib::Error &ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
}

void TextEditor::btnclose_clicked()
{
    buffer1->set_text("");
    file_opened = false;
}

void TextEditor::infobar_response()
{
    infobar.set_visible(false);
}

void TextEditor::about_activated()
{
    char *version, *copyright;
    // The Gtkmm Version
    version = g_strdup_printf("1.0\nRunning Against Gtkmm %d.%d.%d",
                              GTKMM_MAJOR_VERSION,
                              GTKMM_MINOR_VERSION,
                              GTKMM_MICRO_VERSION);
    const char *authors[] = {"Dale Clack", NULL};
    // Copyright Informaion
    copyright = g_strdup_printf("© 2019—2022 The Xe Project");
    // Show the about dialog
    gtk_show_about_dialog(GTK_WINDOW(this->gobj()),
                          "program-name", "Text Editot",
                          "version", version,
                          "copyright", copyright,
                          "comments", "A simple text editor",
                          "authors", authors,
                          "license-type", GTK_LICENSE_GPL_3_0,
                          "logo-icon-name", "my_textedit",
                          "title", "About Simple text editor",
                          (char *)NULL);
    // Free memory
    g_free(version);
    g_free(copyright);
}
