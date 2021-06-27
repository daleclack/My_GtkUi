#include "background.h"
#include "winconf.h"
#include "winpe.xpm"
#include "image_types.h"

static void dialog_response(GtkNativeDialog *widget,int response,GtkBuilder *builder){
    //Handle file chooser response and set background
    int width=640,height=360;
    get_config(&width,&height);
    GObject *background=gtk_builder_get_object(builder,"background");
    const gchar *filename;
    GFile *file;
    //g_print("%s\n",filename);
    if(response==GTK_RESPONSE_ACCEPT){
        file=gtk_file_chooser_get_file(GTK_FILE_CHOOSER(widget));
        filename=g_file_get_path(file);
        GdkPixbuf *pixbuf=gdk_pixbuf_new_from_file(filename,NULL);
        GdkPixbuf *sized=gdk_pixbuf_scale_simple(pixbuf,width,height,GDK_INTERP_BILINEAR);
        gtk_image_set_from_pixbuf(GTK_IMAGE(background),sized);
        g_object_unref(pixbuf);
        g_object_unref(sized);
    }
    //gtk_widget_destroy(widget);
    gtk_native_dialog_destroy(widget);
}

void fileopen(GtkWidget *widget,GtkBuilder *builder){
    GObject *parent=gtk_builder_get_object(builder,"window");

    //Change background
    GtkFileChooserNative *dialog;
    GtkFileChooserAction action=GTK_FILE_CHOOSER_ACTION_OPEN;
    dialog=gtk_file_chooser_native_new("Choose a Image file",GTK_WINDOW(parent),action,"OK","Cancel");
    
    //Use GtkFileFilter to select image file
    GtkFileFilter *filter=gtk_file_filter_new();
    gtk_file_filter_set_name(filter,"Image Files");

    /*
    Microsoft Windows doesn't support mine types,
    For Microsoft Windows,I use a list to store image extenstions;
    And For Linux,just use the image/* mime type
    */

#ifdef G_OS_WIN32
    int i;
    for (i = 0; supported_globs != NULL && supported_globs[i] != NULL; i++)
		{
			const char *glob = supported_globs[i];

			gtk_file_filter_add_pattern (filter, glob);
		}

#else
    gtk_file_filter_add_mime_type(filter,"image/*");
#endif

    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),filter);
    //Filter For All Files
    filter=gtk_file_filter_new();
    gtk_file_filter_set_name(filter,"All Files");
    gtk_file_filter_add_pattern(filter,"*");
    gtk_file_chooser_add_filter((GtkFileChooser*)dialog,filter);
    //Show Dialog and wait for response
    gtk_native_dialog_show(GTK_NATIVE_DIALOG(dialog));
    g_signal_connect(dialog,"response",G_CALLBACK(dialog_response),builder);
}

void default_background(GtkBuilder *builder){
    //Set the default background
    int width=640,height=360;
    get_config(&width,&height);
    GObject *background=gtk_builder_get_object(builder,"background");
    GdkPixbuf *pixbuf=gdk_pixbuf_new_from_xpm_data(winpe);
    GdkPixbuf *sized=gdk_pixbuf_scale_simple(pixbuf,width,height,GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(GTK_IMAGE(background),sized);
}
