#include "ImageApp.h"
#include "MyImage.h"

struct _ImageApp
{
    GtkApplicationWindow parent_instance;
    // Child widgets
    GtkWidget *main_box, *btn_box;
    GtkWidget *image_sw;
    GtkWidget *image_scale, *btnopen;

    // Context Menu
    GtkGesture *gesture_click, *gesture_drag;
    GtkBuilder *builder;
    GtkWidget *image_menu;

    // Main Image View
    MyImage *image_view;
};

G_DEFINE_TYPE(ImageApp, image_app, GTK_TYPE_APPLICATION_WINDOW)

static gboolean image_app_change_scale(GtkRange *range, GtkScrollType *scroll,
                                       double scale_value, ImageApp *app)
{
    my_image_scale_draw(app->image_view, scale_value);
    gtk_range_set_value(range, scale_value);
    return TRUE;
}

static void image_app_zoom_in(GSimpleAction *action,
                              GVariant *parmeter,
                              gpointer data)
{
    // Zoom in: Scale + 0.1
    ImageApp *app = IMAGE_APP(data);
    double value = gtk_range_get_value(GTK_RANGE(app->image_scale));
    value += 0.1;
    if (value > 10.0)
    {
        value = 10.0;
    }
    gtk_range_set_value(GTK_RANGE(app->image_scale), value);
    image_app_change_scale(GTK_RANGE(app->image_scale), NULL, value, app);
}

static void image_app_zoom_out(GSimpleAction *action,
                               GVariant *parmeter,
                               gpointer data)
{
    // Zoom Out: Scale - 0.1
    ImageApp *app = IMAGE_APP(data);
    double value = gtk_range_get_value(GTK_RANGE(app->image_scale));
    value -= 0.1;
    if (value < 0.0)
    {
        value = 0.0;
    }
    gtk_range_set_value(GTK_RANGE(app->image_scale), value);
    image_app_change_scale(GTK_RANGE(app->image_scale), NULL, value, app);
}

static void image_app_zoom_reset(GSimpleAction *action,
                                 GVariant *parmeter,
                                 gpointer data)
{
    // 1:1 Scale=1
    ImageApp *app = IMAGE_APP(data);
    gtk_range_set_value(GTK_RANGE(app->image_scale), 1.0);
    image_app_change_scale(GTK_RANGE(app->image_scale), NULL, 1.0, app);
}

static void gesture_pressed(GtkGesture *gesture,
                            int n_press,
                            double x,
                            double y,
                            ImageApp *app)
{
    GdkRectangle pos;
    pos.height = 1;
    pos.width = 1;
    pos.x = x;
    pos.y = y;
    // Set popover position and show
    gtk_popover_set_pointing_to(GTK_POPOVER(app->image_menu), &pos);
    gtk_popover_popup(GTK_POPOVER(app->image_menu));
}

static void image_app_dialog_response(GObject *dialog, GAsyncResult *result, gpointer data)
{
    GFile *file;
    ImageApp *app = IMAGE_APP(data);
    file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(dialog), result, NULL);
    if (file)
    {
        // Open a image and show it
        GdkPixbuf *pixbuf;
        pixbuf = gdk_pixbuf_new_from_file(g_file_get_path(file), NULL);
        my_image_set_source_pixbuf(app->image_view, pixbuf);
        g_object_unref(file);
    }
}

static void btnopen_clicked(GtkButton *btn, ImageApp *self)
{
    // Create a file dialog and show
    GtkFileDialog *dialog;
    dialog = gtk_file_dialog_new();
    gtk_file_dialog_open(dialog, GTK_WINDOW(self), NULL, image_app_dialog_response, self);
}

static void image_app_init(ImageApp *self)
{
    // Initalize window
    gtk_window_set_title(GTK_WINDOW(self), "Image Viewer");
    gtk_window_set_default_size(GTK_WINDOW(self), 800, 450);
    gtk_window_set_icon_name(GTK_WINDOW(self), "image_app");

    // Add action for menu
    GActionEntry entries[] =
        {
            {"zoom_out", image_app_zoom_out, NULL, NULL, NULL},
            {"zoom_in", image_app_zoom_in, NULL, NULL, NULL},
            {"zoom_reset", image_app_zoom_reset, NULL, NULL, NULL},
        };
    g_action_map_add_action_entries(G_ACTION_MAP(self), entries,
                                    G_N_ELEMENTS(entries), self);

    // Create child widgets
    self->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    self->btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    self->image_view = my_image_new();
    self->image_sw = gtk_scrolled_window_new();
    self->image_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.1, 10.0, 0.1);
    self->btnopen = gtk_button_new_with_label("Open Image");
    g_signal_connect(self->btnopen, "clicked", G_CALLBACK(btnopen_clicked), self);
    g_signal_connect(self->image_scale, "change-value", G_CALLBACK(image_app_change_scale), self);

    // Initalize widgets
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(self->image_sw),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_hexpand(self->image_sw, TRUE);
    gtk_widget_set_vexpand(self->image_sw, TRUE);
    gtk_widget_set_hexpand(self->image_scale, TRUE);
    gtk_range_set_value(GTK_RANGE(self->image_scale), 1.0);
    gtk_scale_set_draw_value(GTK_SCALE(self->image_scale), TRUE);
    gtk_scale_set_value_pos(GTK_SCALE(self->image_scale), GTK_POS_LEFT);

    // Add gesture for context menu
    self->gesture_click = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(self->gesture_click), GDK_BUTTON_SECONDARY);
    gtk_widget_add_controller(GTK_WIDGET(self->image_view), GTK_EVENT_CONTROLLER(self->gesture_click));
    g_signal_connect(self->gesture_click, "pressed", G_CALLBACK(gesture_pressed), self);

    // Add a menu
    self->builder = gtk_builder_new_from_resource("/org/gtk/daleclack/image_appmenu.xml");
    GMenuModel *model = G_MENU_MODEL(gtk_builder_get_object(self->builder, "model"));
    self->image_menu = gtk_popover_menu_new_from_model(model);
    gtk_popover_set_has_arrow(GTK_POPOVER(self->image_menu), FALSE);
    gtk_widget_set_parent(self->image_menu, GTK_WIDGET(self->image_view));
    gtk_popover_present(GTK_POPOVER(self->image_menu));

    // Pack Widgets
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(self->image_sw), GTK_WIDGET(self->image_view));
    gtk_box_append(GTK_BOX(self->main_box), self->image_sw);
    gtk_box_append(GTK_BOX(self->btn_box), self->image_scale);
    gtk_box_append(GTK_BOX(self->btn_box), self->btnopen);
    gtk_box_append(GTK_BOX(self->main_box), self->btn_box);
    gtk_window_set_child(GTK_WINDOW(self), self->main_box);
}

static void image_app_class_init(ImageAppClass *klass)
{
}

ImageApp *image_app_new()
{
    return IMAGE_APP(g_object_new(image_app_get_type(), NULL));
}
