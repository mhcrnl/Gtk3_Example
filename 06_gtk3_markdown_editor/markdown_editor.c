#include <gtk/gtk.h>

static GtkWidget *text_view;

static void on_open_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileFilter *filter;
    GtkTextBuffer *buffer;
    gchar *filename;
    gchar *content;
    gsize length;
    GError *error = NULL;

    dialog = gtk_file_chooser_dialog_new("Deschide fișier",
                                         GTK_WINDOW(user_data),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Anulează", GTK_RESPONSE_CANCEL,
                                         "_Deschide", GTK_RESPONSE_ACCEPT,
                                         NULL);

    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Fișiere Markdown");
    gtk_file_filter_add_pattern(filter, "*.md");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        if (g_file_get_contents(filename, &content, &length, &error)) {
            buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
            gtk_text_buffer_set_text(buffer, content, length);
            g_free(content);
        } else {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(user_data),
                                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                                             GTK_MESSAGE_ERROR,
                                                             GTK_BUTTONS_CLOSE,
                                                             "Eroare la deschiderea fișierului: %s", error->message);
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
            g_error_free(error);
        }

        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

static void on_save_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    gchar *text;
    gchar *filename;
    GError *error = NULL;

    dialog = gtk_file_chooser_dialog_new("Salvează fișierul",
                                         GTK_WINDOW(user_data),
                                         GTK_FILE_CHOOSER_ACTION_SAVE,
                                         "_Anulează", GTK_RESPONSE_CANCEL,
                                         "_Salvează", GTK_RESPONSE_ACCEPT,
                                         NULL);

    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "document.md");

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        gtk_text_buffer_get_bounds(buffer, &start, &end);
        text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        if (!g_file_set_contents(filename, text, -1, &error)) {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(user_data),
                                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                                             GTK_MESSAGE_ERROR,
                                                             GTK_BUTTONS_CLOSE,
                                                             "Eroare la salvarea fișierului: %s", error->message);
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
            g_error_free(error);
        }

        g_free(text);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *scrolled_window;
    GtkWidget *open_button;
    GtkWidget *save_button;
    GtkWidget *button_box;
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

    button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(box), button_box, FALSE, FALSE, 0);

    open_button = gtk_button_new_with_label("Deschide");
    g_signal_connect(open_button, "clicked", G_CALLBACK(on_open_button_clicked), window);
    gtk_box_pack_start(GTK_BOX(button_box), open_button, FALSE, FALSE, 0);

    save_button = gtk_button_new_with_label("Salvează");
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_button_clicked), window);
    gtk_box_pack_start(GTK_BOX(button_box), save_button, FALSE, FALSE, 0);

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

