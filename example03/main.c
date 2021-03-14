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
  
  gtk_widget_show(window);
  
  g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);  
      
  g_object_unref(icon);

  gtk_main();

  return 0;
}
