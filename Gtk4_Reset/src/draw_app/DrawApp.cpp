#include "DrawApp.h"
#include "MyFinder.h"
#include "MyLimit.h"
#include "MyTitleBar.h"

typedef enum
{
    Freehand,
    Circle,
    Line,
    Rectangle
} DrawMode;

typedef enum
{
    Begin,
    Update,
    End
} DrawProc;

struct _DrawApp
{
    GtkWindow parent_instance;

    // Custom Title Bar
    MyTitleBar *title_bar;

    // Child widgets
    GtkWidget *draw_area;
    GtkWidget *fill_check;
    GtkWidget *left_box, *main_box, *btn_box;
    GtkWidget *btn_free, *btn_line, *btn_circle, *btn_rectangle;
    GtkWidget *btn_fill, *btn_color, *label_color, *label_size;
    GtkWidget *size_scale;
    GtkWidget *btn_save, *btn_clear, *btn_exit;
    GtkColorDialog *fill_dialog, *pen_dialog;

    // Drawing Area
    cairo_surface_t *surface;
    GtkGesture *drag;
    double start_x, start_y;
    double prev_x, prev_y;

    // Draw Mode config
    DrawMode draw_mode;
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

static void draw_brush(DrawApp *app, double x, double y, DrawProc process)
{
    GdkRectangle update_rect;
    cairo_t *cr;
    GtkWidget *widget = app->draw_area;
    const GdkRGBA *pen_color, *fill_color;

    if (app->surface == NULL ||
        cairo_image_surface_get_width(app->surface) != gtk_widget_get_width(widget) ||
        cairo_image_surface_get_height(app->surface) != gtk_widget_get_height(widget))
        create_surface(app);

    // Get Color configs
    pen_color = gtk_color_dialog_button_get_rgba(GTK_COLOR_DIALOG_BUTTON(app->btn_color));
    fill_color = gtk_color_dialog_button_get_rgba(GTK_COLOR_DIALOG_BUTTON(app->btn_fill));

    // Get config for fill color
    gboolean fill = gtk_check_button_get_active(GTK_CHECK_BUTTON(app->fill_check));

    /* Paint to the surface, where we store our state */
    cr = cairo_create(app->surface);

    // Get Line width config
    double line_width = gtk_range_get_value(GTK_RANGE(app->size_scale));
    cairo_set_line_width(cr, line_width);

    // Color for line and freehand drawing
    gdk_cairo_set_source_rgba(cr, pen_color);

    // Draw with the mode
    switch (app->draw_mode)
    {
    case DrawMode::Freehand:
        update_rect.x = x - (line_width / 2.0);
        update_rect.y = y - (line_width / 2.0);
        update_rect.width = line_width;
        update_rect.height = line_width;

        gdk_cairo_rectangle(cr, &update_rect);
        cairo_fill(cr);

        // Draw lines to link points
        if (process != DrawProc::Begin)
        {
            cairo_move_to(cr, app->prev_x, app->prev_y);
            cairo_line_to(cr, x, y);
            cairo_stroke(cr);
        }

        // Update position for next point
        app->prev_x = x;
        app->prev_y = y;
        break;
    case DrawMode::Circle:
        if (process == DrawProc::End)
        {
            cairo_move_to(cr, app->start_x, app->start_y);
            double x1 = fabs(x - (app->start_x));
            double y1 = fabs(y - (app->start_y));
            double radios = sqrt(x1 * x1 + y1 * y1);
            cairo_arc(cr, app->start_x, app->start_y, radios, 0, 2.0 * G_PI);
            cairo_stroke(cr);

            // If fill check is enable, fill color
            if(fill)
            {
                gdk_cairo_set_source_rgba(cr, fill_color);
                cairo_arc(cr, app->start_x, app->start_y, radios, 0, 2.0 * G_PI);
                cairo_fill(cr);
            }
        }
        break;
    case DrawMode::Line:
        if (process == DrawProc::End)
        {
            cairo_move_to(cr, app->start_x, app->start_y);
            cairo_line_to(cr, x, y);
            cairo_stroke(cr);
        }
        break;
    case DrawMode::Rectangle:
        if (process == DrawProc::End)
        {
            double width = fabs(x - (app->start_x));
            double height = fabs(y - (app->start_y));
            cairo_rectangle(cr, app->start_x, app->start_y, width, height);
            cairo_stroke(cr);

            // If fill check is enable, fill color
            if(fill)
            {
                gdk_cairo_set_source_rgba(cr, fill_color);
                cairo_rectangle(cr, app->start_x, app->start_y, width, height);
                cairo_fill(cr);
            }
        }
        break;
    }

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
    draw_brush(app, x, y, DrawProc::Begin);
}

static void drag_update(GtkGestureDrag *gesture,
                        double x,
                        double y,
                        DrawApp *app)
{
    draw_brush(app, (app->start_x) + x, (app->start_y) + y, DrawProc::Update);
}

static void drag_end(GtkGestureDrag *gesture,
                     double x,
                     double y,
                     DrawApp *app)
{
    draw_brush(app, (app->start_x) + x, (app->start_y) + y, DrawProc::End);
}

static void btnfree_clicked(GtkButton *btn, DrawApp *self)
{
    self->draw_mode = DrawMode::Freehand;
}

static void btncircle_clicked(GtkButton *btn, DrawApp *self)
{
    self->draw_mode = DrawMode::Circle;
}

static void btnline_clicked(GtkButton *btn, DrawApp *self)
{
    self->draw_mode = DrawMode::Line;
}

static void btnrectangle_clicked(GtkButton *btn, DrawApp *self)
{
    self->draw_mode = DrawMode::Rectangle;
}

static void dialog_response(GObject *dialog, GAsyncResult *result, gpointer data)
{
    DrawApp *draw_app = DRAW_APP(data);
    GFile *file;
    file = gtk_file_dialog_save_finish(GTK_FILE_DIALOG(dialog), result, NULL);
    if (file)
    {
        char path[PATH_MAX];
        char *file_name = g_file_get_path(file);
        strncpy(path, file_name, strlen(file_name));
        strncat(path, ".png", 4);
        cairo_surface_write_to_png(draw_app->surface, path);
        g_free(file_name);
    }
}

static void btnsave_clicked(GtkButton *btn, DrawApp *self)
{
    GtkFileDialog *dialog;
    dialog = gtk_file_dialog_new();
    gtk_file_dialog_save(dialog, GTK_WINDOW(self), NULL, dialog_response, self);
}

static void btnclear_clicked(GtkButton *btn, DrawApp *self)
{
    if (self->surface)
    {
        // Paint to white to clear
        cairo_t *cr = cairo_create(self->surface);
        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_paint(cr);
        cairo_destroy(cr);
        gtk_widget_queue_draw(self->draw_area);
    }
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
    self->title_bar = my_titlebar_new();
    my_titlebar_set_window(self->title_bar, self);
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
    g_signal_connect(self->btn_free, "clicked", G_CALLBACK(btnfree_clicked), self);
    g_signal_connect(self->btn_circle, "clicked", G_CALLBACK(btncircle_clicked), self);
    g_signal_connect(self->btn_line, "clicked", G_CALLBACK(btnline_clicked), self);
    g_signal_connect(self->btn_rectangle, "clicked", G_CALLBACK(btnrectangle_clicked), self);

    // Default Draw Mode Config
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(self->btn_free), TRUE);
    self->draw_mode = DrawMode::Freehand;

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

