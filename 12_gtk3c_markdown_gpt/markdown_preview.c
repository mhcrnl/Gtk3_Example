#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <cmark.h>

static GtkWidget *text_view;
static WebKitWebView *web_view;

static void update_preview(GtkTextBuffer *buffer, gpointer user_data) {
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gchar *markdown_text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    char *html = cmark_markdown_to_html(markdown_text, strlen(markdown_text), CMARK_OPT_DEFAULT);
    webkit_web_view_load_html(web_view, html, NULL);

    g_free(markdown_text);
    free(html);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *scrolled_window;
    GtkWidget *paned;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Editor Markdown cu Previzualizare");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(window), paned);

    // Editor Markdown
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    text_view = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    gtk_paned_pack1(GTK_PANED(paned), scrolled_window, TRUE, FALSE);

    // Previzualizare HTML
    web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());
    gtk_paned_pack2(GTK_PANED(paned), GTK_WIDGET(web_view), TRUE, FALSE);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    g_signal_connect(buffer, "changed", G_CALLBACK(update_preview), NULL);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.example.markdownpreview", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

