#include <stdlib.h>
#include <gtk/gtk.h>

static void on_open_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Optiune a fost activata.\n");
}

static void helloWorld (GtkWidget *wid, GtkWidget *win)
{
  GtkWidget *dialog = NULL;

  dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Hello World!");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

int main (int argc, char *argv[])
{
  GtkWidget *button = NULL;
  GtkWidget *win = NULL;
  GtkWidget *vbox = NULL;

  //Meniu bara
  GtkWidget *menubar = NULL;
  GtkWidget *file_menu = NULL;
  GtkWidget *help_menu = NULL;
  GtkWidget *file_item = NULL;
  GtkWidget *help_item = NULL;
  GtkWidget *open_item = NULL;
  GtkWidget *about_item = NULL;

  /* Initialize GTK+ */
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
  gtk_init (&argc, &argv);
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

  /* Create the main window */
  win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER (win), 8);
  gtk_window_set_title (GTK_WINDOW (win), "Hello World");
  gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
  gtk_widget_realize (win);
  g_signal_connect (win, "destroy", gtk_main_quit, NULL);

  /* Create a vertical box with buttons */
  vbox = gtk_vbox_new (TRUE, 6);
  gtk_container_add (GTK_CONTAINER (win), vbox);

  //== Crearea barei de meniu
  menubar = gtk_menu_bar_new();
  // Crearea meniului "Fisier"
  file_menu = gtk_menu_new();
  file_item = gtk_menu_item_new_with_label("Fisier");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);
  //Crearea meniului Deschide
  //file_menu = gtk_menu_new();
  open_item = gtk_menu_item_new_with_label("Deschide");
  g_signal_connect(open_item, "activate", G_CALLBACK(on_open_activate), NULL);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu), open_item);


  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help_item);

  // Adăugarea barei de meniu la containerul vertical
  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

  button = gtk_button_new_from_stock (GTK_STOCK_DIALOG_INFO);
  g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (helloWorld), (gpointer) win);
  gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);

  button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
  g_signal_connect (button, "clicked", gtk_main_quit, NULL);
  gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);

  /* Enter the main loop */
  gtk_widget_show_all (win);
  gtk_main ();
  return 0;
}
