#include "MediaPlayer.h"

struct _MediaPlayer
{
    GtkApplicationWindow parent;
    GtkWidget *video;
    GtkWidget *menubtn;
};

G_DEFINE_TYPE(MediaPlayer, media_player, GTK_TYPE_APPLICATION_WINDOW)

static void clear_activated(GSimpleAction *action,
                            GVariant *parmeter,
                            gpointer data)
{
    MediaPlayer *player = MEDIA_PLAYER(data);

    GtkMediaStream *stream = gtk_video_get_media_stream(GTK_VIDEO(player->video));
    gtk_media_file_clear(GTK_MEDIA_FILE(stream));
    gtk_video_set_file(GTK_VIDEO(player->video), NULL);
    // Set window title
    gtk_window_set_title(GTK_WINDOW(data), "Gtk4 Media");
}

static void pause_activated(GSimpleAction *action,
                            GVariant *parmeter,
                            gpointer data)
{
    MediaPlayer *player = MEDIA_PLAYER(data);
    GtkMediaStream *media = gtk_video_get_media_stream(GTK_VIDEO(player->video));

    if (media != NULL)
    {
        gtk_media_stream_pause(media);
        g_object_unref(media);
    }
}

static void play_activated(GSimpleAction *action,
                           GVariant *parmeter,
                           gpointer data)
{
    MediaPlayer *player = MEDIA_PLAYER(data);
    GtkMediaStream *media = gtk_video_get_media_stream(GTK_VIDEO(player->video));

    if (media != NULL)
    {
        gtk_media_stream_play(media);
        g_object_unref(media);
    }
}

static void about_activated(GSimpleAction *action,
                            GVariant *parmeter,
                            gpointer data)
{
    GtkWindow *win = GTK_WINDOW(data);
    const char *authors[] = {
        "Dale Clack",
        NULL};
    // Program version
    char *version;
    version = g_strdup_printf("2.1\nRunning Against GTK %d.%d.%d",
                              gtk_get_major_version(),
                              gtk_get_minor_version(),
                              gtk_get_micro_version());
    // GtkWidget *dialog;
    // dialog = gtk_dialog_new();
    gtk_show_about_dialog(win,
                          "program-name", "Gtk4 Media",
                          "version", version,
                          "copyright", "© 2019—2024 The Xe Project",
                          "comments", "A Simple Media Player",
                          "authors", authors,
                          "logo-icon-name", "media-player1",
                          "title", "About Gtk4 Media",
                          NULL);
}

// static void dialog_response(GtkWidget *widget, int response, MediaPlayer *player)
// {
//     // Get File and set the file to the mediacontrols
//     if (response == GTK_RESPONSE_OK)
//     {
//         // Get media File and filename
//         char *filename = NULL;
//         GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(widget));
//         gtk_video_set_file(GTK_VIDEO(player->video), file);
//         filename = g_file_get_basename(file);
//         gtk_window_set_title(GTK_WINDOW(player), filename);
//         // Free the memory
//         g_object_unref(file);
//         free(filename);
//     }
//     gtk_window_destroy(GTK_WINDOW(widget));
// }

static void dialog_response(GObject *object, GAsyncResult *result, gpointer data)
{
    MediaPlayer *player = MEDIA_PLAYER(data);
    GtkFileDialog *dialog = GTK_FILE_DIALOG(object);
    GFile *file;
    file = gtk_file_dialog_open_finish(dialog, result, NULL);
    // Get File and set the file to the mediacontrols
    if (file)
    {
        // Get media File and filename
        char *filename = NULL;
        gtk_video_set_file(GTK_VIDEO(player->video), file);
        filename = g_file_get_basename(file);
        gtk_window_set_title(GTK_WINDOW(player), filename);
        // Free the memory
        g_object_unref(file);
        free(filename);
    }
}

static void open_activated(GSimpleAction *action,
                           GVariant *parmeter,
                           gpointer data)
{
    // Set a dialog and choose the file
    GtkWindow *parent = GTK_WINDOW(data);
    GtkFileDialog *dialog;
    dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Open Media File");
    // GtkWidget *dialog;
    // GtkFileChooserAction action1 = GTK_FILE_CHOOSER_ACTION_OPEN;
    // dialog = gtk_file_chooser_dialog_new("Open Media File", parent, action1,
    //                                      "OK", GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL);
    // Set filters
    GListStore *filters;
    GtkFileFilter *filter;

    // Add file types for filter
    filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.mp3");
    gtk_file_filter_add_pattern(filter, "*.wav");
    gtk_file_filter_add_pattern(filter, "*.flac");
    gtk_file_filter_add_pattern(filter, "*.aac");
    gtk_file_filter_add_pattern(filter, "*.m4a");
    g_list_store_append(filters, filter);

    gtk_file_dialog_open(dialog, parent, NULL, dialog_response, parent);
    // gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);
    // g_signal_connect(dialog, "response", G_CALLBACK(dialog_response), parent);
    // gtk_widget_show(dialog);
}

static void quit_activated(GSimpleAction *action, GVariant *parmeter, gpointer data)
{
    gtk_window_close(GTK_WINDOW(data));
}

static void media_player_init(MediaPlayer *self)
{
    // Initalize Window
    GtkWidget *header = gtk_header_bar_new();
    gtk_window_set_title(GTK_WINDOW(self), "Video Player");
    gtk_window_set_titlebar(GTK_WINDOW(self), header);

    // Ininalize Action
    GActionEntry entries[] = {
        {"quit", quit_activated, NULL, NULL, NULL},
        {"open", open_activated, NULL, NULL, NULL},
        {"play", play_activated, NULL, NULL, NULL},
        {"pause", pause_activated, NULL, NULL, NULL},
        {"clear", clear_activated, NULL, NULL, NULL},
        {"about", about_activated, NULL, NULL, NULL}};
    g_action_map_add_action_entries(G_ACTION_MAP(self), entries, G_N_ELEMENTS(entries), self);

    // Add Menu
    self->menubtn = gtk_menu_button_new();
    GtkBuilder *menu_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/media_menu.xml");
    GMenuModel *model = G_MENU_MODEL(gtk_builder_get_object(menu_builder, "media-menu"));
    GtkWidget *popover = gtk_popover_menu_new_from_model(model);
    gtk_widget_set_halign(popover, GTK_ALIGN_END);
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(self->menubtn), popover);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), self->menubtn);

    // Create video
    self->video = gtk_video_new();
    gtk_widget_set_size_request(self->video, 712, 400);
    gtk_window_set_child(GTK_WINDOW(self), self->video);

    g_object_unref(menu_builder);
}

static void media_player_class_init(MediaPlayerClass *klass) {}

MediaPlayer *media_player_new(GtkWindow *parent_win)
{
    return (MediaPlayer *)g_object_new(media_player_get_type(), "transient-for", parent_win, NULL);
}
