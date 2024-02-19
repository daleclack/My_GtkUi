#include "DrawApp.h"
#include "MyFinder.h"

struct _DrawApp
{
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
    GtkGesture *drag;
    double start_x, start_y;
    double prev_x, prev_y;
};

G_DEFINE_TYPE(DrawApp, draw_app, GTK_TYPE_WINDOW)

// Create a surface for drawing area
static void create_surface(DrawApp *app)
{
    cairo_t *cr;
    GtkWidget *widget = app->draw_area;

    if (app->surface)
        cairo_surface_destroy(app->surface);

    app->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                              gtk_widget_get_width(widget),
                                              gtk_widget_get_height(widget));

    /* Initialize the surface to white */
    cr = cairo_create(app->surface);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_destroy(cr);
}

static void draw_app_resize(GtkWidget *widget,
                            int width,
                            int height,
                            DrawApp *app)
{
    create_surface(app);
}

/* Redraw the screen from the surface */
static void draw_app_draw(GtkDrawingArea *da,
                          cairo_t *cr,
                          int width,
                          int height,
                          gpointer data)
{
    DrawApp *app = DRAW_APP(data);
    cairo_set_source_surface(cr, app->surface, 0, 0);
    cairo_paint(cr);
}

static void draw_brush(DrawApp *app, double x, double y, gboolean first)
{
    GdkRectangle update_rect;
    cairo_t *cr;
    GtkWidget *widget = app->draw_area;

    if (app->surface == NULL ||
        cairo_image_surface_get_width(app->surface) != gtk_widget_get_width(widget) ||
        cairo_image_surface_get_height(app->surface) != gtk_widget_get_height(widget))
        create_surface(app);

    update_rect.x = x - 3;
    update_rect.y = y - 3;
    update_rect.width = 6;
    update_rect.height = 6;

    /* Paint to the surface, where we store our state */
    cr = cairo_create(app->surface);
    cairo_set_line_width(cr, 6);

    gdk_cairo_rectangle(cr, &update_rect);
    cairo_fill(cr);

    // Draw lines to link points
    if (!first)
    {
        cairo_move_to(cr, app->prev_x, app->prev_y);
        cairo_line_to(cr, x, y);
        cairo_stroke(cr);
    }

    // Update position for next point
    app->prev_x = x;
    app->prev_y = y;

    cairo_destroy(cr);

    gtk_widget_queue_draw(widget);
}

static void drag_begin(GtkGestureDrag *gesture,
                       double x,
                       double y,
                       DrawApp *app)
{
    app->start_x = x;
    app->start_y = y;
    draw_brush(app, x, y, TRUE);
}

static void drag_update(GtkGestureDrag *gesture,
                        double x,
                        double y,
                        DrawApp *app)
{
    draw_brush(app, (app->start_x) + x, (app->start_y) + y, FALSE);
}

static void drag_end(GtkGestureDrag *gesture,
                     double x,
                     double y,
                     DrawApp *app)
{
    draw_brush(app, (app->start_x) + x, (app->start_y) + y, FALSE);
}

static GtkWidget *my_toggle_button_new_from_icon_name(DrawApp *self, const char *icon_name)
{
    char icon_name1[NAME_MAX + 1];
    GtkWidget *toggle_button = gtk_toggle_button_new();
    strncpy(icon_name1, icon_name, NAME_MAX);
    if (get_dark_mode(self))
    {
        strncat(icon_name1, "-dark", 5);
    }
    gtk_button_set_icon_name(GTK_BUTTON(toggle_button), icon_name1);
    return toggle_button;
}

static void draw_app_init(DrawApp *self)
{
    // Initalize window
    gtk_window_set_title(GTK_WINDOW(self), "Drawing App");
    gtk_window_set_icon_name(GTK_WINDOW(self), "drawing_app");
    // gtk_window_set_default_size(GTK_WINDOW(self), 640, 480);

    // Create widgets
    self->left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    self->main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    self->btn_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // Add buttons for left box
    self->btn_free = my_toggle_button_new_from_icon_name(self, "freehand");
    self->btn_circle = my_toggle_button_new_from_icon_name(self, "circle");
    gtk_toggle_button_set_group(GTK_TOGGLE_BUTTON(self->btn_circle), GTK_TOGGLE_BUTTON(self->btn_free));
    self->btn_line = my_toggle_button_new_from_icon_name(self, "line");
    gtk_toggle_button_set_group(GTK_TOGGLE_BUTTON(self->btn_line), GTK_TOGGLE_BUTTON(self->btn_free));
    self->btn_rectangle = my_toggle_button_new_from_icon_name(self, "rectangle");
    gtk_toggle_button_set_group(GTK_TOGGLE_BUTTON(self->btn_rectangle), GTK_TOGGLE_BUTTON(self->btn_free));

    // Create Drawing Area
    self->draw_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(self->draw_area, 640, 480);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(self->draw_area), 640);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(self->draw_area), 480);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(self->draw_area), draw_app_draw, self, NULL);
    g_signal_connect(self->draw_area, "resize", G_CALLBACK(draw_app_resize), self);

    // Create Gesture for drawing
    self->drag = gtk_gesture_drag_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(self->drag), 0);
    gtk_widget_add_controller(self->draw_area, GTK_EVENT_CONTROLLER(self->drag));
    g_signal_connect(self->drag, "drag-begin", G_CALLBACK(drag_begin), self);
    g_signal_connect(self->drag, "drag-update", G_CALLBACK(drag_update), self);
    g_signal_connect(self->drag, "drag-end", G_CALLBACK(drag_end), self);

    // Add widget to the window
    gtk_box_append(GTK_BOX(self->left_box), self->btn_free);
    gtk_box_append(GTK_BOX(self->left_box), self->btn_circle);
    gtk_box_append(GTK_BOX(self->left_box), self->btn_line);
    gtk_box_append(GTK_BOX(self->left_box), self->btn_rectangle);
    gtk_box_append(GTK_BOX(self->main_box), self->left_box);
    gtk_box_append(GTK_BOX(self->main_box), self->draw_area);
    gtk_box_append(GTK_BOX(self->main_box), self->btn_box);
    gtk_window_set_child(GTK_WINDOW(self), self->main_box);
}

static void draw_app_class_init(DrawAppClass *klass)
{
}

DrawApp *draw_app_new()
{
    return DRAW_APP(g_object_new(draw_app_get_type(), NULL));
}
