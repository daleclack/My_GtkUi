#include "TextEditor.h"

struct _TextEditor
{
    GtkApplicationWindow parent_instance;
};

G_DEFINE_TYPE(TextEditor, text_editor, GTK_TYPE_APPLICATION_WINDOW)

static void text_editor_init(TextEditor *self)
{
    gtk_window_set_title(GTK_WINDOW(self), "Text editor");
}

static void text_editor_class_init(TextEditorClass *klass)
{

}

TextEditor *text_editor_new()
{
    return TEXT_EDITOR(g_object_new(text_editor_get_type(), NULL));
}