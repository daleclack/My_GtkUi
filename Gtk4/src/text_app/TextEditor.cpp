#include "TextEditor.h"
#include "MyTitleBar.h"

struct _TextEditor
{
    GtkWindow parent_instance;
    MyTitleBar *title_bar;
    GtkWidget *infobar, *info_btn;
    GtkWidget *textview;
    GtkTextBuffer *textbuffer;
    GdkClipboard *clipboard;
    GtkWidget *searchbtn;
    GtkWidget *searchbar;
    GtkWidget *searchentry;
    int filemode;
    int width, height;
};

G_DEFINE_TYPE(TextEditor, text_editor, GTK_TYPE_WINDOW)

static void search_text_changed(GtkEntry *entry, TextEditor *editor)
{
    GtkTextIter start_iter, match_start, match_end;

    // Get Search Text
    const char *text = gtk_editable_get_text(GTK_EDITABLE(entry));

    // Get Start Iter
    gtk_text_buffer_get_start_iter(editor->textbuffer, &start_iter);

    // Get Searched text
    if (gtk_text_iter_forward_search(&start_iter, text, GTK_TEXT_SEARCH_CASE_INSENSITIVE,
                                     &match_start, &match_end, NULL))
    {
        // Select if get the searched text
        gtk_text_buffer_select_range(editor->textbuffer, &match_start, &match_end);
        gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(editor->textview), &match_start,
                                     0.0, FALSE, 0.0, FALSE);
    }
}

static void dialog_response(GObject *dialog, GAsyncResult *result, gpointer data)
{
    TextEditor *editor = TEXT_EDITOR(data);
    GtkFileDialog *dialog1 = GTK_FILE_DIALOG(dialog);
    GFile *file;
    char *contents, *filename;
    gsize length;
    if (editor->filemode == 0)
    {
        // Open a file
        file = gtk_file_dialog_open_finish(dialog1, result, NULL);
        if (file)
        {
            filename = g_file_get_path(file);
            // Get File contents and show
            if (g_file_get_contents(filename, &contents, &length, NULL))
            {
                gtk_text_buffer_set_text(editor->textbuffer, contents, length);
            }
        }
    }
    else
    {
        // Save the content to a file
        file = gtk_file_dialog_save_finish(dialog1, result, NULL);
        if (file)
        {
            filename = g_file_get_path(file);
            // Get Start and End char of Text
            GtkTextIter start, end;
            gtk_text_buffer_get_start_iter(editor->textbuffer, &start);
            gtk_text_buffer_get_end_iter(editor->textbuffer, &end);
            // Get Contents
            contents = gtk_text_buffer_get_text(editor->textbuffer, &start, &end, TRUE);
            // Create a file and write
            FILE *fp;
            fp = fopen(filename, "wt+");
            if (fp != NULL)
            {
                fputs(contents, fp);
            }
            fclose(fp);
        }
    }
    if (file)
    {
        g_object_unref(file);
    }

    g_free(contents);
    g_free(filename);
}

static void openfile_dialog(GtkWidget *widget, TextEditor *editor)
{
    editor->filemode = 0;
    // Create the file dialog
    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Open Text File");

    // Add filters
    GListStore *filter_store = g_list_store_new(GTK_TYPE_FILE_FILTER);
    GtkFileFilter *filter_text = gtk_file_filter_new();
    gtk_file_filter_set_name(filter_text, "Text Files");
    gtk_file_filter_add_mime_type(filter_text, "text/*");
    g_list_store_append(filter_store, filter_text);

    GtkFileFilter *filter_any = gtk_file_filter_new();
    gtk_file_filter_set_name(filter_any, "Any Files");
    gtk_file_filter_add_pattern(filter_any, "*");
    g_list_store_append(filter_store, filter_any);
    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filter_store));

    // Open the dialog
    gtk_file_dialog_open(dialog, GTK_WINDOW(editor), NULL, dialog_response, editor);
}

