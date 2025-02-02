#include "MediaItem.h"
#include <cstring>

struct _MediaItem
{
    GObject parent_instance;
    char disp_name[name_max_length];
    char file_name[path_max_length];
};

G_DEFINE_TYPE(MediaItem, media_item, G_TYPE_OBJECT)

const char *media_item_get_filename(MediaItem *item)
{
    // Get true file name
    return item->file_name;
}

const char *media_item_get_dispname(MediaItem *item)
{
    // Get Base name
    return item->disp_name;
}

static void media_item_init(MediaItem *self)
{
}

static void media_item_class_init(MediaItemClass *klass)
{
}

MediaItem *media_item_new(const char *dispname, const char *filename)
{
    // Create a new item
    MediaItem *item = Media_Item(g_object_new(media_item_get_type(), NULL));
    strncpy(item->disp_name, dispname, name_max_length);
    strncpy(item->file_name, filename, path_max_length);
    return item;
}
