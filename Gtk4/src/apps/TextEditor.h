#pragma once

#include <gtk/gtk.h>
#include <cstdio>

G_DECLARE_FINAL_TYPE(TextEditor, text_editor, TEXT, EDITOR, GtkWindow)

TextEditor *text_editor_new(GtkWindow *parent, int width, int height);
