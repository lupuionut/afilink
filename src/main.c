#include <gtk/gtk.h>


GtkTextView *g_display_result;
GtkEntry *g_input_name;
GtkTextBuffer *display_buffer;

static void destroy(GtkWidget *widget, gpointer data);

int main(int argc, char *argv[])
{
    GtkBuilder      *builder;
    GtkWidget       *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "afilink"));
    gtk_builder_connect_signals(builder, NULL);

    g_input_name =
        GTK_ENTRY(gtk_builder_get_object(builder, "input_name"));
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
    const char *text = gtk_entry_get_text(GTK_ENTRY(g_input_name));
    GtkTextMark *mark;
    GtkTextIter iter;

    display_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(g_display_result));

    mark = gtk_text_buffer_get_insert (display_buffer);
    gtk_text_buffer_get_iter_at_mark (display_buffer, &iter, mark);

    if (gtk_text_buffer_get_char_count(display_buffer))
        gtk_text_buffer_insert (display_buffer, &iter, "\n", 1);

    gtk_text_buffer_insert (display_buffer, &iter, text, -1);
    gtk_entry_set_text(GTK_ENTRY(g_input_name), "");
}

void on_btn_clear_clicked()
{
    display_buffer = NULL;
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(g_display_result), display_buffer);
}

static void destroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}
