#include "LyricsParser.hh"
#include <fstream>
#include <iostream>

LyricsParser::LyricsParser()
{
}

void LyricsParser::UTF8_String_Fix(std::string &str)
{
    // Replace & with &amp; in the string
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == '&')
        {
            str.insert(i + 1, "amp;");
        }
    }
}

bool LyricsParser::update_lyrics(const Glib::ustring &filename)
{
    char line_buffer[1024];
    
    // Clear previous lyrics
    lyric_timestamps.clear();
    lyric_lines.clear();

    // Process file name for lyrics file
    lrc_filename = filename.substr(0, filename.find_last_of('.')) + ".lrc";

    // Open lyrics file
    lrc_file = Gio::File::create_for_path(lrc_filename);
    lrc_file->load_contents(lyric_file_contents, length);

    // Process lyrics file contents
    n_lyric_lines = 0;
    lyric_line_process();
    g_free(lyric_file_contents);
    lrc_file.reset();

    return true;
}

void LyricsParser::lyric_line_process()
{
    lrc_content_buffer = Glib::ustring(lyric_file_contents);

    // Use string stream to split the lyrics file into lines
    std::stringstream ss(lrc_content_buffer);
    std::string item;
    while (std::getline(ss, item))
    {
        lyric_line_get_time(item);
    }
}

void LyricsParser::lyric_line_get_time(std::string &lyric_line_str)
{
    // Split the time and lyrics into two parts
    size_t pos1 = lyric_line_str.find('[');
    size_t pos2 = lyric_line_str.find(']');
    if (pos1 != std::string::npos && pos2 != std::string::npos)
    {
        // Store the timestamps into a vector
        std::string time_str = lyric_line_str.substr(pos1 + 1, pos2 - 1);
        gint64 timestamp = get_lyric_timestamp(time_str);
        if (timestamp != -1)
        {
            lyric_timestamps.push_back(timestamp);
        }

        // Try to check whether all timestamps are converted
        std::string lyrics_line_buffer = lyric_line_str.substr(pos2 + 1);
        lyric_line_get_time(lyrics_line_buffer);

        // Store the lyrics line into a vector
        size_t pos3 = lyric_line_str.rfind(']');
        std::string lyrics_buffer = lyric_line_str.substr(pos3 + 1);
        UTF8_String_Fix(lyrics_buffer);
        lyric_lines.push_back(lyrics_buffer.c_str());
        n_lyric_lines++;
        // std::cout << timestamp << " " << lyrics_buffer << std::endl;
    }else{
        return;
    }
}

gint64 LyricsParser::get_lyric_timestamp(const std::string &line)
{
    gint64 lyric_timestamp = 0;
    gint64 lyric_minutes, lyric_seconds, lyric_milliseconds;

    // Add "-1" timestamp for other information
    if (line[0] > '9' && line[0] < '0')
    {
        return -1;
    }

    // Minutes
    lyric_minutes = (line[0] - '0') * 10 + line[1] - '0';

    // Seconds
    lyric_seconds = (line[3] - '0') * 10 + line[4] - '0';

    // Milliseconds
    lyric_milliseconds = (line[6] - '0') * 100 + (line[7] - '0') * 10;

    lyric_timestamp = lyric_minutes * 60 * 1000 + lyric_seconds * 1000 + lyric_milliseconds;

    return lyric_timestamp;
}

Glib::ustring LyricsParser::get_lyric_line(gint64 timestamp)
{
    Glib::ustring lyric_buffer;
    // Get the lyrics line that matches the timestamp
    // std::cout << "Timestamp: " << timestamp << std::endl;
    for (size_t i = 0; i < lyric_timestamps.size(); i++)
    {
        /* code */
        if (timestamp >= lyric_timestamps[i] - 100 && timestamp < lyric_timestamps[i] + 200)
        {
            lyric_buffer = lyric_lines[i];
            break;
        }
    }
    return lyric_buffer;
}
