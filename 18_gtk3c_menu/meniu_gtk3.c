#include <gtk/gtk.h>

// Funcție callback pentru opțiunea „Deschide”
static void on_open_activate(GtkMenuItem *menuitem, gpointer user_data) {
    g_print("Opțiunea 'Deschide' a fost selectată.\n");
}

// Funcție callback pentru opțiunea „Despre”
static void on_about_activate(GtkMenuItem *menuitem, gpointer user_data) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(user_data),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "Aplicație GTK3\nVersiunea 1.0");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *menubar;
    GtkWidget *file_menu;
    GtkWidget *help_menu;
    GtkWidget *file_item;
    GtkWidget *help_item;
    GtkWidget *open_item;
    GtkWidget *about_item;

    gtk_init(&argc, &argv);

    // Crearea ferestrei principale
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Exemplu Meniu GTK3");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Crearea unui container vertical
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Crearea barei de meniu
    menubar = gtk_menu_bar_new();

    // Crearea meniului „Fișier”
    file_menu = gtk_menu_new();
    file_item = gtk_menu_item_new_with_label("Fișier");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);

    open_item = gtk_menu_item_new_with_label("Deschide");
    g_signal_connect(open_item, "activate", G_CALLBACK(on_open_activate), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open_item);

    // Crearea meniului „Ajutor”
    help_menu = gtk_menu_new();
    help_item = gtk_menu_item_new_with_label("Ajutor");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_item), help_menu);

    about_item = gtk_menu_item_new_with_label("Despre");
    g_signal_connect(about_item, "activate", G_CALLBACK(on_about_activate), window);
    gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), about_item);

    // Adăugarea meniurilor la bara de meniu
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help_item);

    // Adăugarea barei de meniu la containerul vertical
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

