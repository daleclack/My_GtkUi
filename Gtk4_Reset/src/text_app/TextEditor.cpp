#include "TextEditor.h"

struct _TextEditor
{
    GtkApplicationWindow parent_instance;
    GtkWidget *header;
    GtkWidget *main_box;
    GtkWidget *text_view, *scrolled_win;
    GtkTextBuffer *text_buffer;
    GtkWidget *menu_btn, *popover;
    GtkBuilder *menu_builder, *expander_builder;
    GtkWidget *expander;
    GtkWidget *btns[26];
    GtkWidget *btnshift, *btntab, *btn_caps, *btnenter;
    int width, height;
};

G_DEFINE_TYPE(TextEditor, text_editor, GTK_TYPE_APPLICATION_WINDOW)

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
    }else{
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

static void text_editor_init(TextEditor *self)
{
    // Use headerbar for title and more info
    self->header = gtk_header_bar_new();
    gtk_window_set_title(GTK_WINDOW(self), "Text editor");
    gtk_window_set_titlebar(GTK_WINDOW(self), self->header);
    gtk_window_set_default_size(GTK_WINDOW(self), 800, 450);

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

    // Add info area
    

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
    for(int i = 0; i < 26; i++)
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