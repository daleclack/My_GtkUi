#include "DrawApp.h"

struct _DrawApp{
    GtkWindow parent_instance;

    // Child widgets
    GtkWidget *draw_area;
    GtkWidget *color_btn, *fill_btn;
    GtkWidget *fill_check;
    GtkWidget *main_label, *size_label;
    GtkWidget *left_box, *main_box, *btn_box;
    GtkWidget *btn_free, *btn_line, *btn_circle, *btn_rectangle;
    GtkWidget *size_scale;

    // Drawing Area
    cairo_surface_t *surface;
};

G_DEFINE_TYPE(DrawApp, draw_app, GTK_TYPE_WINDOW)

static void draw_app_init(DrawApp *self)
{
    gtk_window_set_title(GTK_WINDOW(self), "Drawing App");
}

static void draw_app_class_init(DrawAppClass *klass)
{
}

DrawApp *draw_app_new()
{
    return DRAW_APP(g_object_new(draw_app_get_type(), NULL));
}
