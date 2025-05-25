#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

static void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *scrolled_window;
    WebKitWebView *web_view;

    // Creați o nouă fereastră GTK
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Simplu Browser GTK");
    gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);

    // Creați un container GtkScrolledWindow pentru a permite derularea paginilor web
    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_container_add (GTK_CONTAINER (window), scrolled_window);

    // Creați un widget WebKitWebView
    web_view = WEBKIT_WEB_VIEW (webkit_web_view_new ());
    gtk_container_add (GTK_CONTAINER (scrolled_window), GTK_WIDGET (web_view));

    // Încărcați o pagină web
    webkit_web_view_load_uri (web_view, "https://www.google.com"); // Puteți schimba URL-ul aici

    // Afișați toate widget-urile
    gtk_widget_show_all (window);
}

int main (int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Inițializați aplicația GTK
    app = gtk_application_new ("org.gtk.example.SimpleBrowser", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
