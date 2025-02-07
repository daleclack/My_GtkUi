#include "MyMediaPlayer.hh"
#include <iostream>
#include <string_view>

MyMediaPlayer::MyMediaPlayer()
    : main_box(Gtk::Orientation::VERTICAL, 5),
      ctrl_box(Gtk::Orientation::HORIZONTAL, 5),
      list_box(Gtk::Orientation::VERTICAL, 5),
      ctrl_box2(Gtk::Orientation::HORIZONTAL, 5)
{
    // Initialize window
    set_title("My Media Player");
    set_icon_name("org.gtk.daleclack");
    set_default_size(300, 500);

    // Add custom header bar
    header.set_ctrl_window(this);
    // header.set_title_bar(*this);

    // Add video widget and label for lyrics
    video.set_autoplay();
    video.set_size_request(300, 150);
    main_box.append(video);
    label_lyrics.set_use_markup(true);
    label_lyrics.set_markup("<span color='red' size='12pt'>No Media Playing...</span>");
    lyrics_area.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::NEVER);
    lyrics_area.set_child(label_lyrics);
    main_box.append(lyrics_area);

    // Create color dialog
    color_dialog = Gtk::ColorDialog::create();
    btncolor.set_dialog(color_dialog);

    // Update icons for widgets
    btnplay.set_icon_name("media-playback-start");
    btnprev.set_icon_name("media-skip-backward");
    btnnext.set_icon_name("media-skip-forward");
    btnstop.set_icon_name("media-playback-stop");
    btnmode.set_icon_name("media-playlist-repeat");
    ctrl_box.append(btnprev);
    ctrl_box.append(btnplay);
    ctrl_box.append(btnnext);
    ctrl_box.append(btnstop);
    ctrl_box.append(btnmode);
    ctrl_box.append(btncolor);
    ctrl_box.set_halign(Gtk::Align::CENTER);
    main_box.append(ctrl_box);

    // Add buttons for playlist management
    btn_add.set_icon_name("list-add");
    btn_remove.set_icon_name("list-remove");
    btn_clear.set_icon_name("list-remove-all-symbolic");
    btn_load.set_icon_name("go-up");
    btn_save.set_icon_name("document-save");
    ctrl_box2.append(btn_add);
    ctrl_box2.append(btn_remove);
    ctrl_box2.append(btn_clear);
    ctrl_box2.append(btn_load);
    ctrl_box2.append(btn_save);
    ctrl_box2.set_halign(Gtk::Align::CENTER);
    list_box.append(ctrl_box2);

    // Create list store and list view for playlist
    media_list = Gio::ListStore<MyItem>::create();
    media_selection = Gtk::SingleSelection::create(media_list);
    media_factory = Gtk::SignalListItemFactory::create();
    media_factory->signal_setup().connect(sigc::mem_fun(*this, &MyMediaPlayer::setup_view));
    media_factory->signal_bind().connect(sigc::mem_fun(*this, &MyMediaPlayer::bind_view));
    list_view.set_model(media_selection);
    list_view.set_factory(media_factory);

    // Add list view to the playlist box
    list_scroller.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    list_scroller.set_child(list_view);
    list_box.append(list_scroller);
    list_scroller.set_vexpand(true);
    list_view.set_vexpand(true);

    // Add expander for playlist
    expander_list.set_label("Playlist");
    expander_list.set_vexpand(true);
    expander_list.set_child(list_box);
    main_box.append(expander_list);

    // Default play mode
    current_mode = PlayMode::List_Repeat;

    // Load a new playlist
    load_playlist("Playlist.toml");

    // Connect signals
    btnprev.signal_clicked().connect(sigc::mem_fun(*this, &MyMediaPlayer::btnprev_clicked));
    btnplay.signal_clicked().connect(sigc::mem_fun(*this, &MyMediaPlayer::btnplay_clicked));
    btnnext.signal_clicked().connect(sigc::mem_fun(*this, &MyMediaPlayer::btnnext_clicked));
    btnstop.signal_clicked().connect(sigc::mem_fun(*this, &MyMediaPlayer::btnstop_clicked));
    btnmode.signal_clicked().connect(sigc::mem_fun(*this, &MyMediaPlayer::btnmode_clicked));
    // btncolor.signal_clicked().connect(sigc::mem_fun(*this, &MyMediaPlayer::btncolor_clicked));
    btn_add.signal_clicked().connect(sigc::mem_fun(*this, &MyMediaPlayer::btnadd_clicked));
    btn_remove.signal_clicked().connect(sigc::mem_fun(*this, &MyMediaPlayer::btnremove_clicked));
    btn_clear.signal_clicked().connect(sigc::mem_fun(*this, &MyMediaPlayer::btnclear_clicked));
    btn_load.signal_clicked().connect(sigc::mem_fun(*this, &MyMediaPlayer::btnload_clicked));
    btn_save.signal_clicked().connect(sigc::mem_fun(*this, &MyMediaPlayer::btnsave_clicked));
    list_view.signal_activate().connect(sigc::mem_fun(*this, &MyMediaPlayer::item_activated));

    set_child(main_box);
}

