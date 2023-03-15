#include "AboutWin.h"

struct _AboutWin{
    GtkWindow parent_instance;
};

G_DEFINE_TYPE(AboutWin, about_win, GTK_TYPE_WINDOW)

static void about_win_init(AboutWin *self){}

static void about_win_class_init(AboutWinClass *klass){}

AboutWin *about_win_new(){
    return ABOUT_WIN(g_object_new(about_win_get_type(), NULL));
}
