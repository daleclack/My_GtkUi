#include "MainWin.h"
#include "MainStack.h"
#include "winconf.h"
#include "winpe.xpm"
#include "img7.xpm"
#include "image_types.h"

enum class BackMode{
    DEFAULT_1,
    DEFAULT_2,
    CUSTOM
};

struct _MainWin{
    GtkApplicationWindow parent;
    GtkWidget * background;
    GtkWidget * overlay;
    GtkGesture * gesture;
    GtkWidget * popover;
    GtkWidget * stack;
    int width,height;
    BackMode back_mode;
};

G_DEFINE_TYPE(MainWin,main_win,GTK_TYPE_APPLICATION_WINDOW)

static void change_background(GtkNativeDialog * dialog,int response,MainWin *win){
    if(response == GTK_RESPONSE_ACCEPT){
        GFile * file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(dialog));
        char * filename = g_file_get_path(file);

        //Set Image from the file
        GdkPixbuf * pixbuf, * sized;
        pixbuf = gdk_pixbuf_new_from_file(filename,NULL);
        sized = gdk_pixbuf_scale_simple(pixbuf,win->width,win->height,GDK_INTERP_BILINEAR);
        gtk_picture_set_pixbuf(GTK_PICTURE(win->background),sized);

        //Change Mode and free memory
        win->back_mode = BackMode::CUSTOM;
        g_object_unref(pixbuf);
        g_object_unref(sized);
        g_object_unref(file);
        g_free(filename);
    }
    gtk_native_dialog_destroy(dialog);
}

static void background_dialog(GSimpleAction * action, GVariant * parmeter, gpointer data){
    GtkFileChooserNative * dialog = gtk_file_chooser_native_new("Open Image File",GTK_WINDOW(data),
                                    GTK_FILE_CHOOSER_ACTION_OPEN,"OK","Cancel");

    GtkFileFilter * filter_image = gtk_file_filter_new();
    gtk_file_filter_set_name(filter_image,"Image Files");
    
    //For Systems supported mime type and not supported, use different way
    if(mime_type_supported()){
        gtk_file_filter_add_mime_type(filter_image,"image/*");
    }else{
        for(int i=0;supported_globs !=NULL && supported_globs[i]!=NULL;i++){
            const char * glob = supported_globs[i];
            gtk_file_filter_add_pattern(filter_image,glob);
        }
    }
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),filter_image);

    GtkFileFilter * filter_any = gtk_file_filter_new();
    gtk_file_filter_set_name(filter_any,"Any Files");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),filter_any);
    
    g_signal_connect(dialog,"response",G_CALLBACK(change_background),data);
    gtk_native_dialog_show(GTK_NATIVE_DIALOG(dialog));
}

static void logout_activated(GSimpleAction * action, GVariant * parmeter, gpointer data){
    MainWin * win = MAIN_WIN(data);
    gtk_stack_set_visible_child_name(GTK_STACK(win->stack),"login_page");
}

static void quit_activated(GSimpleAction * action, GVariant * parmeter, gpointer data){
    gtk_window_destroy(GTK_WINDOW(data));
}
static void winsize_activated(GSimpleAction * action, GVariant * parmeter, gpointer data){
    MainWin * win = MAIN_WIN(data);
    ConfDlg * dialog = conf_dlg_new(GTK_WINDOW(win));
    gtk_window_present(GTK_WINDOW(dialog));
}

static void default_background1(GSimpleAction * action, GVariant * parmeter, gpointer data){
    //Set the background by pixbuf
    MainWin * win = MAIN_WIN(data);
    GdkPixbuf * pixbuf, * sized;
    pixbuf = gdk_pixbuf_new_from_xpm_data(winpe);
    sized = gdk_pixbuf_scale_simple(pixbuf,win->width,win->height,GDK_INTERP_BILINEAR);
    gtk_picture_set_pixbuf(GTK_PICTURE(win->background),sized);

    //Change Mode and free memory
    win->back_mode = BackMode::DEFAULT_1;
    g_object_unref(pixbuf);
    g_object_unref(sized);
}

static void default_background2(GSimpleAction * action, GVariant * parmeter, gpointer data){
    //Set the background by pixbuf
    MainWin * win = MAIN_WIN(data);
    GdkPixbuf * pixbuf, * sized;
    pixbuf = gdk_pixbuf_new_from_xpm_data(img7);
    sized = gdk_pixbuf_scale_simple(pixbuf,win->width,win->height,GDK_INTERP_BILINEAR);
    gtk_picture_set_pixbuf(GTK_PICTURE(win->background),sized);

    //Change Mode and free memory
    win->back_mode = BackMode::DEFAULT_2;
    g_object_unref(pixbuf);
    g_object_unref(sized);
}