MyMediaPlayer::~MyMediaPlayer()
{
}

void MyMediaPlayer::setup_view(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    // Create a label for the list item
    auto label = Gtk::make_managed<Gtk::Label>("");
    list_item->set_child(*label);
}

void MyMediaPlayer::bind_view(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    // Bind the label's text property to the item's titles
    auto pos = list_item->get_position();
    if (pos == GTK_INVALID_LIST_POSITION)
        return;
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());
    auto item = media_list->get_item(pos);
    label->set_label(item->get_name());
}

void MyMediaPlayer::item_activated(guint pos)
{
    // Check whether a media stream is playing
    auto media_stream = video.get_media_stream();
    // if has audio, stop it
    if (media_stream)
    {
        btnstop_clicked();
    }

    // Update the audio
    update_audio(pos);
    current_index = pos;

    // Add a timer for timer
    timer = Glib::signal_timeout().connect(sigc::mem_fun(*this, &MyMediaPlayer::timeout_func), 100);
}

void MyMediaPlayer::update_audio(guint index)
{
    // Get the pos of media file and play
    auto item = media_list->get_item(index);
    video.set_filename(item->get_path());

    // Update lyrics label
    update_lyrics_label(item->get_name());
    lyrics_parser.update_lyrics(item->get_path());

    // Update selection
    media_selection->set_selected(index);
}

void MyMediaPlayer::update_lyrics_label(const Glib::ustring &lyrics_string)
{
    char *color_string;
    // Get color and add color information to the markup
    Gdk::RGBA color = btncolor.get_rgba();
    color_string = g_strdup_printf("#%02X%02X%02X",
                                   color.get_red_u(),
                                   color.get_green_u(),
                                   color.get_blue_u());

    Glib::ustring lyrics_str;
    lyrics_str = Glib::ustring("<span color='");
    lyrics_str += Glib::ustring(color_string);
    lyrics_str += Glib::ustring("' size='12pt'>");
    lyrics_str += lyrics_string;
    lyrics_str += Glib::ustring("</span>");

    // Update lyrics label
    label_lyrics.set_markup(lyrics_str);

    free(color_string);
}

void MyMediaPlayer::btnprev_clicked()
{
    guint n_audios = media_list->get_n_items();

    // Select the previous audio item
    if (current_index > 0)
    {
        current_index--;
    }
    else
    {
        current_index = n_audios - 1;
    }

    // Get the filename and play
    update_audio(current_index);
    // auto item = media_list->get_item(current_index);
    // video.set_filename(item->get_path());
}

void MyMediaPlayer::btnplay_clicked()
{
    // Get the media stream to control
    auto media_stream = video.get_media_stream();

    if (!media_stream)
    {
        return;
    }

    // Play and pause the media
    if (media_stream->get_playing())
    {
        media_stream->pause();
        btnplay.set_icon_name("media-playback-start");
    }
    else
    {
        media_stream->play();
        btnplay.set_icon_name("media-playback-pause");
    }
}

void MyMediaPlayer::btnnext_clicked()
{
    guint n_audios = media_list->get_n_items();

    // Select the next audio item
    if (current_index < n_audios - 1)
    {
        current_index++;
    }
    else
    {
        current_index = 0;
    }

    // Get the filename and play
    update_audio(current_index);
    // auto item = media_list->get_item(current_index);
    // video.set_filename(item->get_path());
}

void MyMediaPlayer::btnstop_clicked()
{
    // Get the media stream to control
    // auto media_stream = video.get_media_stream();
    video.set_filename("");
    // media_stream->unrealize();
    // media_stream.reset();
}

