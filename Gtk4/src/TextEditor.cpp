#include "TextEditor.h"

static GtkWidget *infobar;
static GtkTextBuffer *textbuffer;
static int filemode;

static void dialog_response(GtkWidget *widget,int response,gpointer data){
    char *contents,*filename;
    gsize length;
    if(response==GTK_RESPONSE_OK){
        GFile *file=gtk_file_chooser_get_file((GtkFileChooser*)widget);
        filename=g_file_get_path(file);
        if(filemode==0){
            //Get File contents and show
            if(g_file_get_contents(filename,&contents,&length,NULL)){
                gtk_text_buffer_set_text(textbuffer,contents,length);
            }
        }else{
            //Get Start and End char of Text
            GtkTextIter start,end;
            gtk_text_buffer_get_start_iter(textbuffer,&start);
            gtk_text_buffer_get_end_iter(textbuffer,&end);
            //Get Contents
            contents=gtk_text_buffer_get_text(textbuffer,&start,&end,TRUE);
            //Create a file and write
            FILE *fp;
            fp=fopen(filename,"wt+");
            if(fp!=NULL){
                fputs(contents,fp);
            }
            fclose(fp);
        }
        g_object_unref(file);
        g_free(contents);
        g_free(filename);
    }
    gtk_widget_destroy(widget);
}

static void openfile_dialog(GtkWidget *widget,GtkWindow *parent){
    filemode=0;
    GtkFileChooserAction action=GTK_FILE_CHOOSER_ACTION_OPEN;
    GtkWidget *dialog=gtk_file_chooser_dialog_new("Open a text file",parent,
                      action,"OK",GTK_RESPONSE_OK,"Cancel",GTK_RESPONSE_CANCEL,NULL);

    GtkFileFilter *filter=gtk_file_filter_new();
    gtk_file_filter_set_name(filter,"Text Files");
    gtk_file_filter_add_mime_type(filter,"text/*");
    gtk_file_chooser_add_filter((GtkFileChooser*)dialog,filter);
    //g_object_unref(filter);

    filter=gtk_file_filter_new();
    gtk_file_filter_set_name(filter,"Any Files");
    gtk_file_filter_add_pattern(filter,"*");
    gtk_file_chooser_add_filter((GtkFileChooser*)dialog,filter);
    //g_object_unref(filter);

    g_signal_connect(dialog,"response",G_CALLBACK(dialog_response),NULL);
    gtk_widget_show_all(dialog);
}

static void savefile_dialog(GtkWidget *widget,GtkWindow *parent){
    filemode=1;
    GtkFileChooserAction action=GTK_FILE_CHOOSER_ACTION_SAVE;
    GtkWidget *dialog=gtk_file_chooser_dialog_new("Open a text file",parent,
                      action,"OK",GTK_RESPONSE_OK,"Cancel",GTK_RESPONSE_CANCEL,NULL);

    GtkFileFilter *filter=gtk_file_filter_new();
    gtk_file_filter_set_name(filter,"Text Files");
    gtk_file_filter_add_mime_type(filter,"text/*");
    gtk_file_chooser_add_filter((GtkFileChooser*)dialog,filter);
    //g_object_unref(filter);

    filter=gtk_file_filter_new();
    gtk_file_filter_set_name(filter,"Any Files");
    gtk_file_filter_add_pattern(filter,"*");
    gtk_file_chooser_add_filter((GtkFileChooser*)dialog,filter);
    //g_object_unref(filter);

    g_signal_connect(dialog,"response",G_CALLBACK(dialog_response),NULL);
    gtk_widget_show_all(dialog);
}

static void textbuffer_clear(GtkWidget *widget,GtkTextBuffer *buffer){
    gtk_text_buffer_set_text(buffer,"",-1);
    gtk_widget_show(infobar);
}

static void infobar_response(GtkWidget *widget,gpointer data){
    gtk_widget_hide(widget);
}

static void btncopy_clicked(GtkWidget *widget,gpointer data){
    //Get Text or selected text
    const char *contents;
    GtkTextIter start,end;
    if(gtk_text_buffer_get_has_selection(textbuffer)){
        gtk_text_buffer_get_bounds(textbuffer,&start,&end);
        contents=gtk_text_buffer_get_text(textbuffer,&start,&end,TRUE);
    }else{
        gtk_text_buffer_get_start_iter(textbuffer,&start);
        gtk_text_buffer_get_end_iter(textbuffer,&end);
        //Get Contents
        contents=gtk_text_buffer_get_text(textbuffer,&start,&end,TRUE);    
    }

    //Get Clipboard and put text
    GtkClipboard *clipboard=gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text(clipboard,contents,-1);

}

