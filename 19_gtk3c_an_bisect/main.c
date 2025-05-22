#include <gtk/gtk.h>

// Funcție pentru a verifica dacă un an este bisect
gboolean is_leap_year(int year) {
    // Un an este bisect dacă este divizibil cu 4,
    // dar nu este divizibil cu 100,
    // SAU este divizibil cu 400.
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Funcția de callback pentru butonul de verificare
static void on_check_button_clicked(GtkWidget *widget, gpointer data) {
    // Obține widget-urile din structura de date pasată
    GtkEntry *year_entry = (GtkEntry *)data;
    GtkLabel *result_label = (GtkLabel *)g_object_get_data(G_OBJECT(year_entry), "result_label");

    // Obține textul din câmpul de intrare
    const char *year_text = gtk_entry_get_text(year_entry);
    int year = atoi(year_text); // Convertește textul în număr întreg

    // Verifică dacă anul este valid (nu este 0 sau negativ, deși atoi returnează 0 pentru non-numere)
    if (year <= 0) {
        gtk_label_set_text(result_label, "Introduceți un an valid.");
        return;
    }

    // Verifică dacă este an bisect și actualizează eticheta de rezultat
    if (is_leap_year(year)) {
        char result_str[50];
        sprintf(result_str, "Anul %d ESTE an bisect.", year);
        gtk_label_set_text(result_label, result_str);
    } else {
        char result_str[50];
        sprintf(result_str, "Anul %d NU este an bisect.", year);
        gtk_label_set_text(result_label, result_str);
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *year_entry;
    GtkWidget *check_button;
    GtkWidget *result_label;

    // Inițializează GTK+
    gtk_init(&argc, &argv);

    // Creează fereastra principală
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Verificator An Bisect");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Creează un container vertical (GtkBox)
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); // 10 pixeli spațiu între elemente
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Creează un câmp de intrare pentru an
    year_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(year_entry), "Introduceți anul (ex: 2024)");
    gtk_box_pack_start(GTK_BOX(vbox), year_entry, FALSE, FALSE, 0);

    // Creează butonul de verificare
    check_button = gtk_button_new_with_label("Verifică");
    gtk_box_pack_start(GTK_BOX(vbox), check_button, FALSE, FALSE, 0);

    // Creează eticheta pentru afișarea rezultatului
    result_label = gtk_label_new("Rezultat: ");
    gtk_box_pack_start(GTK_BOX(vbox), result_label, FALSE, FALSE, 0);

    // Asociază eticheta de rezultat cu câmpul de intrare, pentru a o putea accesa în callback
    g_object_set_data(G_OBJECT(year_entry), "result_label", result_label);

    // Conectează semnalul "clicked" al butonului la funcția de callback
    g_signal_connect(check_button, "clicked", G_CALLBACK(on_check_button_clicked), year_entry);

    // Afișează toate widget-urile
    gtk_widget_show_all(window);

    // Pornește bucla principală GTK+
    gtk_main();

    return 0;
}
