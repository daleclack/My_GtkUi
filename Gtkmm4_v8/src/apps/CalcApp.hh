#pragma once

#include <gtkmm.h>
#include "MyHeader.hh"

class CalcApp : public Gtk::Window
{
public:
    CalcApp(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);
    static CalcApp *create();

private:
    // The builder
    Glib::RefPtr<Gtk::Builder> ref_builder;

    // Custom title bar
    MyHeader header;

    // Child widgets
    Gtk::Button *btns[18];
    Gtk::Button *btnclear, *btnback,
        *btnanswer, *btnpow, *btnsqrt;
    Gtk::Entry *entry_ans;

    bool focused;
    int pos;

    // Signal handlers
    void btns_clicked(Gtk::Button *button);
    void entry_ans_focus();
    void btnclear_clicked();
    void btnback_clicked();
    void btnanswer_clicked();
    void btnpow_clicked();
    void btnsqrt_clicked();
};