void MyMediaPlayer::btnmode_clicked()
{
    // Change Play Mode
    switch (current_mode)
    {
    case PlayMode::List_Once:
        current_mode = PlayMode::List_Repeat;
        btnmode.set_icon_name("media-playlist-repeat");
        break;
    case PlayMode::List_Repeat:
        current_mode = PlayMode::List_Shuffle;
        btnmode.set_icon_name("media-playlist-shuffle");
        break;
    case PlayMode::List_Shuffle:
        current_mode = PlayMode::One_Repeat;
        btnmode.set_icon_name("media-playlist-repeat-one");
        break;
    case PlayMode::One_Repeat:
        current_mode = PlayMode::List_Once;
        btnmode.set_icon_name("media-playlist-normal");
        break;
    }
}

void MyMediaPlayer::btnadd_clicked()
{
    // Create a dialog and open it
    auto dialog = Gtk::FileDialog::create();
    dialog->set_title("Open a media file");
    dialog->open(sigc::bind(sigc::mem_fun(*this, &MyMediaPlayer::openfile_dialog_finish), dialog));
}

void MyMediaPlayer::btnload_clicked()
{
    // Create a dialog to open a toml file
    auto dialog = Gtk::FileDialog::create();
    dialog->set_title("Open a playlist file");

    // Add file filter
    auto filters = Gio::ListStore<Gtk::FileFilter>::create();
    auto filter = Gtk::FileFilter::create();
    filter->add_pattern("*.toml");
    filters->append(filter);

    dialog->set_filters(filters);

    // Open the dialog
    dialog->open(sigc::bind(sigc::mem_fun(*this, &MyMediaPlayer::openlist_dialog_finish), dialog));
}

void MyMediaPlayer::btnsave_clicked()
{
    // Create a dialog to open a toml file
    auto dialog = Gtk::FileDialog::create();
    dialog->set_title("Save the playlist");

    // Add file filter
    auto filters = Gio::ListStore<Gtk::FileFilter>::create();
    auto filter = Gtk::FileFilter::create();
    filter->add_pattern("*.toml");
    filters->append(filter);

    dialog->set_filters(filters);

    // Open the dialog
    dialog->save(sigc::bind(sigc::mem_fun(*this, &MyMediaPlayer::savelist_dialog_finish), dialog));
}

void MyMediaPlayer::btnremove_clicked()
{
    // Remove a item
    auto pos = media_selection->get_selected();
    media_list->remove(pos);

    // After a media remove, update item numbers
    n_media = media_list->get_n_items();

    // Update saved playlist
    save_playlist("Playlist.toml");
}

void MyMediaPlayer::btnclear_clicked()
{
    // Clear all items
    media_list->remove_all();
    n_media = 0;

    // Update saved playlist
    save_playlist("Playlist.toml");
}

void MyMediaPlayer::load_playlist(const std::string &filename)
{
    std::fstream infile;
    infile.open(filename, std::ios_base::in);
    if (!infile.is_open()){
        return;
    }
    infile.close();

    auto config = toml::parse_file(filename);
    if (config.contains("names") && config.contains("paths"))
    {
        // Clear previous list
        if (media_list->get_n_items() > 0)
        {
            media_list->remove_all();
        }

        // Load name and path array from the playlist file
        auto names = *(config.get_as<toml::array>("names"));
        auto paths = *(config.get_as<toml::array>("paths"));

        // Update list
        for (size_t i = 0; i < names.size(); i++)
        {
            std::optional<std::string> name1 = names[i].value<std::string>();
            std::optional<std::string> path1 = paths[i].value<std::string>();
            media_list->append(MyItem::create(name1.value(), path1.value()));
        }

        // Update media quantity
        n_media = media_list->get_n_items();
    }
}

void MyMediaPlayer::save_playlist(const std::string &filename)
{
    // Create toml object
    static constexpr std::string_view source = R"(
        paths = []
        names = []
    )";

    toml::table tbl = toml::parse(source);
    toml::array names, paths;

    // Get content from the list
    for (int i = 0; i < n_media; i++)
    {
        auto item = media_list->get_item(i);
        names.push_back(std::string(item->get_name()));
        paths.push_back(std::string(item->get_path()));
    }

    // Save the content to toml file
    tbl.insert_or_assign("names", names);
    tbl.insert_or_assign("paths", paths);

    // Save the data to file
    std::fstream outfile;
    outfile.open(filename, std::ios_base::out);
    if (outfile.is_open())
    {
        outfile << tbl;
    }
    outfile.close();
}

