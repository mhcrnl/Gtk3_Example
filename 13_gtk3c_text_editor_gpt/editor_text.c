#include <gtk/gtk.h>

static GtkWidget *text_view;
static GtkWidget *line_numbers;

static void on_open_activate(GtkMenuItem *menuitem, gpointer user_data) {
    GtkWidget *dialog;
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

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

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

static void on_save_activate(GtkMenuItem *menuitem, gpointer user_data) {
    GtkWidget *dialog;
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

    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "document.txt");

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

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

static void on_quit_activate(GtkMenuItem *menuitem, gpointer user_data) {
    gtk_window_close(GTK_WINDOW(user_data));
}

static void on_toggle_line_numbers(GtkCheckMenuItem *menuitem, gpointer user_data) {
    gboolean active = gtk_check_menu_item_get_active(menuitem);
    gtk_text_view_set_show_line_numbers(GTK_TEXT_VIEW(text_view), active);
}

static void on_about_activate(GtkMenuItem *menuitem, gpointer user_data) {
    GtkWidget *about_dialog = gtk_about_dialog_new();
    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(about_dialog), "Editor de Text GTK3");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about_dialog), "1.0");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(about_dialog), "Un editor de text simplu folosind GTK3 și C.");
    gtk_dialog_run(GTK_DIALOG(about_dialog));
    gtk_widget_destroy(about_dialog);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *menubar;
    GtkWidget *file_menu;
    GtkWidget *view_menu;
    GtkWidget *help_menu;
    GtkWidget *file_menu_item;
    GtkWidget *view_menu_item;
    GtkWidget *help_menu_item;
    GtkWidget *open_item;
    GtkWidget *save_item;
    GtkWidget *quit_item;
    GtkWidget *line_numbers_item;
    GtkWidget *about_item;
    GtkWidget *scrolled_window;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Editor de Text GTK3");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    menubar = gtk_menu_bar_new();

    // File Menu
    file_menu = gtk_menu_new();
    file_menu_item = gtk_menu_item_new_with_label("Fișier");
    open_item = gtk_menu_item_new_with_label("Deschide");
    save_item = gtk_menu_item_new_with_label("Salvează");
    quit_item = gtk_menu_item_new_with_label("Ieșire");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit_item);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_menu_item);

    // View Menu
    view_menu = gtk_menu_new();
    view_menu_item = gtk_menu_item_new_with_label("Vizualizare");
    line_numbers_item = gtk_check_menu_item_new_with_label("Afișează numerele de linii");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(line_numbers_item), TRUE);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), line_numbers_item);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_menu_item), view_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), view_menu_item);

    // Help Menu
    help_menu = gtk_menu_new();
    help_menu_item = gtk_menu_item_new_with_label("Ajutor");
    about_item = gtk_menu_item_new_with_label("Despre");
    gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), about_item);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_menu_item), help_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help_menu_item);

    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    text_view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
    gtk_text_view_set_show_line_numbers(GTK_TEXT_VIEW(text_view), TRUE);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

    // Conectarea semnalelor
    g_signal_connect(open_item, "activate", G_CALLBACK(on_open_activate), window);
    g_signal_connect(save_item, "activate", G_CALLBACK(on_save_activate), window);
    g_signal_connect(quit_item, "activate", G_CALLBACK(on_quit_activate), window);
    g_signal_connect(line_numbers_item, "toggled", G_CALLBACK(on_toggle_line_numbers), NULL);
    g_signal_connect(about_item, "activate", G_CALLBACK(on_about_activate), window);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.example.texteditor", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

