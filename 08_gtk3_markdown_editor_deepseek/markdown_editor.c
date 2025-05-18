#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    GtkWidget *window;
    GtkWidget *text_view;
    GtkWidget *preview_label;
    GtkWidget *paned;
    gchar *current_file;
} AppData;

static void update_preview(GtkTextBuffer *buffer, AppData *data) {
    GtkTextIter start, end;
    gchar *text;

    gtk_text_buffer_get_bounds(buffer, &start, &end);
    text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    
    // Aici ai putea adăuga un renderer Markdown mai avansat
    gtk_label_set_text(GTK_LABEL(data->preview_label), text);
    g_free(text);
}

static void save_to_file(AppData *data, const gchar *filename) {
    GtkTextIter start, end;
    gchar *text;
    FILE *file;

    gtk_text_buffer_get_bounds(gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->text_view)), &start, &end);
    text = gtk_text_buffer_get_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->text_view)), &start, &end, FALSE);

    file = fopen(filename, "w");
    if (file == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data->window),
                                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                                  GTK_MESSAGE_ERROR,
                                                  GTK_BUTTONS_OK,
                                                  "Eroare la salvarea fișierului");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        fprintf(file, "%s", text);
        fclose(file);
        data->current_file = g_strdup(filename);
        gchar *title = g_strdup_printf("Editor Markdown - %s", filename);
        gtk_window_set_title(GTK_WINDOW(data->window), title);
        g_free(title);
    }

    g_free(text);
}

static void on_save_clicked(GtkWidget *widget, AppData *data) {
    if (data->current_file != NULL) {
        save_to_file(data, data->current_file);
    } else {
        GtkWidget *dialog = gtk_file_chooser_dialog_new("Salvează fișier",
                                                        GTK_WINDOW(data->window),
                                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                                        "_Anulează", GTK_RESPONSE_CANCEL,
                                                        "_Salvează", GTK_RESPONSE_ACCEPT,
                                                        NULL);

        GtkFileFilter *filter = gtk_file_filter_new();
        gtk_file_filter_set_name(filter, "Fișiere Markdown (*.md, *.markdown)");
        gtk_file_filter_add_pattern(filter, "*.md");
        gtk_file_filter_add_pattern(filter, "*.markdown");
        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

        filter = gtk_file_filter_new();
        gtk_file_filter_set_name(filter, "Toate fișierele");
        gtk_file_filter_add_pattern(filter, "*");
        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
            gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            if (!g_str_has_suffix(filename, ".md") && !g_str_has_suffix(filename, ".markdown")) {
                gchar *new_filename = g_strconcat(filename, ".md", NULL);
                save_to_file(data, new_filename);
                g_free(new_filename);
            } else {
                save_to_file(data, filename);
            }
            g_free(filename);
        }

        gtk_widget_destroy(dialog);
    }
}

static void on_open_clicked(GtkWidget *widget, AppData *data) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Deschide fișier Markdown",
                                                   GTK_WINDOW(data->window),
                                                   GTK_FILE_CHOOSER_ACTION_OPEN,
                                                   "_Anulează", GTK_RESPONSE_CANCEL,
                                                   "_Deschide", GTK_RESPONSE_ACCEPT,
                                                   NULL);

    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Fișiere Markdown (*.md, *.markdown)");
    gtk_file_filter_add_pattern(filter, "*.md");
    gtk_file_filter_add_pattern(filter, "*.markdown");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Toate fișierele");
    gtk_file_filter_add_pattern(filter, "*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        gchar *contents;
        gsize length;

        if (g_file_get_contents(filename, &contents, &length, NULL)) {
            GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->text_view));
            gtk_text_buffer_set_text(buffer, contents, length);
            
            if (data->current_file) {
                g_free(data->current_file);
            }
            data->current_file = g_strdup(filename);
            
            gchar *title = g_strdup_printf("Editor Markdown - %s", filename);
            gtk_window_set_title(GTK_WINDOW(data->window), title);
            g_free(title);
            
            g_free(contents);
        } else {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(data->window),
                                                           GTK_DIALOG_DESTROY_WITH_PARENT,
                                                           GTK_MESSAGE_ERROR,
                                                           GTK_BUTTONS_OK,
                                                           "Eroare la deschiderea fișierului");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
        
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

static void on_quit_clicked(GtkWidget *widget, AppData *data) {
    gtk_main_quit();
}

static void create_toolbar(AppData *data) {
    GtkWidget *toolbar = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

    GtkToolItem *open_button = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
    g_signal_connect(open_button, "clicked", G_CALLBACK(on_open_clicked), data);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), open_button, -1);

    GtkToolItem *save_button = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_clicked), data);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save_button, -1);

    GtkToolItem *separator = gtk_separator_tool_item_new();
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), separator, -1);

    GtkToolItem *quit_button = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
    g_signal_connect(quit_button, "clicked", G_CALLBACK(on_quit_clicked), data);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), quit_button, -1);

    gtk_box_pack_start(GTK_BOX(gtk_bin_get_child(GTK_BIN(data->window))), toolbar, FALSE, FALSE, 0);
}

static void create_editor(AppData *data) {
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    data->text_view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(data->text_view), GTK_WRAP_WORD);
    gtk_container_add(GTK_CONTAINER(scrolled_window), data->text_view);

    g_signal_connect(gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->text_view)),
                    "changed",
                    G_CALLBACK(update_preview),
                    data);

    // Previzualizare
    GtkWidget *preview_scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(preview_scrolled),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    data->preview_label = gtk_label_new(NULL);
    gtk_label_set_line_wrap(GTK_LABEL(data->preview_label), TRUE);
    gtk_label_set_selectable(GTK_LABEL(data->preview_label), TRUE);
    gtk_container_add(GTK_CONTAINER(preview_scrolled), data->preview_label);

    // Panou divizat
    data->paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_paned_add1(GTK_PANED(data->paned), scrolled_window);
    gtk_paned_add2(GTK_PANED(data->paned), preview_scrolled);
    gtk_paned_set_position(GTK_PANED(data->paned), 400);

    gtk_box_pack_start(GTK_BOX(gtk_bin_get_child(GTK_BIN(data->window))), data->paned, TRUE, TRUE, 0);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    AppData *data = g_malloc(sizeof(AppData));
    data->current_file = NULL;

    data->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(data->window), "Editor Markdown");
    gtk_window_set_default_size(GTK_WINDOW(data->window), 800, 600);
    g_signal_connect(data->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(data->window), vbox);

    create_toolbar(data);
    create_editor(data);

    gtk_widget_show_all(data->window);
    gtk_main();

    if (data->current_file) {
        g_free(data->current_file);
    }
    g_free(data);

    return 0;
}