void MyMediaPlayer::openfile_dialog_finish(Glib::RefPtr<Gio::AsyncResult> &result, Glib::RefPtr<Gtk::FileDialog> &dialog)
{
    try
    {
        Glib::ustring path, filename;
        // Open the media file and append to the list
        auto file = dialog->open_finish(result);
        path = file->get_path();
        filename = file->get_basename();
        auto item = MyItem::create(filename, path);
        media_list->append(item);

        // After a media add, update item numbers
        n_media = media_list->get_n_items();

        // Update saved playlist
        save_playlist("Playlist.toml");
        file.reset();
    }
    catch (const Gtk::DialogError &err)
    {
        // Can be thrown by dialog->open_finish(result).
        std::cout << "No file selected. " << err.what() << std::endl;
    }
    catch (const Glib::Error &err)
    {
        std::cout << "Unexpected exception. " << err.what() << std::endl;
    }
}

void MyMediaPlayer::savelist_dialog_finish(Glib::RefPtr<Gio::AsyncResult> &result, Glib::RefPtr<Gtk::FileDialog> &dialog)
{
    try
    {
        // Get filename
        auto file = dialog->save_finish(result);
        std::string path = file->get_path();

        // Save current playlist to a path
        save_playlist(path);
    }
    catch (const Gtk::DialogError &err)
    {
        // Can be thrown by dialog->open_finish(result).
        std::cout << "No file selected. " << err.what() << std::endl;
    }
    catch (const Glib::Error &err)
    {
        std::cout << "Unexpected exception. " << err.what() << std::endl;
    }
}

void MyMediaPlayer::openlist_dialog_finish(Glib::RefPtr<Gio::AsyncResult> &result, Glib::RefPtr<Gtk::FileDialog> &dialog)
{
    try
    {
        // Get filename
        auto file = dialog->open_finish(result);
        std::string path = file->get_path();

        // Load a new playlist
        load_playlist(path);

        // Save current playlist
        save_playlist("Playlist.toml");
    }
    catch (const Gtk::DialogError &err)
    {
        // Can be thrown by dialog->open_finish(result).
        std::cout << "No file selected. " << err.what() << std::endl;
    }
    catch (const Glib::Error &err)
    {
        std::cout << "Unexpected exception. " << err.what() << std::endl;
    }
}

bool MyMediaPlayer::timeout_func()
{
    Glib::RefPtr<MyItem> item;
    // Get media stream status
    auto media_stream = video.get_media_stream();

    // No media stream, just skip
    if (!media_stream)
    {
        return true;
    }

    // Change "Play/Pause" Button icon
    bool media_playing = media_stream->get_playing();
    if (media_playing)
    {
        btnplay.set_icon_name("media-playback-pause");
    }
    else
    {
        btnplay.set_icon_name("media-playback-start");
    }

    // Update Current Lyrics
    gint64 current_time = media_stream->get_timestamp();
    gint64 timestamp_ms = current_time / 1000;
    Glib::ustring current_lyric = lyrics_parser.get_lyric_line(timestamp_ms);
    if (!current_lyric.empty() || current_lyric == " ")
    {
        update_lyrics_label(current_lyric);
    }

    // std::cout << "Current lyrics: " << current_lyric << std::endl;

    // Get media status
    bool media_ended = media_stream->get_ended();
    if (!media_ended)
    {
        return true;
    }

    switch (current_mode)
    {
    case PlayMode::List_Once:            // Play media in list once
        if (current_index < n_media - 1) // Update media index
        {
            current_index++; 

            // Get media and play
            update_audio(current_index);
        }
        break;
    case PlayMode::List_Repeat:          // Play media in list repeat
        if (current_index < n_media - 1) // Update media index
        {
            current_index++;
        }
        else
        {
            current_index = 0;
        }
        // std::cout << current_index << " " << n_media;

        // Get media and play
        update_audio(current_index);
        break;
    case PlayMode::List_Shuffle:          // Play random media in the list
        current_index = rand() % n_media; // Update media index
        
        // Get media and play
        update_audio(current_index);
        break;
    case PlayMode::One_Repeat: // Play a media repeatly
        media_stream->play();
        break;
    }

    return true;
}