static void refresh_activated(GSimpleAction * action, GVariant * parmeter, gpointer data){
    MainWin * win = MAIN_WIN(data);
    //Redraw background image
    switch(win->back_mode){
        case BackMode::DEFAULT_1:
            default_background1(action,parmeter,data);
            break;
        case BackMode::DEFAULT_2:
            default_background2(action,parmeter,data);
            break;
        case BackMode::CUSTOM:
            break;
    }
}

static void about_activated(GSimpleAction * action, GVariant * parmeter, gpointer data){
    GtkWindow * win =GTK_WINDOW(data);
    const char *authors[]={
        "Dale Clack",
        "GCR_CMake on github https://github.com/Makman2/GCR_CMake",
        NULL
    };
    char *version;
    version=g_strdup_printf("6.3\nRunning Against GTK %d.%d.%d",
                            gtk_get_major_version(),
                            gtk_get_minor_version(),
                            gtk_get_micro_version());
    GtkWidget *dialog;
    dialog=gtk_about_dialog_new();
    gtk_show_about_dialog(win,
    "program-name","My GtkUI Classic",
    "version",version,
    "copyright", "© 2019—2021 The Xe Project",
    "comments", "A program that simulates desktop (Gtk4 Version)",
    "authors",authors,
    "license-type",GTK_LICENSE_GPL_3_0,
    "logo-icon-name","My_GtkUI",
    "title","About My GtkUI (Gtk4 Version)",
    NULL);
    g_free(version);
}

void btnabout_clicked(GtkWidget * widget,gpointer data){
    about_activated(NULL,NULL,data);
}

static void gesture_pressed(GtkGestureClick *self,int n_press,double x,double y,MainWin * win){
    //When third button is pressed, show popover menu
    GdkRectangle rect;
    rect.x=x;
    rect.y=y;
    rect.height=1;
    rect.width=1;
    gtk_popover_set_pointing_to(GTK_POPOVER(win->popover),&rect);
    gtk_popover_popup(GTK_POPOVER(win->popover));
}

GtkWidget * main_win_get_background(MainWin * win){
    //Get Background widget, for window size config.
    return win->background;
}

static void main_win_init(MainWin * win){
    //For Custom Window Size
    win->width=800;
    win->height=450;
    get_config(&(win->width),&(win->height));
    //g_print("%d %d",win->width,win->height);

    //Initailze Window
    gtk_window_set_icon_name(GTK_WINDOW(win),"My_GtkUI");
    gtk_window_set_title(GTK_WINDOW(win),"My GtkUI (Gtk4 Version)");

    //Add Actions
    static GActionEntry entries[] = {
        {"back",background_dialog,NULL,NULL,NULL},
        {"default1",default_background1,NULL,NULL,NULL},
        {"default2",default_background2,NULL,NULL,NULL},
        {"refresh",refresh_activated,NULL,NULL,NULL},
        {"about",about_activated,NULL,NULL,NULL},
        {"logout",logout_activated,NULL,NULL,NULL},
        {"quit",quit_activated,NULL,NULL,NULL},
        {"size",winsize_activated,NULL,NULL,NULL}
    };
    g_action_map_add_action_entries(G_ACTION_MAP(win),entries,G_N_ELEMENTS(entries),win);

    //Add Overlay and background widget
    win->overlay = gtk_overlay_new();
    win->background = gtk_picture_new();
    gtk_widget_set_size_request(win->background,win->width,win->height);
    gtk_overlay_set_child(GTK_OVERLAY(win->overlay),win->background);
    default_background1(NULL,NULL,win);

    //Add Menu
    GtkBuilder * menu_builder = gtk_builder_new_from_resource("/org/gtk/daleclack/appmenu.xml");
    GMenuModel * model = G_MENU_MODEL(gtk_builder_get_object(menu_builder,"app-menu"));
    win->popover = gtk_popover_menu_new_from_model(model);
    gtk_popover_set_has_arrow(GTK_POPOVER(win->popover),FALSE);
    gtk_widget_set_parent(win->popover,win->overlay);

    //Add Gesture to show menu
    win->gesture = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(win->gesture),GDK_BUTTON_SECONDARY);
    g_signal_connect(win->gesture,"pressed",G_CALLBACK(gesture_pressed),win);
    gtk_widget_add_controller(win->overlay,GTK_EVENT_CONTROLLER(win->gesture));

    //Add Main Page
    win->stack = create_main_stack(win,model);
    gtk_widget_set_halign(GTK_WIDGET(win->stack),GTK_ALIGN_FILL);
    gtk_widget_set_valign(GTK_WIDGET(win->stack),GTK_ALIGN_FILL);
    gtk_overlay_add_overlay(GTK_OVERLAY(win->overlay),GTK_WIDGET(win->stack));

    gtk_window_set_child(GTK_WINDOW(win),win->overlay);
    g_object_unref(menu_builder);
}

static void main_win_class_init(MainWinClass * klass){}

MainWin * main_win_new(GtkApplication * app){
    return (MainWin*)g_object_new(main_win_get_type(),"application",app,NULL);
}
