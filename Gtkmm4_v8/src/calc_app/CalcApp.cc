#include "CalcApp.hh"
#include "calc.hh"
#include <iostream>

CalcApp::CalcApp(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_Glade)
    : Gtk::Window(cobject),
      ref_builder(ref_Glade),
      focused(true)
{
    set_icon_name("calcapp");
    set_title("Calculator");
    header.set_ctrl_window(this);

    // Get Widgets
    //ref_builder->get_widget("label_numbers", label_numbers);
    entry_ans = ref_builder->get_widget<Gtk::Entry>("entry_ans");
    btnanswer = ref_builder->get_widget<Gtk::Button>("btnanswer");
    btnback = ref_builder->get_widget<Gtk::Button>("btnback");
    btnclear = ref_builder->get_widget<Gtk::Button>("btnclear");
    btns[0] = ref_builder->get_widget<Gtk::Button>("btn0");
    btns[1] = ref_builder->get_widget<Gtk::Button>("btn1");
    btns[2] = ref_builder->get_widget<Gtk::Button>("btn2");
    btns[3] = ref_builder->get_widget<Gtk::Button>("btn3");
    btns[4] = ref_builder->get_widget<Gtk::Button>("btn4");
    btns[5] = ref_builder->get_widget<Gtk::Button>("btn5");
    btns[6] = ref_builder->get_widget<Gtk::Button>("btn6");
    btns[7] = ref_builder->get_widget<Gtk::Button>("btn7");
    btns[8] = ref_builder->get_widget<Gtk::Button>("btn8");
    btns[9] = ref_builder->get_widget<Gtk::Button>("btn9");
    btns[10] = ref_builder->get_widget<Gtk::Button>("btnadd");
    btns[11] = ref_builder->get_widget<Gtk::Button>("btnsubb");
    btns[12] = ref_builder->get_widget<Gtk::Button>("btnmul");
    btns[13] = ref_builder->get_widget<Gtk::Button>("btndiv");
    btns[14] = ref_builder->get_widget<Gtk::Button>("btnleft");
    btns[15] = ref_builder->get_widget<Gtk::Button>("btnright");
    btns[16] = ref_builder->get_widget<Gtk::Button>("btnpoint");
    btns[17] = ref_builder->get_widget<Gtk::Button>("btndiv100");
    btnsqrt = ref_builder->get_widget<Gtk::Button>("btnsqrt");
    btnpow = ref_builder->get_widget<Gtk::Button>("btnpow");

    // Link Signals
    btnclear->signal_clicked().connect(sigc::mem_fun(*this,&CalcApp::btnclear_clicked));
    btnback->signal_clicked().connect(sigc::mem_fun(*this,&CalcApp::btnback_clicked));
    btnanswer->signal_clicked().connect(sigc::mem_fun(*this,&CalcApp::btnanswer_clicked));
    btnpow->signal_clicked().connect(sigc::mem_fun(*this,&CalcApp::btnpow_clicked));
    btnsqrt->signal_clicked().connect(sigc::mem_fun(*this,&CalcApp::btnsqrt_clicked));
    for (int i = 0; i < 18; i++)
    {
        btns[i]->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalcApp::btns_clicked), btns[i]));
    }
    // entry_ans->signal_grab_focus().connect(sigc::mem_fun(*this, &CalcApp::entry_ans_focus));
}

void CalcApp::btns_clicked(Gtk::Button *button)
{
    // Get Original text and add number from button

    // 1.Get text buffer for insert
    auto buffer = entry_ans->get_buffer();

    // 2.Get text to insert
    Glib::ustring label = button->get_label();
    pos = entry_ans->get_position();

    if (focused)
    {
        pos = entry_ans->get_position();
    }

    // 3.Get Position and update text
    buffer->insert_text(pos, label);
    pos++;
    entry_ans->set_position(pos);
    focused = false;
}

void CalcApp::entry_ans_focus()
{
    focused = true;
}

void CalcApp::btnclear_clicked(){
    // Clear the text
    entry_ans->set_text("");
}

void CalcApp::btnpow_clicked(){
    Glib::ustring string = entry_ans->get_text();
    Glib::ustring string2 = string + "^2";
    entry_ans->set_text(string2);
}

void CalcApp::btnsqrt_clicked(){
    Glib::ustring string = entry_ans->get_text();
    Glib::ustring string2 = string + "^(1/2)";
    entry_ans->set_text(string2);
}

void CalcApp::btnback_clicked(){
    // Get Original text and add number from button

    // 1.Get text buffer for insert
    auto buffer = entry_ans->get_buffer();

    // 2.Get Position to delete
    if (focused)
    {
        pos = entry_ans->get_position();
    }

    // std::cout<<pos<<std::endl;
    pos--;

    // 3.Delete text in the position of cursor
    buffer->delete_text(pos,1);
    focused = false;
}

void CalcApp::btnanswer_clicked(){
    // Get expression
    Glib::ustring text = entry_ans->get_text();
    //std::cout<<text<<std::endl;

    // Calculation
    calc_reset();
    double result = calc_expression_value(text.c_str());
    char result_str[40];
    sprintf(result_str,"%f",result);
    entry_ans->set_text(Glib::ustring(result_str));
}

CalcApp *CalcApp::create()
{
    // Create a new window
    CalcApp *window = nullptr;
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/calcapp.ui");
    window = Gtk::Builder::get_widget_derived<CalcApp>(builder, "window");
    // builder->get_widget_derived("window", window);

    return window;
}
