#include <gtk/gtk.h>

static GtkWidget *text_view;

static void on_save_button_clicked(GtkButton *button, gpointer user_data) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    GtkWidget *dialog = gtk_file_chooser_dialog_new("Salvează fișierul",
                                                    GTK_WINDOW(user_data),
                                                    GTK_FILE_CHOOSER_ACTION_SAVE,
                                                    "_Anulează", GTK_RESPONSE_CANCEL,
                                                    "_Salvează", GTK_RESPONSE_ACCEPT,
                                                    NULL);

    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "document.md");

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        GError *error = NULL;
        if (!g_file_set_contents(filename, text, -1, &error)) {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(user_data),
                                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                                             GTK_MESSAGE_ERROR,
                                                             GTK_BUTTONS_CLOSE,
                                                             "Eroare la salvare: %s", error->message);
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
            g_error_free(error);
        }
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
    g_free(text);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *scrolled_window;
    GtkWidget *save_button;
    GtkWidget *box;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Editor Markdown");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(box), scrolled_window, TRUE, TRUE, 0);

    text_view = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

    save_button = gtk_button_new_with_label("Salvează");
    gtk_box_pack_start(GTK_BOX(box), save_button, FALSE, FALSE, 0);

    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_button_clicked), window);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.example.markdowneditor", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