    // Create other widgets
    self->fill_check = gtk_check_button_new_with_label("Enable Fill Color");
    self->fill_dialog = gtk_color_dialog_new();
    self->btn_fill = gtk_color_dialog_button_new(self->fill_dialog);
    self->label_color = gtk_label_new("Select a color");
    self->pen_dialog = gtk_color_dialog_new();
    self->btn_color = gtk_color_dialog_button_new(self->pen_dialog);
    self->label_size = gtk_label_new("Pen Size");
    self->size_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.1, 20.0, 0.1);
    self->btn_save = gtk_button_new_with_label("Save to png");
    self->btn_clear = gtk_button_new_with_label("Clear Board");
    self->btn_exit = gtk_button_new_with_label("Exit");

    // Properties for the buttons
    GdkRGBA color = {0, 0, 0, 1};
    gtk_color_dialog_button_set_rgba(GTK_COLOR_DIALOG_BUTTON(self->btn_fill), &color);
    gtk_color_dialog_button_set_rgba(GTK_COLOR_DIALOG_BUTTON(self->btn_color), &color);
    gtk_range_set_value(GTK_RANGE(self->size_scale), 3.0);
    gtk_scale_set_draw_value(GTK_SCALE(self->size_scale), TRUE);
    gtk_scale_set_value_pos(GTK_SCALE(self->size_scale), GTK_POS_BOTTOM);
    gtk_widget_set_valign(self->btn_box, GTK_ALIGN_CENTER);
    g_signal_connect(self->btn_save, "clicked", G_CALLBACK(btnsave_clicked), self);
    g_signal_connect(self->btn_clear, "clicked", G_CALLBACK(btnclear_clicked), self);
    g_signal_connect_swapped(self->btn_exit, "clicked", G_CALLBACK(gtk_window_close), self);

    // Add widget to the window
    gtk_box_append(GTK_BOX(self->left_box), self->btn_free);
    gtk_box_append(GTK_BOX(self->left_box), self->btn_circle);
    gtk_box_append(GTK_BOX(self->left_box), self->btn_line);
    gtk_box_append(GTK_BOX(self->left_box), self->btn_rectangle);
    gtk_box_append(GTK_BOX(self->main_box), self->left_box);
    gtk_box_append(GTK_BOX(self->main_box), self->draw_area);
    gtk_box_append(GTK_BOX(self->btn_box), self->fill_check);
    gtk_box_append(GTK_BOX(self->btn_box), self->btn_fill);
    gtk_box_append(GTK_BOX(self->btn_box), self->label_color);
    gtk_box_append(GTK_BOX(self->btn_box), self->btn_color);
    gtk_box_append(GTK_BOX(self->btn_box), self->label_size);
    gtk_box_append(GTK_BOX(self->btn_box), self->size_scale);
    gtk_box_append(GTK_BOX(self->btn_box), self->btn_save);
    gtk_box_append(GTK_BOX(self->btn_box), self->btn_clear);
    gtk_box_append(GTK_BOX(self->btn_box), self->btn_exit);
    gtk_box_append(GTK_BOX(self->main_box), self->btn_box);
    gtk_window_set_child(GTK_WINDOW(self), self->main_box);
}

static void draw_app_class_init(DrawAppClass *klass)
{
}

DrawApp *draw_app_new(double dpi_value)
{
    DrawApp *app = DRAW_APP(g_object_new(draw_app_get_type(), NULL));
    my_titlebar_set_dpi_scale(app->title_bar, dpi_value);
    return app;
}
