#pragma once

#ifdef _WIN32

// Definition fix for microsoft windows
// On linux, the limits.h is automatically included
#define NAME_MAX 256

#endif