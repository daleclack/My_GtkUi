#pragma once

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(TextEditor, text_editor, TEXT, EDITOR, GtkApplicationWindow)

TextEditor *text_editor_new();