static void get_text(GtkClipboard * clipboard,const char * text,gpointer data){
    //Paste text
    gtk_text_buffer_insert_at_cursor(textbuffer,text,-1);
}

static void btnpaste_clicked(GtkWidget *widget,gpointer data){
    //Get ClipBoard and paste text
    GtkClipboard * clipboard=gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_request_text(clipboard,get_text,NULL);
}

void text_editor(GtkWidget *widget,GtkWindow *parent){
    //Initalize window
    GtkWidget *window,*hbox;
    //window=gtk_application_window_new(app);
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_transient_for((GtkWindow*)window,parent);
    gtk_window_set_title((GtkWindow*)window,"Simple Text Editor");
    gtk_window_set_icon_name((GtkWindow*)window,"org.gtk.daleclack");
    gtk_window_set_default_size((GtkWindow*)window,800,450);
    hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,5);

    //Initalize infobar
    infobar=gtk_info_bar_new_with_buttons("OK",GTK_RESPONSE_OK,NULL);
    GtkWidget *content_area=gtk_info_bar_get_content_area((GtkInfoBar*)infobar);
    GtkWidget *label=gtk_label_new("Cleared the text");
    g_signal_connect(infobar,"response",G_CALLBACK(infobar_response),NULL);
    gtk_container_add((GtkContainer*)content_area,label);
    gtk_box_pack_start((GtkBox*)hbox,infobar,TRUE,FALSE,0);

    //Ininalize TextView
    GtkWidget *scrolled;
    GtkWidget *textview;
    //GtkTextBuffer *textbuffer;
    scrolled=gtk_scrolled_window_new(NULL,NULL);
    gtk_scrolled_window_set_policy((GtkScrolledWindow*)scrolled,
                                  GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    textview=gtk_text_view_new();
    textbuffer=gtk_text_view_get_buffer((GtkTextView*)textview);
    //gtk_widget_set_size_request(scrolled,800,440);
    gtk_container_add((GtkContainer*)scrolled,textview);
    gtk_box_pack_start((GtkBox*)hbox,scrolled,TRUE,TRUE,0);

    //Add Buttons
    GtkWidget *btn_box=gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
    GtkWidget *btnopen,*btnsave,*btnclear,*btncopy,*btnpaste,*btnexit;
    btnopen=gtk_button_new_with_label("Open");
    g_signal_connect(btnopen,"clicked",G_CALLBACK(openfile_dialog),window);
    btnsave=gtk_button_new_with_label("Save");
    g_signal_connect(btnsave,"clicked",G_CALLBACK(savefile_dialog),window);
    btnclear=gtk_button_new_with_label("Clear");
    g_signal_connect(btnclear,"clicked",G_CALLBACK(textbuffer_clear),textbuffer);
    btncopy=gtk_button_new_with_label("Copy");
    g_signal_connect(btncopy,"clicked",G_CALLBACK(btncopy_clicked),NULL);
    btnpaste=gtk_button_new_with_label("Paste");
    g_signal_connect(btnpaste,"clicked",G_CALLBACK(btnpaste_clicked),NULL);
    btnexit=gtk_button_new_with_label("Exit");
    g_signal_connect_swapped(btnexit,"clicked",G_CALLBACK(gtk_widget_destroy),window);
    gtk_widget_set_valign(btn_box,GTK_ALIGN_CENTER);
    gtk_box_pack_end((GtkBox*)btn_box,btnexit,FALSE,FALSE,0);
    gtk_box_pack_end((GtkBox*)btn_box,btnpaste,FALSE,FALSE,0);
    gtk_box_pack_end((GtkBox*)btn_box,btncopy,FALSE,FALSE,0);
    gtk_box_pack_end((GtkBox*)btn_box,btnclear,FALSE,FALSE,0);
    gtk_box_pack_end((GtkBox*)btn_box,btnsave,FALSE,FALSE,0);
    gtk_box_pack_end((GtkBox*)btn_box,btnopen,FALSE,FALSE,0);
    gtk_box_pack_end((GtkBox*)hbox,btn_box,FALSE,FALSE,0);

    //Add widgets and show everything
    gtk_container_add((GtkContainer*)window,hbox);
    gtk_widget_show_all(window);
    gtk_widget_hide(infobar);
}
