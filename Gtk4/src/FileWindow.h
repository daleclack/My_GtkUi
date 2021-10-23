#ifndef __FILE_WINDOW_H_
#define __FILE_WINDOW_H_

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(FileWindow,file_window,FILE,WINDOW,GtkWindow)

FileWindow * file_window_new();

#endif
