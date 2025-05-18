#include <gtk/gtk.h>

// Funcție pentru salvarea conținutului într-un fișier
void save_file(GtkWidget *widget, gpointer text_view) {
    GtkWidget *dialog;
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    gchar *text;
    
    dialog = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                         GTK_FILE_CHOOSER_ACTION_SAVE,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Save", GTK_RESPONSE_ACCEPT,
                                         NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        gtk_text_buffer_get_bounds(buffer, &start, &end);
        text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        FILE *file = fopen(filename, "w");
        if (file) {
            fputs(text, file);
            fclose(file);
        }
        g_free(filename);
        g_free(text);
    }
    
    gtk_widget_destroy(dialog);
}

// Funcție pentru încărcarea unui fișier
void open_file(GtkWidget *widget, gpointer text_view) {
    GtkWidget *dialog;
    GtkTextBuffer *buffer;
    gchar *content;
    gsize length;
    
    dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Open", GTK_RESPONSE_ACCEPT,
                                         NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        
        if (g_file_get_contents(filename, &content, &length, NULL)) {
            buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
            gtk_text_buffer_set_text(buffer, content, length);
            g_free(content);
        }
        g_free(filename);
    }
    
    gtk_widget_destroy(dialog);
}

// Funcția principală
int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *text_view;
    GtkWidget *button_open, *button_save;
    
    gtk_init(&argc, &argv);
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Markdown Editor");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    text_view = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 0);
    
    button_open = gtk_button_new_with_label("Open File");
    g_signal_connect(button_open, "clicked", G_CALLBACK(open_file), text_view);
    gtk_box_pack_start(GTK_BOX(vbox), button_open, FALSE, FALSE, 0);
    
    button_save = gtk_button_new_with_label("Save File");
    g_signal_connect(button_save, "clicked", G_CALLBACK(save_file), text_view);
    gtk_box_pack_start(GTK_BOX(vbox), button_save, FALSE, FALSE, 0);
    
    gtk_widget_show_all(window);
    gtk_main();
    
    return 0;
}

