#pragma once

#include <glib.h>

static const char * const supported_globs[]={
    "*.bmp",
    "*.cod",
    "*.gif",
    "*.ief",
    "*.jpe",
    "*.jpeg",
    "*.jpg",
    "*.jfif",
    "*.svg",
    "*.png",
    "*.tif",
    "*.tiff",
    "*.ras",
    "*.cmx",
    "*.ico",
    "*.pnm",
    "*.pbm",
    "*.pgm",
    "*.ppm",
    "*.rgb",
    "*.xbm",
    "*.xpm",
    "*.xwd",
    NULL
};

static inline gboolean mime_type_supported(){
#ifdef G_OS_WIN32
    return FALSE;
#else 
    return TRUE;
#endif
}
