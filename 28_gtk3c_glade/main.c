#include <gtk/gtk.h>

// Funcția de callback pentru butonul "Close"
void on_close_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Butonul 'Close' a fost apăsat.\n");
    GtkWindow *window = GTK_WINDOW(user_data); // Obținem fereastra principală
    gtk_widget_destroy(GTK_WIDGET(window)); // Închidem fereastra
}

// Funcția de activare a aplicației
void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *window;

    // Creează un GtkBuilder nou
    builder = gtk_builder_new();

    // Încarcă interfața din fișierul Glade
    // Asigură-te că fișierul my_app.glade este în același director cu executabilul
    if (!gtk_builder_add_from_file(builder, "my_app.glade", NULL)) {
        g_warning("Nu am putut încărca fișierul Glade: my_app.glade");
        return;
    }

    // Obține fereastra principală după ID-ul setat în Glade
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    // Conectează semnalele din Glade la funcțiile din codul C
    gtk_builder_connect_signals(builder, window); // Pasăm fereastra ca user_data pentru callback

    // Afișează fereastra
    gtk_widget_show_all(window);

    // Eliberează GtkBuilder-ul după ce nu mai este necesar
    g_object_unref(builder);
}

int main(int argc, char *argv[]) {
    GtkApplication *app;
    int status;

    // Creează o nouă aplicație GTK
    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);

    // Conectează semnalul "activate" la funcția noastră de activare
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Rulează aplicația
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Eliberează aplicația
    g_object_unref(app);

    return status;
}