static void savefile_dialog(GtkWidget *widget, TextEditor *editor)
{
    editor->filemode = 1;
    // Create the file dialog
    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Save to Text File");

    // Add filters to the dialog
    GListStore *filter_store = g_list_store_new(GTK_TYPE_FILE_FILTER);
    GtkFileFilter *filter_text = gtk_file_filter_new();
    gtk_file_filter_set_name(filter_text, "Text Files");
    gtk_file_filter_add_mime_type(filter_text, "text/*");
    g_list_store_append(filter_store, filter_text);

    GtkFileFilter *filter_any = gtk_file_filter_new();
    gtk_file_filter_set_name(filter_any, "Any Files");
    gtk_file_filter_add_pattern(filter_any, "*");
    g_list_store_append(filter_store, filter_any);
    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filter_store));

    // Open the dialog for save text
    gtk_file_dialog_save(dialog, GTK_WINDOW(editor), NULL, dialog_response, editor);
}

static void textbuffer_clear(GtkWidget *widget, TextEditor *editor)
{
    gtk_text_buffer_set_text(editor->textbuffer, "", -1);
    gtk_widget_set_visible(editor->infobar, TRUE);
}

static void infobar_response(GtkWidget *widget, gpointer data)
{
    gtk_widget_set_visible(GTK_WIDGET(data), FALSE);
}

static void btncopy_clicked(GtkWidget *widget, TextEditor *editor)
{
    // Get Text or selected text
    const char *contents;
    GtkTextIter start, end;
    if (gtk_text_buffer_get_has_selection(editor->textbuffer))
    {
        gtk_text_buffer_get_selection_bounds(editor->textbuffer, &start, &end);
        contents = gtk_text_buffer_get_text(editor->textbuffer, &start, &end, TRUE);
        // g_print("Get Selected text\n");
        // g_print("%s\n",contents);
    }
    else
    {
        gtk_text_buffer_get_start_iter(editor->textbuffer, &start);
        gtk_text_buffer_get_end_iter(editor->textbuffer, &end);
        // Get Contents
        contents = gtk_text_buffer_get_text(editor->textbuffer, &start, &end, TRUE);
    }

    // Get Clipboard and put text
    gdk_clipboard_set_text(GDK_CLIPBOARD(editor->clipboard), contents);
}

static void get_text(GObject *source_object, GAsyncResult *res, gpointer data)
{
    char *text;
    GError *error = NULL;
    TextEditor *editor = TEXT_EDITOR(data);
    // Get text for copy operation
    text = gdk_clipboard_read_text_finish(editor->clipboard, res, &error);
    // Paste text
    if (text)
    {
        gtk_text_buffer_insert_at_cursor(editor->textbuffer, text, -1);
    }
    else
    {
        GtkWidget *dialog;

        /* Show an error about why pasting failed.
         * Usually you probably want to ignore such failures,
         * but for demonstration purposes, we show the error.
         */
        g_print("Could not paste text: %s", error->message);
        // dialog = gtk_message_dialog_new(GTK_WINDOW(editor),
        //                                 GTK_DIALOG_DESTROY_WITH_PARENT,
        //                                 GTK_MESSAGE_ERROR,
        //                                 GTK_BUTTONS_CLOSE,
        //                                 "Could not paste text: %s",
        //                                 error->message);
        // g_signal_connect(dialog, "response",
        //                  G_CALLBACK(gtk_window_destroy), NULL);
        // gtk_widget_show(dialog);

        g_error_free(error);
    }
}

static void btnpaste_clicked(GtkWidget *widget, TextEditor *editor)
{
    // Get ClipBoard and paste text
    gdk_clipboard_read_text_async(editor->clipboard, NULL, get_text, editor);
}

