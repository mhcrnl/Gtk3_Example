#include <gtk/gtk.h>

// Funcția de callback care se apelează la apăsarea butonului "Închide"
static void on_close_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Butonul 'Închide Aplicația' a fost apăsat.\n");
    GtkWindow *window = GTK_WINDOW(user_data); // Obținem referința la fereastră
    gtk_window_close(window); // Închidem fereastra
}

// Funcția de activare a aplicației
static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *button;

    // Creăm un GtkBuilder nou
    builder = gtk_builder_new();

    // Încărcăm fișierul XML cu interfața grafică
    if (gtk_builder_add_from_file(builder, "/ui.xml", NULL) == 0) {
        g_warning("Nu s-a putut încărca fișierul ui.xml. Asigurați-vă că este în același director cu executabilul.\n");
        g_object_unref(builder);
        return;
    }

    // Obținem referința la fereastra principală după ID-ul său
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    if (window == NULL) {
        g_warning("Nu s-a putut găsi fereastra cu ID-ul 'main_window' în ui.xml.\n");
        g_object_unref(builder);
        return;
    }

    // Obținem referința la butonul "Închide Aplicația" după ID-ul său
    button = GTK_WIDGET(gtk_builder_get_object(builder, "close_button"));
    if (button == NULL) {
        g_warning("Nu s-a putut găsi butonul cu ID-ul 'close_button' în ui.xml.\n");
        g_object_unref(builder);
        return;
    }

    // Conectăm semnalul "clicked" al butonului la funcția noastră de callback
    // Transmitem fereastra ca user_data pentru a o putea închide
    g_signal_connect(button, "clicked", G_CALLBACK(on_close_button_clicked), window);

    // Adăugăm fereastra la aplicație (pentru a o gestiona corect de GTK)
    gtk_application_add_window(app, GTK_WINDOW(window));

    // Arătăm toate widget-urile din fereastră (inclusiv cele din grilă)
    gtk_widget_show_all(window);

    // Eliberăm GtkBuilder-ul (nu mai avem nevoie de el după ce am construit interfața)
    g_object_unref(builder);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Creăm o nouă aplicație GTK
    app = gtk_application_new("org.gtk.aplicatie-simpla", G_APPLICATION_FLAGS_NONE);

    // Conectăm semnalul "activate" la funcția noastră activate
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Rulăm aplicația
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Eliberăm aplicația
    g_object_unref(app);

    return status;
}
