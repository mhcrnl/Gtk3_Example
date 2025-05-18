#include <gtk/gtk.h>

#define WORK_DURATION 25 * 60  // 25 minute
#define SHORT_BREAK 5 * 60     // 5 minute
#define LONG_BREAK 15 * 60     // 15 minute

static GtkWidget *label;
static GtkWidget *start_button;
static GtkWidget *pause_button;
static GtkWidget *reset_button;

static guint timer_id = 0;
static int remaining_time = WORK_DURATION;
static int pomodoro_count = 0;
static gboolean is_running = FALSE;
static gboolean is_break = FALSE;

static void update_label() {
    int minutes = remaining_time / 60;
    int seconds = remaining_time % 60;
    gchar *text = g_strdup_printf("%02d:%02d", minutes, seconds);
    gtk_label_set_text(GTK_LABEL(label), text);
    g_free(text);
}

static gboolean timer_callback(gpointer data) {
    if (remaining_time > 0) {
        remaining_time--;
        update_label();
        return TRUE;
    } else {
        if (!is_break) {
            pomodoro_count++;
            if (pomodoro_count % 4 == 0) {
                remaining_time = LONG_BREAK;
            } else {
                remaining_time = SHORT_BREAK;
            }
            is_break = TRUE;
        } else {
            remaining_time = WORK_DURATION;
            is_break = FALSE;
        }
        update_label();
        return TRUE;
    }
}

static void start_timer(GtkWidget *widget, gpointer data) {
    if (!is_running) {
        timer_id = g_timeout_add_seconds(1, timer_callback, NULL);
        is_running = TRUE;
    }
}

static void pause_timer(GtkWidget *widget, gpointer data) {
    if (is_running) {
        g_source_remove(timer_id);
        is_running = FALSE;
    }
}

static void reset_timer(GtkWidget *widget, gpointer data) {
    if (is_running) {
        g_source_remove(timer_id);
        is_running = FALSE;
    }
    remaining_time = WORK_DURATION;
    is_break = FALSE;
    update_label();
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *hbox;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Pomodoro Timer");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    label = gtk_label_new("25:00");
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

    start_button = gtk_button_new_with_label("Start");
    g_signal_connect(start_button, "clicked", G_CALLBACK(start_timer), NULL);
    gtk_box_pack_start(GTK_BOX(hbox), start_button, TRUE, TRUE, 0);

    pause_button = gtk_button_new_with_label("Pause");
    g_signal_connect(pause_button, "clicked", G_CALLBACK(pause_timer), NULL);
    gtk_box_pack_start(GTK_BOX(hbox), pause_button, TRUE, TRUE, 0);

    reset_button = gtk_button_new_with_label("Reset");
    g_signal_connect(reset_button, "clicked", G_CALLBACK(reset_timer), NULL);
    gtk_box_pack_start(GTK_BOX(hbox), reset_button, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.example.pomodoro", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

