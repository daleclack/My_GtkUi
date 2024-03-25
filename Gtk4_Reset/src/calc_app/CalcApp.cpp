#include "CalcApp.h"
#include "calc.h"
#include "MyTitleBar.h"
#include <string>
#include <iostream>

struct _CalcApp
{
    GtkWindow parent_instance;
    MyTitleBar *title_bar;
    GtkWidget *entry_ans; // Child widgets
    GtkWidget *btnanswer, *btnback, *btnclear;
    GtkWidget *btn0, *btn1, *btn2, *btn3, *btn4,
        *btn5, *btn6, *btn7, *btn8, *btn9;
    GtkWidget *btnadd, *btnsubb, *btnmul, *btndiv,
        *btnleft, *btnright, *btnpoint, *btndiv100,
        *btnsqrt, *btnpow;
    gboolean focused;
};

G_DEFINE_TYPE(CalcApp, calc_app, GTK_TYPE_WINDOW)

static void calc_app_dispose(GObject *object)
{
    gtk_widget_dispose_template(GTK_WIDGET(object),
                                calc_app_get_type());
    G_OBJECT_CLASS(calc_app_parent_class)->dispose(object);
}

static void btns_clicked(GtkButton *btn, CalcApp *self)
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

static void btnpow_clicked(GtkButton *btn, CalcApp *self)
{
    auto text = gtk_editable_get_text(GTK_EDITABLE(self->entry_ans));
    std::string tmp_str(text);
    tmp_str += "^2";
    gtk_editable_set_text(GTK_EDITABLE(self->entry_ans), tmp_str.c_str());
}

static void btnsqrt_clicked(GtkButton *btn, CalcApp *self)
{
    auto text = gtk_editable_get_text(GTK_EDITABLE(self->entry_ans));
    std::string tmp_str(text);
    tmp_str += "^(1/2)";
    gtk_editable_set_text(GTK_EDITABLE(self->entry_ans), tmp_str.c_str());
}

static void btnback_clicked(GtkButton *btn, CalcApp *self)
{
    // Get text buffer for backspace
    auto buffer = gtk_entry_get_buffer(GTK_ENTRY(self->entry_ans));
    auto pos = gtk_editable_get_position(GTK_EDITABLE(self->entry_ans));
    gtk_entry_buffer_delete_text(buffer, pos, 1);
    gtk_editable_set_position(GTK_EDITABLE(self->entry_ans), pos - 1);
}

static void btnclear_clicked(GtkButton *btn, CalcApp *self)
{
    // Clear text
    gtk_editable_set_text(GTK_EDITABLE(self->entry_ans), "");
}

static void btnanswer_clicked(GtkButton *btn, CalcApp *self)
{
    // Get Expression
    const gchar *expression = gtk_editable_get_text(GTK_EDITABLE(self->entry_ans));

    // Calculation
    calc_reset();
    double result = calc_expression_value(expression);
    char result_str[40];
    sprintf(result_str, "%f", result);
    gtk_editable_set_text(GTK_EDITABLE(self->entry_ans), result_str);
}

static void calc_app_init(CalcApp *self)
{
    gtk_widget_init_template(GTK_WIDGET(self));
    // Initalize window
    self->title_bar = my_titlebar_new();
    gtk_window_set_title(GTK_WINDOW(self), "Calculator");
    gtk_window_set_icon_name(GTK_WINDOW(self), "calcapp");
    my_titlebar_set_window(self->title_bar, GTK_WIDGET(self));
    
    // Link Signals
    g_signal_connect(self->btnanswer, "clicked", G_CALLBACK(btnanswer_clicked), self);
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
    g_signal_connect(self->btnpoint, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btndiv100, "clicked", G_CALLBACK(btns_clicked), self);
    g_signal_connect(self->btnpow, "clicked", G_CALLBACK(btnpow_clicked), self);
    g_signal_connect(self->btnsqrt, "clicked", G_CALLBACK(btnsqrt_clicked), self);
    g_signal_connect(self->btnanswer, "clicked", G_CALLBACK(btnanswer_clicked), self);
    g_signal_connect(self->btnclear, "clicked", G_CALLBACK(btnclear_clicked), self);
    g_signal_connect(self->btnback, "clicked", G_CALLBACK(btnback_clicked), self);

    // Initalize entry
    gtk_editable_set_text(GTK_EDITABLE(self->entry_ans), "");
}

static void calc_app_class_init(CalcAppClass *klass)
{
    G_OBJECT_CLASS(klass)->dispose = calc_app_dispose;
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass),
                                                "/org/gtk/daleclack/calcapp.ui");
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, entry_ans);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btnanswer);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btnback);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btnclear);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btn0);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btn1);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btn2);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btn3);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btn4);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btn5);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btn6);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btn7);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btn8);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btn9);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btnadd);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btnsubb);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btnmul);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btndiv);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btnleft);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btnright);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btnpoint);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btndiv100);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btnsqrt);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), CalcApp, btnpow);
}

CalcApp *calc_app_new()
{
    return CALC_APP(g_object_new(calc_app_get_type(), NULL));
}
