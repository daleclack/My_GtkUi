#include "drawing.h"
#include <cstdio>
#include "MyTitleBar.h"

struct _DrawingApp
{
    GtkWindow parent_instance;
    MyTitleBar *title_bar;
    cairo_surface_t *cairo_surface = NULL;
    GtkWidget *draw_area;
    GtkWidget *color_button;
    GtkColorDialog *color_dialog;
    double x, y;
};

G_DEFINE_TYPE(DrawingApp, drawing_app, GTK_TYPE_WINDOW)

static void drawing_app_create_surface(DrawingApp *self)
{
    cairo_t *cr;

    // Create Cairo Surface
    if (self->cairo_surface)
    {
        cairo_surface_destroy(self->cairo_surface);
    }

    self->cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                                     gtk_widget_get_width(self->draw_area),
                                                     gtk_widget_get_height(self->draw_area));

    /* Initialize the surface to white */
    cr = cairo_create(self->cairo_surface);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_destroy(cr);
}

static void resize_cb(GtkWidget *widget, int width, int height, DrawingApp *draw_app)
{
    drawing_app_create_surface(draw_app);
}

static void draw_cb(GtkDrawingArea *area, cairo_t *cr, int width, int height, DrawingApp *draw_app)
{
    cairo_set_source_surface(cr, draw_app->cairo_surface, 0, 0);
    cairo_paint(cr);
    // Set Color
    //  guint width, height;
    //  width = gtk_widget_get_allocated_width (draw_app->draw_area);
    //  height = gtk_widget_get_allocated_height (draw_app->draw_area);
    //  cairo_arc (cr,
    //           width / 2.0, height / 2.0,
    //           MIN (width, height) / 2.0,
    //           0, 2 * G_PI);
    //  cairo_fill(cr);
}

static void draw_brush(DrawingApp *draw_app, double x, double y)
{
    cairo_t *cr;
    // Draw on the surface
    cr = cairo_create(draw_app->cairo_surface);
    // Draw and fill in color
    const GdkRGBA *color_set;
    // gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(draw_app->color_button),&color_set);
    color_set = gtk_color_dialog_button_get_rgba(GTK_COLOR_DIALOG_BUTTON(draw_app->color_button));
    cairo_set_source_rgba(cr, color_set->red, color_set->green,
                          color_set->blue, color_set->alpha);
    cairo_arc(cr, x, y, 3, 0, 2 * G_PI);

    cairo_fill(cr);

    cairo_destroy(cr);
    // g_print("%f %f\n",x,y);

    gtk_widget_queue_draw(draw_app->draw_area);
}

static void drag_begin(GtkGesture *gesture, double x, double y, DrawingApp *draw_app)
{
    draw_app->x = x;
    draw_app->y = y;
    draw_brush(draw_app, x, y);
    /*
    freopen("drag.txt","a",stdout);
    g_print("%lf %lf\n",x,y);
    fclose(stdout);
    */
}

static void drag_update(GtkGesture *gesture, double x, double y, DrawingApp *draw_app)
{
    draw_brush(draw_app, x + (draw_app->x), y + (draw_app->y));
    /*
    freopen("drag.txt","a",stdout);
    g_print("%lf %lf\n",x+draw_app->x,y+draw_app->y);
    fclose(stdout);
    */
}

static void drag_end(GtkGesture *gesture, double x, double y, DrawingApp *draw_app)
{
    draw_brush(draw_app, x + (draw_app->x), (y + draw_app->y));
    /*
    freopen("drag.txt","a",stdout);
    g_print("%lf %lf\n",x+pos->x,y+pos->y);
    fclose(stdout);
    */
}

static void btnclear_clicked(GtkWidget *widget, DrawingApp *draw_app)
{
    drawing_app_create_surface(draw_app);
    gtk_widget_queue_draw(draw_app->draw_area);
}

static void close_window(DrawingApp *self, gpointer data)
{
    if (self->cairo_surface)
    {
        cairo_surface_destroy(self->cairo_surface);
    }
}

static void drawing_app_init(DrawingApp *self)
{
    // Initalize Drawing Window
    self->title_bar = my_titlebar_new();
    my_titlebar_set_window(self->title_bar, self);
    gtk_window_set_title(GTK_WINDOW(self), "Drawing Test");
    gtk_window_set_default_size(GTK_WINDOW(self), 640, 360);
    gtk_window_set_icon_name(GTK_WINDOW(self), "drawing_app");

    GtkWidget *hbox, *frame, *btnbox;
    GtkWidget *btn_clear, *btn_exit;
    GtkGesture *drag;

    // Create box and frame
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    frame = gtk_frame_new("Drawing Area");
    gtk_widget_set_hexpand(frame, TRUE);
    btnbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // Create a draw area
    self->draw_area = gtk_drawing_area_new();
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(self->draw_area),
                                   (GtkDrawingAreaDrawFunc)draw_cb, self, NULL);
    g_signal_connect_after(self->draw_area, "resize", G_CALLBACK(resize_cb), self);

    // Create Buttons
    btn_clear = gtk_button_new_with_label("Clear");
    btn_exit = gtk_button_new_with_label("Exit");

    // Default Color
    GdkRGBA color_set = {0, 0, 0, 1};
    self->color_dialog = gtk_color_dialog_new();
    self->color_button = gtk_color_dialog_button_new(self->color_dialog);
    gtk_color_dialog_button_set_rgba(GTK_COLOR_DIALOG_BUTTON(self->color_button), &color_set);
    // self->color_button = gtk_color_button_new_with_rgba(&color_set);

    gtk_box_append(GTK_BOX(btnbox), self->color_button);
    gtk_box_append(GTK_BOX(btnbox), btn_clear);
    gtk_box_append(GTK_BOX(btnbox), btn_exit);
    gtk_widget_set_valign(btnbox, GTK_ALIGN_CENTER);

    // Create Surface
    drawing_app_create_surface(self);
    g_assert(self->cairo_surface);

    //"Drag" gesture
    drag = gtk_gesture_drag_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(drag), GDK_BUTTON_PRIMARY);
    gtk_widget_add_controller(self->draw_area, GTK_EVENT_CONTROLLER(drag));
    g_signal_connect(drag, "drag-begin", G_CALLBACK(drag_begin), self);
    g_signal_connect(drag, "drag-update", G_CALLBACK(drag_update), self);
    g_signal_connect(drag, "drag-end", G_CALLBACK(drag_end), self);

    // Connect the "click" signal of clear button
    g_signal_connect(btn_clear, "clicked", G_CALLBACK(btnclear_clicked), self);

    // Connect the "click" signal of exit button
    g_signal_connect_swapped(btn_exit, "clicked", G_CALLBACK(gtk_window_close), self);

    // Link to the "destroy" signal
    g_signal_connect(self, "destroy", G_CALLBACK(close_window), NULL);

    gtk_frame_set_child(GTK_FRAME(frame), self->draw_area);
    gtk_box_append(GTK_BOX(hbox), frame);
    gtk_box_append(GTK_BOX(hbox), btnbox);
    gtk_window_set_child(GTK_WINDOW(self), hbox);
}

static void drawing_app_class_init(DrawingAppClass *klass)
{
}

DrawingApp *drawing_app_new(GtkWindow *parent_win)
{
    return (DrawingApp *)g_object_new(drawing_app_get_type(), "transient-for", parent_win, NULL);
}
