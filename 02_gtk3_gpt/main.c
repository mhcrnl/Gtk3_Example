#include <gtk/gtk.h>

// Funcția apelată la apăsarea butonului
static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Butonul a fost apăsat!\n");
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    
    GtkWidget *button;
    
    GtkWidget *fileMenu;
    GtkWidget *menubar;
    GtkWidget *fileMi;
    GtkWidget *quitMi;

    // Inițializarea GTK
    gtk_init(&argc, &argv);

    // Crearea ferestrei principale
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Aplicație GTK3 în C");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Conectarea semnalului de închidere
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    vbox = gtk_box_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    menubar = gtk_menu_bar_new();
    fileMenu = gtk_menu_new();
    fileMi = gtk_menu_item_new_with_label("File");
    quitMi = gtk_menu_item_new_with_label("Quit");
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
    
    g_signal_connect(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

     g_signal_connect(G_OBJECT(quitMi), "activate",
        G_CALLBACK(gtk_main_quit), NULL);

    // Crearea butonului
    button = gtk_button_new_with_label("Apasă-mă!");
    g_signal_connect(button, "clicked",    G_CALLBACK(on_button_clicked), NULL);

    // Adăugarea butonului în fereastră
    gtk_container_add(GTK_CONTAINER(window), button);

    // Afișarea tuturor widgeturilor
    gtk_widget_show_all(window);

    // Bucla principală GTK
    gtk_main();

    return 0;
}