static void text_editor_init(TextEditor *self)
{
    GtkWidget *hbox, *textbox;
    self->title_bar = my_titlebar_new();
    my_titlebar_set_window(self->title_bar, self);
    gtk_window_set_title((GtkWindow *)self, "Simple Text Editor");
    gtk_window_set_icon_name((GtkWindow *)self, "org.gtk.daleclack");
    // gtk_window_set_default_size((GtkWindow *)self, 800, 450);
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    textbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // Initalize Searchbar
    self->searchbar = gtk_search_bar_new();
    self->searchentry = gtk_search_entry_new();
    self->searchbtn = gtk_toggle_button_new_with_label("Search");
    gtk_search_bar_set_child(GTK_SEARCH_BAR(self->searchbar), self->searchentry);
    g_object_bind_property(self->searchbtn, "active", self->searchbar,
                           "search-mode-enabled", G_BINDING_DEFAULT);
    gtk_box_append((GtkBox *)textbox, self->searchbar);
    g_signal_connect(self->searchentry, "search-changed", G_CALLBACK(search_text_changed), self);

    // Initalize infobar
    self->infobar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *label = gtk_label_new("Cleared the text");
    GtkWidget *space_label = gtk_label_new("    ");
    self->info_btn = gtk_button_new_with_label("OK");
    gtk_box_append(GTK_BOX(self->infobar), label);
    gtk_box_append(GTK_BOX(self->infobar), space_label);
    gtk_box_append(GTK_BOX(self->infobar), self->info_btn);
    gtk_widget_set_halign(self->infobar, GTK_ALIGN_CENTER);
    g_signal_connect(self->info_btn, "clicked", G_CALLBACK(infobar_response), self->infobar);
    gtk_box_append((GtkBox *)textbox, self->infobar);

    // Ininalize TextView
    GtkWidget *scrolled;
    scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy((GtkScrolledWindow *)scrolled,
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    self->textview = gtk_text_view_new();
    self->textbuffer = gtk_text_view_get_buffer((GtkTextView *)(self->textview));
    gtk_text_buffer_set_enable_undo(self->textbuffer, TRUE);
    self->clipboard = gtk_widget_get_clipboard(self->textview);
    // gtk_widget_set_size_request(scrolled,800,440);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), self->textview);
    gtk_widget_set_hexpand(scrolled, TRUE);
    gtk_widget_set_vexpand(scrolled, TRUE);
    gtk_box_append((GtkBox *)textbox, scrolled);
    gtk_box_append((GtkBox *)hbox, textbox);

    // Add Buttons
    GtkWidget *btn_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *btnopen, *btnsave, *btnclear, *btncopy, *btnpaste, *btnexit;
    btnopen = gtk_button_new_with_label("Open");
    g_signal_connect(btnopen, "clicked", G_CALLBACK(openfile_dialog), self);
    btnsave = gtk_button_new_with_label("Save");
    g_signal_connect(btnsave, "clicked", G_CALLBACK(savefile_dialog), self);
    btnclear = gtk_button_new_with_label("Clear");
    g_signal_connect(btnclear, "clicked", G_CALLBACK(textbuffer_clear), self);
    btncopy = gtk_button_new_with_label("Copy");
    g_signal_connect(btncopy, "clicked", G_CALLBACK(btncopy_clicked), self);
    btnpaste = gtk_button_new_with_label("Paste");
    g_signal_connect(btnpaste, "clicked", G_CALLBACK(btnpaste_clicked), self);
    btnexit = gtk_button_new_with_label("Exit");
    g_signal_connect_swapped(btnexit, "clicked", G_CALLBACK(gtk_window_close), self);
    gtk_widget_set_valign(btn_box, GTK_ALIGN_CENTER);
    gtk_box_prepend((GtkBox *)btn_box, btnexit);
    gtk_box_prepend((GtkBox *)btn_box, btnpaste);
    gtk_box_prepend((GtkBox *)btn_box, btncopy);
    gtk_box_prepend((GtkBox *)btn_box, btnclear);
    gtk_box_prepend((GtkBox *)btn_box, btnsave);
    gtk_box_prepend((GtkBox *)btn_box, btnopen);
    gtk_box_prepend((GtkBox *)btn_box, self->searchbtn);
    gtk_box_append((GtkBox *)hbox, btn_box);

    // Add widgets and show everything
    gtk_window_set_child(GTK_WINDOW(self), hbox);
    gtk_widget_set_visible(self->infobar, FALSE);
}

static void text_editor_class_init(TextEditorClass *klass) {}

TextEditor *text_editor_new(GtkWindow *parent, int width, int height)
{
    // Create a window with default size
    TextEditor *editor = (TextEditor *)g_object_new(text_editor_get_type(),
                                                    "transient-for", parent, NULL);
    gtk_window_set_default_size(GTK_WINDOW(editor), width, height);
    return editor;
}
