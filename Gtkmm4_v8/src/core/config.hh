#pragma once

#include <gtkmm.h>
typedef void (*pfun)(guint id);

static inline bool is_windows()
{
#ifdef _WIN32
    return true;
#else
    return false;
#endif
}