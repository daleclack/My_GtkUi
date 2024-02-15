#include "TextEditor.h"

struct _TextEditor
{
    GtkApplicationWindow parent_instance;
    GtkWidget *header;
    GtkWidget *main_box;
    GtkWidget *text_view, *scrolled_win;
};

G_DEFINE_TYPE(TextEditor, text_editor, GTK_TYPE_APPLICATION_WINDOW)

static void text_editor_init(TextEditor *self)
{
    // Use headerbar for title and more info
    self->header = gtk_header_bar_new();
    gtk_window_set_title(GTK_WINDOW(self), "Text editor");
    gtk_window_set_titlebar(GTK_WINDOW(self), self->header);
    gtk_window_set_default_size(GTK_WINDOW(self), 800, 450);

    // Create box for layouts
    self->main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    self->scrolled_win = gtk_scrolled_window_new();

    // Create text view
    self->text_view = gtk_text_view_new();
    gtk_widget_set_hexpand(self->text_view, TRUE);
    gtk_widget_set_vexpand(self->text_view, TRUE);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(self->scrolled_win), self->text_view);

    gtk_box_append(GTK_BOX(self->main_box), self->scrolled_win);
    gtk_window_set_child(GTK_WINDOW(self), self->main_box);
}

static void text_editor_class_init(TextEditorClass *klass)
{
}

TextEditor *text_editor_new()
{
    return TEXT_EDITOR(g_object_new(text_editor_get_type(), NULL));
}