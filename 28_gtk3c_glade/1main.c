#include <gtk/gtk.h>

// Funcția de callback pentru butonul "Close"
void on_close_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Butonul 'Close' a fost apăsat.\n");
    // Obținem fereastra părinte (top-level) a butonului
    GtkWidget *toplevel_window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    if (GTK_IS_WINDOW(toplevel_window)) {
        gtk_widget_destroy(toplevel_window); // Închidem fereastra
    }
}

// Funcția de callback pentru evenimentul de închidere a ferestrei (butonul X)
gboolean on_main_window_delete_event(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    g_print("Fereastra a fost închisă prin 'X' sau altă acțiune de închidere.\n");
    return FALSE; // Returnează FALSE pentru a lăsa GTK să proceseze evenimentul (adică, să închidă fereastra)
}


// Funcția de activare a aplicației
void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *window;

    // Creează un GtkBuilder nou
    builder = gtk_builder_new();

    // Încarcă interfața din fișierul Glade
    if (!gtk_builder_add_from_file(builder, "my_app.glade", NULL)) {
        g_warning("Nu am putut încărca fișierul Glade: my_app.glade");
        g_object_unref(builder); // Eliberează builder-ul chiar și la eroare
        return;
    }

    // Obține fereastra principală după ID-ul setat în Glade
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    // Conectează semnalul "delete-event" al ferestrei pentru a închide aplicația
    // Acesta este evenimentul standard pentru închiderea ferestrei prin butonul "X"
    g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(on_main_window_delete_event), NULL);

    // Conectează toate semnalele din Glade (inclusiv pe cel pentru close_button)
    // IMPORTANT: Aici NU pasăm 'window' ca user_data, deoarece GtkBuilder leagă direct la handler
    gtk_builder_connect_signals(builder, NULL); // NULL aici este corect pentru a nu suprascrie user_data al semnalelor din Glade

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

    // Rulează aplicația. Aceasta va menține fereastra deschisă.
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Eliberează aplicația
    g_object_unref(app);

    return status;
}
