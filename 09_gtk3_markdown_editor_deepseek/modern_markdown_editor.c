#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <stdlib.h>

typedef struct {
    GtkWidget *window;
    GtkWidget *text_view;
    GtkWidget *preview_label;
    GtkWidget *header_bar;
    gchar *current_file;
} AppData;

static void update_preview(GtkTextBuffer *buffer, AppData *data) {
    GtkTextIter start, end;
    gchar *text;

    gtk_text_buffer_get_bounds(buffer, &start, &end);
    text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    
    // Basic markdown rendering (would be replaced with a proper renderer)
    gtk_label_set_markup(GTK_LABEL(data->preview_label), text);
    g_free(text);
}

static gboolean save_to_file(AppData *data, const gchar *filename) {
    GtkTextIter start, end;
    gchar *text;
    GError *error = NULL;

    gtk_text_buffer_get_bounds(gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->text_view)), 
                              &start, &end);
    text = gtk_text_buffer_get_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->text_view)), 
                                  &start, &end, FALSE);

    if (!g_file_set_contents(filename, text, -1, &error)) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data->window),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_ERROR,
                                                  GTK_BUTTONS_OK,
                                                  "Error saving file: %s", 
                                                  error->message);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        g_error_free(error);
        g_free(text);
        return FALSE;
    }

    g_free(data->current_file);
    data->current_file = g_strdup(filename);
    
    gchar *title = g_strdup_printf("Markdown Editor - %s", g_path_get_basename(filename));
    gtk_header_bar_set_title(GTK_HEADER_BAR(data->header_bar), title);
    gtk_header_bar_set_subtitle(GTK_HEADER_BAR(data->header_bar), filename);
    g_free(title);
    
    g_free(text);
    return TRUE;
}

static void on_save(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    AppData *data = user_data;
    
    if (data->current_file) {
        save_to_file(data, data->current_file);
    } else {
        on_save_as(action, parameter, user_data);
    }
}

static void on_save_as(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    AppData *data = user_data;
    
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Save File",
                                                   GTK_WINDOW(data->window),
                                                   GTK_FILE_CHOOSER_ACTION_SAVE,
                                                   "_Cancel", GTK_RESPONSE_CANCEL,
                                                   "_Save", GTK_RESPONSE_ACCEPT,
                                                   NULL);

    // Set filters
    GtkFileFilter *filter_md = gtk_file_filter_new();
    gtk_file_filter_set_name(filter_md, "Markdown Files");
    gtk_file_filter_add_pattern(filter_md, "*.md");
    gtk_file_filter_add_pattern(filter_md, "*.markdown");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_md);

    GtkFileFilter *filter_all = gtk_file_filter_new();
    gtk_file_filter_set_name(filter_all, "All Files");
    gtk_file_filter_add_pattern(filter_all, "*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_all);

    // Suggest a filename
    if (data->current_file) {
        gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), 
                                         g_path_get_basename(data->current_file));
    } else {
        gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "untitled.md");
    }

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        save_to_file(data, filename);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

static void on_open(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    AppData *data = user_data;
    
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
                                                   GTK_WINDOW(data->window),
                                                   GTK_FILE_CHOOSER_ACTION_OPEN,
                                                   "_Cancel", GTK_RESPONSE_CANCEL,
                                                   "_Open", GTK_RESPONSE_ACCEPT,
                                                   NULL);

    // Set filters
    GtkFileFilter *filter_md = gtk_file_filter_new();
    gtk_file_filter_set_name(filter_md, "Markdown Files");
    gtk_file_filter_add_pattern(filter_md, "*.md");
    gtk_file_filter_add_pattern(filter_md, "*.markdown");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_md);

    GtkFileFilter *filter_all = gtk_file_filter_new();
    gtk_file_filter_set_name(filter_all, "All Files");
    gtk_file_filter_add_pattern(filter_all, "*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_all);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        gchar *contents;
        gsize length;
        GError *error = NULL;

        if (g_file_get_contents(filename, &contents, &length, &error)) {
            GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->text_view));
            gtk_text_buffer_set_text(buffer, contents, length);
            
            g_free(data->current_file);
            data->current_file = g_strdup(filename);
            
            gchar *title = g_strdup_printf("Markdown Editor - %s", g_path_get_basename(filename));
            gtk_header_bar_set_title(GTK_HEADER_BAR(data->header_bar), title);
            gtk_header_bar_set_subtitle(GTK_HEADER_BAR(data->header_bar), filename);
            g_free(title);
            
            g_free(contents);
        } else {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(data->window),
                                                           GTK_DIALOG_MODAL,
                                                           GTK_MESSAGE_ERROR,
                                                           GTK_BUTTONS_OK,
                                                           "Error opening file: %s", 
                                                           error->message);
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
            g_error_free(error);
        }
        
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

