#pragma once

#include <gtk/gtk.h>

// File name and path limits
#define name_max_length 256
#define path_max_length 4096

#define MEDIA_ITEM_TYPE (media_item_get_type())
G_DECLARE_FINAL_TYPE(MediaItem, media_item, Media, Item, GObject)

const char *media_item_get_filename(MediaItem *item);

const char *media_item_get_dispname(MediaItem *item);

MediaItem *media_item_new(const char *dispname, const char *filename);
