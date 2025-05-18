#include <gtk/gtk.h>

// Funcție apelată la apăsarea butonului
void on_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Butonul a fost apăsat!\n");
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *button;
    GError *error = NULL;

    // Inițializare GTK
    gtk_init(&argc, &argv);

    // Creare builder și încărcare fișier .ui
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "interface.glade", &error)) {
        g_printerr("Eroare la încărcarea fișierului UI: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Obținere referințe către widget-uri
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    button = GTK_WIDGET(gtk_builder_get_object(builder, "btn_click_me"));

    if (!window || !button) {
        g_printerr("Nu s-au putut obține widget-urile din fișierul UI.\n");
        return 1;
    }

    // Conectare semnal pentru buton
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    // Afișare fereastră
    gtk_widget_show_all(window);

    // Bucla principală GTK
    gtk_main();

    // Eliberare resurse
    g_object_unref(builder);

    return 0;
}