static void on_quit(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_application_quit(G_APPLICATION(user_data));
}

static void setup_actions(AppData *data, GtkApplication *app) {
    const GActionEntry app_entries[] = {
        { "open", on_open, NULL, NULL, NULL },
        { "save", on_save, NULL, NULL, NULL },
        { "saveas", on_save_as, NULL, NULL, NULL },
        { "quit", on_quit, NULL, NULL, NULL }
    };
    
    g_action_map_add_action_entries(G_ACTION_MAP(app),
                                  app_entries,
                                  G_N_ELEMENTS(app_entries),
                                  data);
}

static void create_ui(AppData *data, GtkApplication *app) {
    // Main window
    data->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(data->window), "Markdown Editor");
    gtk_window_set_default_size(GTK_WINDOW(data->window), 800, 600);
    
    // Header bar
    data->header_bar = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(data->header_bar), TRUE);
    gtk_header_bar_set_title(GTK_HEADER_BAR(data->header_bar), "Markdown Editor");
    gtk_window_set_titlebar(GTK_WINDOW(data->window), data->header_bar);
    
    // Create menu button
    GtkWidget *menu_button = gtk_menu_button_new();
    GtkWidget *menu_icon = gtk_image_new_from_icon_name("open-menu-symbolic", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(menu_button), menu_icon);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(data->header_bar), menu_button);
    
    // Create menu
    GMenu *menu = g_menu_new();
    g_menu_append(menu, "Open", "app.open");
    g_menu_append(menu, "Save", "app.save");
    g_menu_append(menu, "Save As...", "app.saveas");
    g_menu_append(menu, "Quit", "app.quit");
    
    GtkWidget *popover = gtk_popover_new_from_model(GTK_WIDGET(menu_button), G_MENU_MODEL(menu));
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(menu_button), popover);
    
    // Main container
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(data->window), paned);
    
    // Editor
    GtkWidget *editor_scrolled = gtk_scrolled_window_new(NULL, NULL);
    data->text_view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(data->text_view), GTK_WRAP_WORD_CHAR);
    gtk_container_add(GTK_CONTAINER(editor_scrolled), data->text_view);
    gtk_paned_add1(GTK_PANED(paned), editor_scrolled);
    
    // Preview
    GtkWidget *preview_scrolled = gtk_scrolled_window_new(NULL, NULL);
    data->preview_label = gtk_label_new(NULL);
    gtk_label_set_line_wrap(GTK_LABEL(data->preview_label), TRUE);
    gtk_label_set_selectable(GTK_LABEL(data->preview_label), TRUE);
    gtk_label_set_xalign(GTK_LABEL(data->preview_label), 0.0);
    gtk_label_set_yalign(GTK_LABEL(data->preview_label), 0.0);
    gtk_container_add(GTK_CONTAINER(preview_scrolled), data->preview_label);
    gtk_paned_add2(GTK_PANED(paned), preview_scrolled);
    
    // Set initial pane position
    gtk_paned_set_position(GTK_PANED(paned), 400);
    
    // Connect text buffer signal
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->text_view));
    g_signal_connect(buffer, "changed", G_CALLBACK(update_preview), data);
}

static void activate(GtkApplication *app, gpointer user_data) {
    AppData *data = g_new0(AppData, 1);
    setup_actions(data, app);
    create_ui(data, app);
    gtk_widget_show_all(data->window);
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("com.example.markdown-editor", 
                                            G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    
    return status;
}
