#include <gtk/gtk.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcebuffer.h>
#include <glib/gstdio.h>

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
                                           "_Cancel", GTK_RESPONSE_CANCEL,
                                           "_Open", GTK_RESPONSE_ACCEPT,
                                           NULL);

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
            gtk_text_buffer_set_text(buffer, "", -1); // Clear existing text

            if (!g_file_load_contents(filename, NULL, &buffer, NULL, &error)) {
                g_printerr("Eroare la deschiderea fișierului: %s\n", error->message);
                g_error_free(error);
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

        if (!g_file_save_contents(app_data->current_file, text, -1, NULL, FALSE, &error)) {
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
                                           "_Cancel", GTK_RESPONSE_CANCEL,
                                           "_Save", GTK_RESPONSE_ACCEPT,
                                           NULL);

    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

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

            if (!g_file_save_contents(filename, text, -1, NULL, FALSE, &error)) {
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

    // Crează bara de meniuri
    GtkWidget *menu_bar = gtk_menu_bar_new();

    // Crează meniul "Fișier"
    GtkWidget *file_menu = gtk_menu_new();
    GtkWidget *file_menu_item = gtk_menu_item_new_with_label("Fișier");
    GtkWidget *open_item = gtk_menu_item_new_with_label("Deschide");
    GtkWidget *save_item = gtk_menu_item_new_with_label("Salvează");
    GtkWidget *save_as_item = gtk_menu_item_new_with_label("Salvează ca...");
    GtkWidget *quit_item = gtk_menu_item_new_with_label("Ieșire");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_as_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_menu_item);

    // Conectează semnalele pentru elementele de meniu
    AppData app_data = {window, text_view, NULL};
    g_signal_connect(open_item, "activate", G_CALLBACK(on_open_activate), &app_data);
    g_signal_connect(save_item, "activate", G_CALLBACK(on_save_activate), &app_data);
    g_signal_connect(save_as_item, "activate", G_CALLBACK(on_save_as_activate), &app_data);
    g_signal_connect(quit_item, "activate", G_CALLBACK(on_quit_activate), &app_data);

    // Creează un layout vertical și adaugă bara de meniuri și zona de text
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, FALSE, 0);
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
