#include "GameWin.h"
#include <ctime>
#include <cstdlib>
#include <cstring>
#include "MyTitleBar.h"

struct _GameWin
{
    GtkWindow parent;
    MyTitleBar *title_bar;
    GtkWidget *btn_1, *btn_2, *btn_3, *btn_4;
    GtkWidget *image_btn1, *image_btn2, *image_btn3, *image_btn4;
    GtkWidget *btn_go, *btn_exit;
    GtkWidget *label_status;
    int flag;
};

G_DEFINE_TYPE(GameWin, game_win, GTK_TYPE_WINDOW)

static void check_status(GtkToggleButton *togglebtn, GtkLabel *label)
{
    if (gtk_toggle_button_get_active(togglebtn))
    {
        gtk_label_set_label(label, "Good Luck!");
    }
    else
    {
        gtk_label_set_label(label, "The Button is here!");
    }
}

static void btn_click(GtkWidget *widget, GameWin *win)
{
    // When a button clicked, show image and check if the button with image is clicked
    switch (win->flag)
    {
    case 1:
        gtk_widget_set_visible(win->image_btn1, TRUE);
        check_status(GTK_TOGGLE_BUTTON(win->btn_1), GTK_LABEL(win->label_status));
        break;
    case 2:
        gtk_widget_set_visible(win->image_btn2, TRUE);
        check_status(GTK_TOGGLE_BUTTON(win->btn_2), GTK_LABEL(win->label_status));
        break;
    case 3:
        gtk_widget_set_visible(win->image_btn3, TRUE);
        check_status(GTK_TOGGLE_BUTTON(win->btn_3), GTK_LABEL(win->label_status));
        break;
    case 4:
        gtk_widget_set_visible(win->image_btn4, TRUE);
        check_status(GTK_TOGGLE_BUTTON(win->btn_4), GTK_LABEL(win->label_status));
        break;
    }

    // disable all buttons
    gtk_widget_set_sensitive(win->btn_1, FALSE);
    gtk_widget_set_sensitive(win->btn_2, FALSE);
    gtk_widget_set_sensitive(win->btn_3, FALSE);
    gtk_widget_set_sensitive(win->btn_4, FALSE);
}

static void btngo_click(GtkWidget *widget, GameWin *win)
{
    // Reinitalize the buttons and the flag
    gtk_label_set_label(GTK_LABEL(win->label_status), "Select a button");

    // Reinitalize Button
    gtk_widget_set_visible(win->image_btn1, FALSE);
    gtk_widget_set_visible(win->image_btn2, FALSE);
    gtk_widget_set_visible(win->image_btn3, FALSE);
    gtk_widget_set_visible(win->image_btn4, FALSE);

    // Enable All Buttons
    gtk_widget_set_sensitive(win->btn_1, TRUE);
    gtk_widget_set_sensitive(win->btn_2, TRUE);
    gtk_widget_set_sensitive(win->btn_3, TRUE);
    gtk_widget_set_sensitive(win->btn_4, TRUE);

    // Set All Buttons to inactive
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(win->btn_1), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(win->btn_2), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(win->btn_3), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(win->btn_4), FALSE);

    // Get a new flag
    srand((unsigned)time(NULL));
    int x = rand();
    win->flag = x % 4 + 1;
}

static void game_win_init(GameWin *game_win)
{
    // Initalize Window
    gtk_widget_init_template(GTK_WIDGET(game_win));
    gtk_window_set_icon_name(GTK_WINDOW(game_win), "game");
    game_win->title_bar = my_titlebar_new();
    my_titlebar_set_window(game_win->title_bar, game_win);

    // Initalize Flag
    srand((unsigned)time(NULL));
    game_win->flag = (rand() % 4) + 1;

    // link signals
    g_signal_connect(game_win->btn_1, "clicked", G_CALLBACK(btn_click), game_win);
    g_signal_connect(game_win->btn_2, "clicked", G_CALLBACK(btn_click), game_win);
    g_signal_connect(game_win->btn_3, "clicked", G_CALLBACK(btn_click), game_win);
    g_signal_connect(game_win->btn_4, "clicked", G_CALLBACK(btn_click), game_win);
    g_signal_connect(game_win->btn_go, "clicked", G_CALLBACK(btngo_click), game_win);
    g_signal_connect_swapped(game_win->btn_exit, "clicked", G_CALLBACK(gtk_window_close), game_win);
}

static void game_win_class_init(GameWinClass *klass)
{
    // GTK_WINDOW_CLASS(klass)->close_request=game_win_close;

    // Set template for the game window
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass),
                                                "/org/gtk/daleclack/game1.ui");
    // Get Childe Widgets
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameWin, btn_1);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameWin, btn_2);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameWin, btn_3);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameWin, btn_4);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameWin, btn_go);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameWin, btn_exit);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameWin, label_status);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameWin, image_btn1);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameWin, image_btn2);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameWin, image_btn3);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GameWin, image_btn4);
}

GameWin *game_win_new(GtkWindow *win)
{
    return (GameWin *)g_object_new(game_win_get_type(), "transient-for", win, NULL);
}
