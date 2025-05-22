#include <gtk/gtk.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcebuffer.h>
#include <glib/gstdio.h>
#include <gio/gio.h>

// Structură pentru datele aplicației
typedef struct {
    GtkWidget *window;
    GtkWidget *text_view;
    char *current_file;
} AppData;

// Funcție pentru a deschide un fișier
static void open_file(GtkWidget *widget, AppData *app_data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Deschide fișier",
                                           GTK_WINDOW(app_data->window),
                                           action,
                                           "_Anulare", GTK_RESPONSE_CANCEL,
                                           "_Deschide", GTK_RESPONSE_ACCEPT,
                                           NULL);

    // Restricționează la fișiere text (utile pe Ubuntu)
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_mime_type(filter, "text/markdown");
    gtk_file_filter_add_mime_type(filter, "text/plain");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkTextBuffer *buffer;
        GError *error = NULL;

        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (filename != NULL) {
            if (app_data->current_file) {
                g_free(app_data->current_file);
            }
            app_data->current_file = g_strdup(filename);
            gtk_window_set_title(GTK_WINDOW(app_data->window), filename);

            buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_data->text_view));
            gtk_text_buffer_set_text(buffer, "", -1);

            gchar *file_content = NULL;
            gsize file_length = 0;
            if (!g_file_get_contents(filename, &file_content, &file_length, &error)) {
                g_printerr("Eroare la deschiderea fișierului: %s\n", error->message);
                g_error_free(error);
                file_content = NULL;
            }

            if (file_content) {
                gtk_text_buffer_set_text(buffer, file_content, file_length);
                g_free(file_content);
            }
            g_free(filename);
        }
    }
    gtk_widget_destroy(dialog);
}

// Funcție pentru a salva un fișier
static void save_file(GtkWidget *widget, AppData *app_data) {
    if (app_data->current_file) {
        GtkTextBuffer *buffer;
        GtkTextIter start, end;
        gchar *text;
        GError *error = NULL;

        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_data->text_view));
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
        text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        if (!g_file_set_contents(app_data->current_file, text, -1, &error)) {
            g_printerr("Eroare la salvarea fișierului: %s\n", error->message);
            g_error_free(error);
        }
        g_free(text);
    } else {
        save_file_as(widget, app_data);
    }
}

// Funcție pentru a salva ca un fișier nou
static void save_file_as(GtkWidget *widget, AppData *app_data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Salvează ca...",
                                           GTK_WINDOW(app_data->window),
                                           action,
                                           "_Anulare", GTK_RESPONSE_CANCEL,
                                           "_Salvează", GTK_RESPONSE_ACCEPT,
                                           NULL);

    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

    // Restricționează la fișiere text (utile pe Ubuntu)
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_mime_type(filter, "text/markdown");
    gtk_file_filter_add_mime_type(filter, "text/plain");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkTextBuffer *buffer;
        GtkTextIter start, end;
        gchar *text;
        GError *error = NULL;

        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (filename != NULL) {
            if (app_data->current_file) {
                g_free(app_data->current_file);
            }
            app_data->current_file = g_strdup(filename);
            gtk_window_set_title(GTK_WINDOW(app_data->window), filename);

            buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_data->text_view));
            gtk_text_buffer_get_start_iter(buffer, &start);
            gtk_text_buffer_get_end_iter(buffer, &end);
            text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

            if (!g_file_set_contents(filename, text, -1, &error)) {
                g_printerr("Eroare la salvarea fișierului: %s\n", error->message);
                g_error_free(error);
            }
            g_free(filename);
            g_free(text);
        }
    }

    gtk_widget_destroy(dialog);
}

// Funcția de callback pentru meniul "Deschide"
static void on_open_activate(GtkWidget *menuitem, AppData *app_data) {
    open_file(menuitem, app_data);
}

// Funcția de callback pentru meniul "Salvează"
static void on_save_activate(GtkWidget *menuitem, AppData *app_data) {
    save_file(menuitem, app_data);
}

// Funcția de callback pentru meniul "Salvează ca..."
static void on_save_as_activate(GtkWidget *menuitem, AppData *app_data) {
    save_file_as(menuitem, app_data);
}

// Funcția de callback pentru meniul "Ieșire"
static void on_quit_activate(GtkWidget *menuitem, AppData *app_data) {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Crează fereastra principală
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Editor Markdown Simplu");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Crează zona de text
    GtkWidget *text_view = gtk_text_view_new();
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    // Configurează derularea pentru zona de text
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);

    // Crează bara de instrumente
    GtkWidget *toolbar = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

    // Crează butonul "Deschide"
    GtkWidget *open_button = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
    gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(open_button), "Deschide fișier");
    g_signal_connect(open_button, "clicked", G_CALLBACK(on_open_activate), NULL);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), open_button, -1);

    // Crează butonul "Salvează"
    GtkWidget *save_button = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
    gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(save_button), "Salvează fișier");
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_activate), NULL);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save_button, -1);

     // Crează butonul "Ieșire"
    GtkWidget *quit_button = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
    gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(quit_button), "Ieșire");
    g_signal_connect(quit_button, "clicked", G_CALLBACK(on_quit_activate), NULL);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), quit_button, -1);

    // Creează un layout vertical și adaugă bara de instrumente, bara de meniu și zona de text
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);
    //gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, FALSE, 0);  //Elimină bara de meniu
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Afișează toate widget-urile
    gtk_widget_show_all(window);

    // Rulează bucla principală GTK+
    gtk_main();

    // Eliberează memoria pentru numele fișierului curent
    if (app_data.current_file) {
        g_free(app_data.current_file);
    }

    return 0;
}

