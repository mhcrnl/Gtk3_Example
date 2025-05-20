#include <gtk/gtk.h>

#define POMODORO_DURATION 25 * 60 // 25 de minute în secunde

static GtkWidget *label_timer;
static guint timer_id = 0;
static gint remaining_time = POMODORO_DURATION;
static gboolean is_running = FALSE;

// Funcție pentru actualizarea etichetei cu timpul rămas
static void update_timer_label() {
    gint minutes = remaining_time / 60;
    gint seconds = remaining_time % 60;
    gchar time_str[6];
    g_snprintf(time_str, sizeof(time_str), "%02d:%02d", minutes, seconds);
    gtk_label_set_text(GTK_LABEL(label_timer), time_str);
}

// Funcție apelată la fiecare secundă
static gboolean on_timeout(gpointer user_data) {
    if (remaining_time > 0) {
        remaining_time--;
        update_timer_label();
        return TRUE;
    } else {
        is_running = FALSE;
        timer_id = 0;
        // Afișează un dialog la finalul sesiunii
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(user_data),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "Sesiunea Pomodoro s-a încheiat!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return FALSE;
    }
}

// Handler pentru butonul Start
void on_start_clicked(GtkButton *button, gpointer user_data) {
    if (!is_running) {
        is_running = TRUE;
        timer_id = g_timeout_add_seconds(1, on_timeout, user_data);
    }
}

// Handler pentru butonul Pause
void on_pause_clicked(GtkButton *button, gpointer user_data) {
    if (is_running) {
        is_running = FALSE;
        if (timer_id != 0) {
            g_source_remove(timer_id);
            timer_id = 0;
        }
    }
}

// Handler pentru butonul Reset
void on_reset_clicked(GtkButton *button, gpointer user_data) {
    is_running = FALSE;
    if (timer_id != 0) {
        g_source_remove(timer_id);
        timer_id = 0;
    }
    remaining_time = POMODORO_DURATION;
    update_timer_label();
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;
    GError *error = NULL;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "pomodoro.ui", &error)) {
        g_printerr("Eroare la încărcarea fișierului UI: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    gtk_builder_connect_signals(builder, NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    label_timer = GTK_WIDGET(gtk_builder_get_object(builder, "label_timer"));

    if (!window || !label_timer) {
        g_printerr("Nu s-au putut obține widget-urile din fișierul UI.\n");
        return 1;
    }

    update_timer_label();

    gtk_widget_show_all(window);
    gtk_main();

    g_object_unref(builder);
    return 0;
}

