#include "MsgDialog.hh"

MsgDialog::MsgDialog()
    : btn_ok("Ok"),
    dialog_box(Gtk::Orientation::HORIZONTAL, 10)

{
    set_title("Message");

    // Add header bar
    set_titlebar(header_bar);
    header_bar.pack_end(btn_ok);
    header_bar.set_show_title_buttons(false);

    // Add message label
    image_icon.set_from_resource("/org/gtk/daleclack/dialog-error.png");
    image_icon.set_pixel_size(48);
    dialog_box.append(image_icon);
    dialog_box.append(label_msg);
    set_child(dialog_box);
    dialog_box.set_margin(10);

    btn_ok.signal_clicked().connect([this] { close(); });
}

void MsgDialog::show_message(const Glib::ustring& message)
{
    label_msg.set_text(message);
    present();
}