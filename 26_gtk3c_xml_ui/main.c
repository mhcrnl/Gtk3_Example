#include <gtk/gtk.h>

// Declararea funcției de callback (handler-ul pentru semnalul "clicked")
static void on_my_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Butonul a fost apasat!\n");
    // Aici puteti adauga logica aplicatiei, de exemplu, actualizarea unui label
    GtkLabel *label = GTK_LABEL(user_data); // Presupunem ca user_data este un GtkLabel
    if (label) {
        gtk_label_set_text(label, "Ai apasat butonul!");
    }
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *label; // Vom obtine referinta la label pentru a-l actualiza

    // 1. Creare GtkBuilder
    builder = gtk_builder_new();

    // 2. Încărcare UI din fișierul XML
    // Asigurati-va ca "ui_example.ui" este in aceeasi locatie cu executabilul
    // sau specificati calea completa.
    if (!gtk_builder_add_from_file(builder, "ui_example.ui", NULL)) {
        g_warning("Nu s-a putut încărca fișierul UI!");
        g_object_unref(builder);
        return;
    }

    // 3. Obținere referințe la widget-uri prin ID-urile lor
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    button = GTK_WIDGET(gtk_builder_get_object(builder, "my_button"));
    label = GTK_WIDGET(gtk_builder_get_object(builder, "my_label")); // Daca avem un label cu ID "my_label" in Glade

    // Conectarea ferestrei la aplicație (necesar pentru GtkApplication)
    gtk_application_add_window(app, GTK_WINDOW(window));

    // Exemplu de conectare manuala (daca nu a fost facuta in Glade)
    // g_signal_connect(button, "clicked", G_CALLBACK(on_my_button_clicked), NULL);

    // Conectarea semnalelor definite în Glade.
    // Argumentul user_data va fi pasat la handler-ul semnalului.
    // In acest caz, vom conecta butonul la functie si vom pasa label-ul ca user_data
    gtk_builder_connect_signals(builder, label); // Acest lucru va conecta TOATE semnalele specificate in UI la functii cu acelasi nume

    // 4. Afisare fereastra
    gtk_widget_show_all(window);

    // Eliberare referinta la GtkBuilder (după ce toate widget-urile au fost instanțiate)
    g_object_unref(builder);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example.GtkBuilderExample", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
