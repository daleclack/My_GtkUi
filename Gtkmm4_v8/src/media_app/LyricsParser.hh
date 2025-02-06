#pragma once

#include <gtkmm.h>
#include <map>
#include <vector>

// Use map to store lyrics timestamp and lyric lines
// typedef std::map<gint64, std::string> lyrics_map;

class LyricsParser
{
public:
    LyricsParser();
    bool update_lyrics(const Glib::ustring &filename);
    Glib::ustring get_lyric_line(gint64 timestamp);

private:
    // lyric file
    Glib::RefPtr<Gio::File> lrc_file;
    char *lyric_file_contents;
    gsize length;
    Glib::ustring lrc_content_buffer, lrc_line_buffer;

    // lyric data structure
    Glib::ustring current_lyric_line;
    Glib::ustring lrc_filename;
    std::vector<gint64> lyric_timestamps;
    std::vector<Glib::ustring> lyric_lines;

    // current lyric index
    // gint64 current_lyric_index;
    gint64 n_lyric_lines;

    // Useful functions
    void lyric_line_process();
    void lyric_line_get_time(std::string &lyric_line_str);
    gint64 get_lyric_timestamp(const std::string &line);
    void UTF8_String_Fix(std::string &str);
};