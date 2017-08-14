#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

GtkTextView *g_display_result;
GtkEntry *g_input_http;
GtkEntry *g_input_image;
GtkEntry *g_input_title;
GtkTextBuffer *display_buffer;

static void destroy(GtkWidget *widget, gpointer data);
char* concat(const char *s1, const char *s2);
char* generate_html();
int counter = 0;

int main(int argc, char *argv[])
{
    GtkBuilder      *builder;
    GtkWidget       *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "afilink"));
    gtk_builder_connect_signals(builder, NULL);

    g_input_http =
        GTK_ENTRY(gtk_builder_get_object(builder, "input_http"));
    g_input_image =
        GTK_ENTRY(gtk_builder_get_object(builder, "input_image"));
    g_input_title =
        GTK_ENTRY(gtk_builder_get_object(builder, "input_title"));
    g_display_result =
        GTK_TEXT_VIEW(gtk_builder_get_object(builder, "display_result"));


    g_signal_connect (window, "destroy",
                  G_CALLBACK (destroy), NULL);

    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

void on_btn_add_clicked()
{
    char *text = malloc(sizeof(char)*3000);
    strcpy(text,generate_html());

    GtkTextMark *mark;
    GtkTextIter iter;

    display_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(g_display_result));

    mark = gtk_text_buffer_get_insert (display_buffer);
    gtk_text_buffer_get_iter_at_mark (display_buffer, &iter, mark);

    if (gtk_text_buffer_get_char_count(display_buffer))
        gtk_text_buffer_insert (display_buffer, &iter, "\n", 1);

    gtk_text_buffer_insert (display_buffer, &iter, text, -1);
    gtk_entry_set_text(GTK_ENTRY(g_input_http), "");
    gtk_entry_set_text(GTK_ENTRY(g_input_image), "");
    gtk_entry_set_text(GTK_ENTRY(g_input_title), "");
    free(text);
    counter++;
}

void on_btn_clear_clicked()
{
    display_buffer = NULL;
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(g_display_result), display_buffer);
    counter = 0;
}

void on_btn_finish_clicked()
{   
    GtkTextBuffer *buffer;
    GtkTextIter start_sel, end_sel;
    char *text;
    char temp[100];

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(g_display_result));
    gtk_text_buffer_get_bounds(buffer,&start_sel, &end_sel);
    text = gtk_text_buffer_get_text(buffer, &start_sel, 
            &end_sel, FALSE);
    sprintf(temp,"<div class='fottogallery t%i'>",counter);
    text = concat(temp,text);
    text = concat(text,"</div>");

    buffer = gtk_text_buffer_new(NULL);
    gtk_text_buffer_set_text(buffer,text,strlen(text));
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(g_display_result),buffer);
    counter = 0;
}

static void destroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char *generate_html()
{
    const char *input_http = NULL;
    const char *input_image = NULL;
    const char *input_title = NULL;
    const char *text = NULL;

    input_http = gtk_entry_get_text(GTK_ENTRY(g_input_http));
    input_image = gtk_entry_get_text(GTK_ENTRY(g_input_image));
    input_title = gtk_entry_get_text(GTK_ENTRY(g_input_title));
    char *html = malloc(sizeof(char)*3000);
    text =
"<figure class='wp-caption alignnone'><a href='%s' target='_blank'>\
<img class='wp-image-1180' src='%s' alt=''></a>\
<label class='prod-title'>%s</label><span class='btn-cumpara'>\
<a href='%s' target='_blank'>\
<img class='cumpara1 size-full wp-image-1071'\
src='%s' alt='cumpara' width='142' height='35'></a></span></figure>"; 

    sprintf(html,text,input_http,input_image,input_title,input_http,input_image);
    return html;
}
