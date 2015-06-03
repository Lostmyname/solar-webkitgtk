#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* web_view, GtkWidget* window);

static void
load_status_cb (WebKitWebView *web_view, WebKitLoadEvent load_event, gpointer user_data) {
  switch (load_event) {
    case WEBKIT_LOAD_STARTED:
      g_printf("Started load\n");
      break;
    case WEBKIT_LOAD_REDIRECTED:
      g_printf("Load redirected\n");
      break;
    case WEBKIT_LOAD_COMMITTED:
      g_printf("Load committed\n");
      break;
    case WEBKIT_LOAD_FINISHED:
      g_printf("Load finished!\n");
  }
}



int main(int argc, char* argv[])
{
    // Initialize GTK+
    gtk_init(&argc, &argv);

    if (argc < 2) {
      printf("Usage: solar-webkitgtk <uri>\n");
      return 1;
    }
    const gchar *uri = argv[1];

    // Create an 800x600 window that will contain the browser instance
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);

    // Create a browser instance
    WebKitWebView *web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());

    // Put the browser area into the main window
    gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(web_view));

    g_object_set(webkit_web_view_get_settings(web_view), "enable-webgl", TRUE, NULL);

    // Set up callbacks so that if either the main window or the browser instance is
    // closed, the program will exit
    g_signal_connect(main_window, "destroy", G_CALLBACK(destroyWindowCb), NULL);
    g_signal_connect(web_view, "close", G_CALLBACK(closeWebViewCb), main_window);
    g_signal_connect(web_view, "load-changed", G_CALLBACK(load_status_cb), NULL);

    // Load a web page into the browser instance
    webkit_web_view_load_uri(web_view, uri);

    // Make sure that when the browser area becomes visible, it will get mouse
    // and keyboard events
    gtk_widget_grab_focus(GTK_WIDGET(web_view));

    // Make sure the main window and all its contents are visible
    gtk_widget_show_all(main_window);

    // Run the main GTK+ event loop
    gtk_main();

    return 0;
}


static void destroyWindowCb(GtkWidget* widget, GtkWidget* window)
{
    gtk_main_quit();
}

static gboolean closeWebViewCb(WebKitWebView* web_view, GtkWidget* window)
{
    gtk_widget_destroy(window);
    return TRUE;
}
