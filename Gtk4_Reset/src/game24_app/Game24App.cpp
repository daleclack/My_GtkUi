#include "Game24.h"
#include "Game24App.h"
#include <cstdlib>
#include "MyTitleBar.h"

static Game24 main_game;

struct _Game24App
{
    GtkWindow parent_instance;
    MyTitleBar *title_bar;
    GtkWidget *label_numbers; // Child widgets
    GtkWidget *entry_ans;
    GtkWidget *btnstart, *btnnext, *btnclear,
        *btncheck, *btnexit;
    GtkWidget *btn0, *btn1, *btn2, *btn3, *btn4,
        *btn5, *btn6, *btn7, *btn8, *btn9;
    GtkWidget *btnadd, *btnsubb, *btnmul, *btndiv,
        *btnleft, *btnright;
    char tmp[20];
    gboolean winned;
};

G_DEFINE_TYPE(Game24App, game24_app, GTK_TYPE_WINDOW)

static void btns_clicked(GtkButton *btn, Game24App *self)
{
    // Get Original text and add number from button

    // 1.Get text buffer for insert
    auto buffer = gtk_entry_get_buffer(GTK_ENTRY(self->entry_ans));

    // Get label from button
    const char *label = gtk_button_get_label(btn);

    // Get Current text
    auto buffer_text = gtk_editable_get_text(GTK_EDITABLE(self->entry_ans));

    // Insert text
    int pos = gtk_editable_get_position(GTK_EDITABLE(self->entry_ans));
    gtk_entry_buffer_insert_text(buffer, pos, label, strlen(label));
    gtk_editable_set_position(GTK_EDITABLE(self->entry_ans), pos + 1);

    // If the cursor position is inside a string, append remain string
    // int length = strlen(buffer_text);
    // std::cout << pos << " " << length << std::endl;
    // if (pos + 1 < length)
    // {
    //     std::string tmp_text(buffer_text);
    //     std::string end_str = tmp_text.substr(pos);
    //     gtk_entry_buffer_insert_text(buffer, pos + 2, end_str.c_str(), end_str.length());
    //     std::cout << end_str << std::endl;
    // }
}

static void btnstart_clicked(GtkButton *btn, Game24App *self)
{
    // If the game is winned, start a new game
    if (self->winned)
    {
        main_game.getnumbers();
        gtk_label_set_label(GTK_LABEL(self->label_numbers), main_game.numbers_char);
    }
    else
    {
        gtk_label_set_label(GTK_LABEL(self->label_numbers), self->tmp);
    }
    auto label_str = gtk_label_get_label(GTK_LABEL(self->label_numbers));
    strncpy(self->tmp, label_str, 20);
}

static void btncheck_clicked(GtkButton *btn, Game24App *self)
{
    // Check Answer
    auto answer = gtk_editable_get_text(GTK_EDITABLE(self->entry_ans));
    self->winned = main_game.startgame(std::string(answer));
    if (self->winned)
    {
        gtk_label_set_label(GTK_LABEL(self->label_numbers), "You Winned!");
    }
    else
    {
        gtk_label_set_label(GTK_LABEL(self->label_numbers), "You Lost!");
    }
}

static void btnclear_clicked(GtkButton *btn, Game24App *self)
{
    // Clear Text input
    gtk_editable_set_text(GTK_EDITABLE(self->entry_ans), "");
}

static void btnnext_clicked(GtkButton *btn, Game24App *self)
{
    main_game.getnumbers();
    gtk_label_set_label(GTK_LABEL(self->label_numbers), main_game.numbers_char);
}

static void game24_app_dispose(GObject *object)
{
    gtk_widget_dispose_template(GTK_WIDGET(object),
                                game24_app_get_type());
    G_OBJECT_CLASS(game24_app_parent_class)->dispose(object);
}

static void game24_app_init(Game24App *self)
{
    // Init Template
    gtk_widget_init_template(GTK_WIDGET(self));
    gtk_window_set_title(GTK_WINDOW(self), "24 Game");
    gtk_window_set_icon_name(GTK_WINDOW(self), "24game");
    self->title_bar = my_titlebar_new();
    my_titlebar_set_window(self->title_bar, self);

    // Default value to start game
    self->winned = TRUE;

    // Link Signals
    g_signal_connect(self->btn0, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btn1, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btn2, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btn3, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btn4, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btn5, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btn6, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btn7, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btn8, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btn9, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btnadd, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btnsubb, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btnmul, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btndiv, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btnleft, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btnright, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btnstart, "clicked", G_CALLBACK(btnstart_clicked), self);
    g_signal_connect(self->btnnext, "clicked", G_CALLBACK(btnnext_clicked), self);
    g_signal_connect(self->btncheck, "clicked", G_CALLBACK(btncheck_clicked), self);
    g_signal_connect(self->btnclear, "clicked", G_CALLBACK(btnclear_clicked), self);
    g_signal_connect_swapped(self->btnexit, "clicked", G_CALLBACK(gtk_window_close), self);
}

static void game24_app_class_init(Game24AppClass *klass)
{
    G_OBJECT_CLASS(klass)->dispose = game24_app_dispose;
    // Set template
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass),
                                                "/org/gtk/daleclack/game24.ui");
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, label_numbers);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, entry_ans);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnstart);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnnext);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btncheck);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnclear);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnexit);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn0);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn1);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn2);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn3);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn4);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn5);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn6);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn7);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn8);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btn9);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnadd);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnsubb);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnmul);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btndiv);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnleft);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), Game24App, btnright);
}

Game24App *game24_app_new(double dpi_value)
{
    // Create main window
    Game24App * app = GAME24_APP(g_object_new(game24_app_get_type(), NULL));
    my_titlebar_set_dpi_scale(app->title_bar, dpi_value);
    return app;
}