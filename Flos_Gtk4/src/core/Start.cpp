#include "Start.h"
#include "MyDock.h"

static void btnprogram_clicked(GtkWidget *widget, GtkStack *stack)
{
    gtk_stack_set_visible_child_name(stack, "page1");
}

static void btnback_clicked(GtkWidget *widget, GtkStack *stack)
{
    gtk_stack_set_visible_child_name(stack, "page0");
}

static void btnmusic_clicked(GtkWidget *widget, GtkWidget *music_box)
{
    if (gtk_widget_get_visible(music_box))
    {
        gtk_widget_set_visible(music_box, FALSE);
    }
    else
    {
        gtk_widget_set_visible(music_box, TRUE);
    }
}

void add_start(MainWin *win, GtkBox *box)
{
    // Create a builder for menu
    GtkBuilder *startbuilder = gtk_builder_new_from_resource("/org/gtk/daleclack/start.ui");

    // Get Widgets
    GtkWidget *menu_btn = (GtkWidget *)gtk_builder_get_object(startbuilder, "menu_btn");
    GtkWidget *label = gtk_label_new(" ");

    // Stack Change effect
    GtkWidget *stack_program, *btn_program, *btn_back;
    stack_program = (GtkWidget *)gtk_builder_get_object(startbuilder, "stack_program");
    btn_program = (GtkWidget *)gtk_builder_get_object(startbuilder, "btn_program");
    btn_back = (GtkWidget *)gtk_builder_get_object(startbuilder, "btn_back");
    g_signal_connect(btn_program, "clicked", G_CALLBACK(btnprogram_clicked), stack_program);
    g_signal_connect(btn_back, "clicked", G_CALLBACK(btnback_clicked), stack_program);

    // Enable Shutdown(Close)
    GtkWidget *btnshut = (GtkWidget *)gtk_builder_get_object(startbuilder, "btnshut");
    g_signal_connect_swapped(btnshut, "clicked", G_CALLBACK(gtk_window_destroy), win);

    // Add Style for separator
    GtkWidget *prim_separator, *ext_separator;
    prim_separator = (GtkWidget *)gtk_builder_get_object(startbuilder, "prim_separator");
    ext_separator = (GtkWidget *)gtk_builder_get_object(startbuilder, "ext_separator");
    gtk_style_context_add_provider(gtk_widget_get_style_context(prim_separator),
                                   main_win_get_style(win), G_MAXINT);
    gtk_style_context_add_provider(gtk_widget_get_style_context(ext_separator),
                                   main_win_get_style(win), G_MAXINT);

    // Integrated Media Player
    GtkWidget *music_box = (GtkWidget *)gtk_builder_get_object(startbuilder, "music_box");
    GtkWidget *btn_music = (GtkWidget *)gtk_builder_get_object(startbuilder, "btnmusic");
    GtkWidget *media_control = (GtkWidget *)gtk_builder_get_object(startbuilder, "media_ctrl");
    g_signal_connect(btn_music, "clicked", G_CALLBACK(btnmusic_clicked), music_box);

    // Add music resource
    GtkMediaStream *media = gtk_media_file_new_for_resource("/org/gtk/daleclack/Yanni-Nightingale.mp3");
    gtk_media_controls_set_media_stream(GTK_MEDIA_CONTROLS(media_control), media);

    // Get Button for file manager
    GtkWidget *btnfile1, *btnfile2;
    btnfile1 = (GtkWidget *)gtk_builder_get_object(startbuilder, "btnfile1");
    btnfile2 = (GtkWidget *)gtk_builder_get_object(startbuilder, "btnfile2");
    g_signal_connect(btnfile1, "clicked", G_CALLBACK(btnfile_clicked), win);
    g_signal_connect(btnfile2, "clicked", G_CALLBACK(btnfile_clicked), win);

    //Buttons for control panel
    GtkWidget * user_button, * btn_computer;
    user_button = (GtkWidget*)gtk_builder_get_object(startbuilder,"user_button");
    btn_computer = (GtkWidget*)gtk_builder_get_object(startbuilder,"btn_computer");
    g_signal_connect(user_button, "clicked", G_CALLBACK(btnhome_clicked), win);
    g_signal_connect(btn_computer, "clicked", G_CALLBACK(btnfile_clicked), win);

    //When the button clicked, popdown the menu
    GtkWidget * start_menu = (GtkWidget*)gtk_builder_get_object(startbuilder,"main_menu");
    g_signal_connect_swapped(btnfile1,"clicked",G_CALLBACK(gtk_popover_popdown),start_menu);
    g_signal_connect_swapped(btnfile2,"clicked",G_CALLBACK(gtk_popover_popdown),start_menu);
    g_signal_connect_swapped(user_button,"clicked",G_CALLBACK(gtk_popover_popdown),start_menu);
    g_signal_connect_swapped(btn_computer,"clicked",G_CALLBACK(gtk_popover_popdown),start_menu);

    // Pack widgets
    gtk_box_prepend(box, menu_btn);
    gtk_box_prepend(box, label);
}
