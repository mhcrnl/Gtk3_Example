#include <gtk/gtk.h>

#define POMODORO_DURATION 25 * 60  // 25 minute în secunde

static GtkWidget *label;
static GtkWidget *start_button;
static guint timer_id = 0;
static int remaining_time = POMODORO_DURATION;
static gboolean is_running = FALSE;

static void update_label() {
    int minutes = remaining_time / 60;
    int seconds = remaining_time % 60;
    gchar *time_str = g_strdup_printf("%02d:%02d", minutes, seconds);
    gtk_label_set_text(GTK_LABEL(label), time_str);
    g_free(time_str);
}

static gboolean on_timer_tick(gpointer user_data) {
    if (remaining_time > 0) {
        remaining_time--;
        update_label();
        return G_SOURCE_CONTINUE;
    } else {
        gtk_label_set_text(GTK_LABEL(label), "Timpul a expirat!");
        is_running = FALSE;
        gtk_button_set_label(GTK_BUTTON(start_button), "Start");
        return G_SOURCE_REMOVE;
    }
}

static void on_start_button_clicked(GtkButton *button, gpointer user_data) {
    if (!is_running) {
        remaining_time = POMODORO_DURATION;
        update_label();
        timer_id = g_timeout_add_seconds(1, on_timer_tick, NULL);
        gtk_button_set_label(button, "Stop");
        is_running = TRUE;
    } else {
        if (timer_id != 0) {
            g_source_remove(timer_id);
            timer_id = 0;
        }
        gtk_button_set_label(button, "Start");
        is_running = FALSE;
    }
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *box;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Pomodoro GTK4");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_bottom(box, 20);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);
    gtk_window_set_child(GTK_WINDOW(window), box);

    label = gtk_label_new("25:00");
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), label);

    start_button = gtk_button_new_with_label("Start");
    gtk_box_append(GTK_BOX(box), start_button);
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start_button_clicked), NULL);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.example.pomodoro", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

