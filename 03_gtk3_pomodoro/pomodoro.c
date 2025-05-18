#include <gtk/gtk.h>
#include <time.h>

#define POMODORO_DURATION 25 * 60  // 25 de minute în secunde

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

static gboolean on_timer_tick(gpointer data) {
    if (remaining_time > 0) {
        remaining_time--;
        update_label();
        return TRUE;
    } else {
        gtk_label_set_text(GTK_LABEL(label), "Timpul a expirat!");
        is_running = FALSE;
        gtk_button_set_label(GTK_BUTTON(start_button), "Start");
        return FALSE; // Oprire timer
    }
}

static void on_start_button_clicked(GtkButton *button, gpointer user_data) {
    if (!is_running) {
        // Pornește pomodoro
        remaining_time = POMODORO_DURATION;
        update_label();
        timer_id = g_timeout_add_seconds(1, on_timer_tick, NULL);
        gtk_button_set_label(button, "Stop");
        is_running = TRUE;
    } else {
        // Oprește pomodoro
        if (timer_id > 0) {
            g_source_remove(timer_id);
            timer_id = 0;
        }
        gtk_button_set_label(button, "Start");
        is_running = FALSE;
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Pomodoro Timer");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 150);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    label = gtk_label_new("25:00");
    gtk_widget_set_name(label, "timer-label");
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

    start_button = gtk_button_new_with_label("Start");
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), start_button, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

