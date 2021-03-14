#include <gtk/gtk.h>
/*
 * The application icon
 */
GdkPixbuf *create_pixbuf(const gchar * filename){
	
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	
	if(!pixbuf) {
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}
	return pixbuf;
}

int main(int argc, char *argv[]) {

  GtkWidget *window;
  GdkPixbuf *icon;
  GtkWidget *vbox;
  
  GtkWidget *text_view;
  
  GtkWidget *menubar;
  GtkWidget *fileMenu;
  GtkWidget *fileMi;
  GtkWidget *quitMi;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  /*
   * Center the window
   */
  gtk_window_set_title(GTK_WINDOW(window), "Center");
  gtk_window_set_default_size(GTK_WINDOW(window), 650, 450);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  
  // Icon
  icon = create_pixbuf("turtle.png");
  gtk_window_set_icon(GTK_WINDOW(window), icon);
  
  // Vbox
  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_container_add(GTK_CONTAINER(window), vbox);
  
  //Menubar
  menubar = gtk_menu_bar_new();
  fileMenu = gtk_menu_new();
  
  //GtkTextView https://developer.gnome.org/gtk3/stable/GtkTextView.html
  text_view = gtk_text_view_new();
  
  fileMi = gtk_menu_item_new_with_label("File");
  quitMi = gtk_menu_item_new_with_label("Quit");
  
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 5);
  
  
  gtk_widget_show_all(window);
  
  g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL); 
      
  g_signal_connect(G_OBJECT(quitMi), "activate", 
      G_CALLBACK(gtk_main_quit), NULL);  
      
  g_object_unref(icon);

  gtk_main();

  return 0;
}
