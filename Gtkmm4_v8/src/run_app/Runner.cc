#include "Runner.hh"
#include <iostream>
#include <thread>

Runner::Runner()
    : btn_ok("OK"),
      btn_cancel("Cancel"),
      btn_path("..."),
      main_box(Gtk::Orientation::HORIZONTAL, 10)
{
    // Initalize window
    set_title("Run a command");
    set_titlebar(header);
    header.set_show_title_buttons(false);
    header.pack_start(btn_cancel);
    header.pack_end(btn_ok);

    // Add entry and button for path
    entry_path.set_hexpand(true);
    main_box.append(entry_path);
    main_box.append(btn_path);
    set_child(main_box);

    // Connect signals
    btn_ok.signal_clicked().connect(sigc::mem_fun(*this, &Runner::btnok_clicked));
    btn_cancel.signal_clicked().connect([this]
                                        { close(); });
    btn_path.signal_clicked().connect(sigc::mem_fun(*this, &Runner::btnpath_clicked));
    entry_path.signal_activate().connect(sigc::mem_fun(*this, &Runner::btnok_clicked));
}

void Runner::btnok_clicked()
{
    // Run command
    auto cmd = entry_path.get_text();
    std::thread thread1(system, cmd.c_str());
    thread1.detach();

    // Close window
    close();
}

void Runner::btnpath_clicked()
{
    // Create a dialog
    auto dialog = Gtk::FileDialog::create();
    dialog->set_title("Open a executable file");

    // Open dialog
    dialog->open(sigc::bind(sigc::mem_fun(*this, &Runner::opendialog_finish), dialog));
}

void Runner::opendialog_finish(Glib::RefPtr<Gio::AsyncResult> &result,
                               Glib::RefPtr<Gtk::FileDialog> &dialog)
{
    try
    {
        // Update entry with selected file path
        auto file = dialog->open_finish(result);
        entry_path.set_text(file->get_path());
    }
    catch (const Gtk::DialogError &e)
    {
        std::cout << "No file selected" << e.what() << std::endl;
    }
    catch (const Glib::Error &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}