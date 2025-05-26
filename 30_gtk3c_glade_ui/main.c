#include<gtk/gtk.h>

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GObject *window;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "ui.glade", NULL);

    window = gtk_builder_get_object(builder, "main_window");
    //gtk_window_set_title(GTK_WINDOW(window), "Hello World");
    //gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //gtk_widget_show_all(window);
    gtk_builder_connect_signals(builder, NULL);
    gtk_main();

    return 0;
}
