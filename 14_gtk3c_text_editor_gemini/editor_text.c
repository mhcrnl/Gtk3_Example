#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structura pentru datele aplicatiei
typedef struct {
    GtkWidget *window;
    GtkWidget *text_view;
    GtkWidget *filename_label;
    char *current_filename;
} AppData;

// Functie pentru crearea ferestrei "About"
static void show_about_dialog(GtkWidget *parent) {
    GtkWidget *about_dialog;
    about_dialog = gtk_about_dialog_new();

    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(about_dialog), "Editor de Text Simplu");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about_dialog), "1.0");
    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(about_dialog), (const gchar *[]){"Your Name", NULL});
    gtk_about_dialog_set_documenters(GTK_ABOUT_DIALOG(about_dialog), (const gchar *[]){"Your Name", NULL});
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(about_dialog), "https://example.com");
    gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(about_dialog), "Site Web");
    gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(about_dialog), "MIT");
    gtk_window_set_transient_for(GTK_WINDOW(about_dialog), GTK_WINDOW(parent));
    gtk_dialog_run(GTK_DIALOG(about_dialog));
    gtk_widget_destroy(about_dialog);
}

// Functie pentru deschiderea unui fisier
static void open_file(GtkWidget *menu_item, AppData *app_data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    dialog = gtk_file_chooser_dialog_new("Deschide Fisier",
                                           GTK_WINDOW(app_data->window),
                                           action,
                                           "Cancel", GTK_RESPONSE_CANCEL,
                                           "Open", GTK_RESPONSE_ACCEPT,
                                           NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkTextBuffer *buffer;
        GError *error = NULL;

        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (app_data->current_filename) {
            g_free(app_data->current_filename);
        }
        app_data->current_filename = g_strdup(filename);
        gtk_label_set_text(GTK_LABEL(app_data->filename_label), filename);

        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_data->text_view));
        gtk_text_buffer_set_text(buffer, "", -1); // Clear existing text

        if (!gtk_text_buffer_load_from_file(buffer, filename, &error)) {
            g_printerr("Eroare la incarcarea fisierului: %s\n", error->message);
            g_error_free(error);
        }
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

// Functie pentru salvarea fisierului
static void save_file(GtkWidget *menu_item, AppData *app_data) {
    if (app_data->current_filename) {
        GtkTextBuffer *buffer;
        GError *error = NULL;
        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_data->text_view));
        GtkTextIter start, end;
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
        gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        if (!g_file_set_contents(app_data->current_filename, text, -1, &error)) {
            g_printerr("Eroare la salvarea fisierului: %s\n", error->message);
            g_error_free(error);
        }
        g_free(text);
    } else {
        save_file_as(menu_item, app_data);
    }
}

// Functie pentru salvarea fisierului ca...
static void save_file_as(GtkWidget *menu_item, AppData *app_data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;

    dialog = gtk_file_chooser_dialog_new("Salveaza Fisier Ca...",
                                           GTK_WINDOW(app_data->window),
                                           action,
                                           "Cancel", GTK_RESPONSE_CANCEL,
                                           "Save", GTK_RESPONSE_ACCEPT,
                                           NULL);

    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkTextBuffer *buffer;
        GError *error = NULL;

        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (app_data->current_filename) {
            g_free(app_data->current_filename);
        }
        app_data->current_filename = g_strdup(filename);
        gtk_label_set_text(GTK_LABEL(app_data->filename_label), filename);

        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_data->text_view));
        GtkTextIter start, end;
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
        gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        if (!g_file_set_contents(filename, text, -1, &error)) {
            g_printerr("Eroare la salvarea fisierului: %s\n", error->message);
            g_error_free(error);
        }
        g_free(text);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

// Functie pentru crearea meniului "File"
static GtkWidget *create_file_menu(AppData *app_data) {
    GtkWidget *menu, *menu_item;

    menu = gtk_menu_new();

    menu_item = gtk_menu_item_new_with_label("Deschide");
    g_signal_connect(menu_item, "activate", G_CALLBACK(open_file), app_data);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    menu_item = gtk_menu_item_new_with_label("Salveaza");
    g_signal_connect(menu_item, "activate", G_CALLBACK(save_file), app_data);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    menu_item = gtk_menu_item_new_with_label("Salveaza Ca...");
    g_signal_connect(menu_item, "activate", G_CALLBACK(save_file_as), app_data);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());

    menu_item = gtk_menu_item_new_with_label("Iesire");
    g_signal_connect(menu_item, "activate", G_CALLBACK(gtk_main_quit), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    return menu;
}

// Functie pentru crearea meniului "View"
static GtkWidget *create_view_menu(AppData *app_data) {
    GtkWidget *menu, *menu_item;

    menu = gtk_menu_new();

    // Momentan, meniul "View" este gol. Se pot adauga optiuni de vizualizare in viitor.

    return menu;
}

// Functie pentru crearea meniului "About"
static GtkWidget *create_about_menu(AppData *app_data) {
    GtkWidget *menu, *menu_item;

    menu = gtk_menu_new();

    menu_item = gtk_menu_item_new_with_label("Despre");
    g_signal_connect(menu_item, "activate", G_CALLBACK(show_about_dialog), app_data->window);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    return menu;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    AppData app_data;
    app_data.current_filename = NULL;

    // Crearea ferestrei principale
    app_data.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(app_data.window), "Editor de Text Simplu");
    gtk_window_set_default_size(GTK_WINDOW(app_data.window), 600, 400);
    g_signal_connect(app_data.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Crearea casetei verticale principale
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(app_data.window), vbox);

    // Crearea meniului
    GtkWidget *menubar = gtk_menu_bar_new();
    GtkWidget *file_menu_item = gtk_menu_item_new_with_label("Fisier");
    GtkWidget *view_menu_item = gtk_menu_item_new_with_label("Vizualizare");
    GtkWidget *about_menu_item = gtk_menu_item_new_with_label("Despre");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), create_file_menu(&app_data));
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_menu_item), create_view_menu(&app_data));
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(about_menu_item), create_about_menu(&app_data));

    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), view_menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), about_menu_item);

    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    // Crearea zonei de text
    app_data.text_view = gtk_text_view_new();
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), app_data.text_view);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    // Crearea etichetei pentru numele fisierului
    app_data.filename_label = gtk_label_new("Niciun fisier deschis");
    gtk_box_pack_end(GTK_BOX(vbox), app_data.filename_label, FALSE, FALSE, 5);

    gtk_widget_show_all(app_data.window);

    gtk_main();

    if (app_data.current_filename) {
        g_free(app_data.current_filename);
    }

    return 0;
}
