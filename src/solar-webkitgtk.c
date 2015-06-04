#include <stdio.h>
#include <JavaScriptCore/JSStringRef.h>
#include <JavaScriptCore/JSValueRef.h>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* web_view, GtkWidget* window);

static void get_javascript_result(GObject *object, GAsyncResult *result, gpointer user_data) {
  g_printf("Here? Surely?\n");
  WebKitJavascriptResult *js_result;
  JSValueRef              value;
  JSGlobalContextRef      context;
  GError                 *error = NULL;

  js_result = webkit_web_view_run_javascript_finish (WEBKIT_WEB_VIEW (object), result, &error);
  if (!js_result) {
    g_warning ("Error running javascript: %s", error->message);
    g_error_free (error);
    return;
  }

  g_printf("I'm here innit 1\n");
  context = webkit_javascript_result_get_global_context (js_result);
  g_printf("I'm here innit 2\n");
  value = webkit_javascript_result_get_value (js_result);
  int value_type = JSValueGetType(context, value);
  switch (value_type) {
    case kJSTypeUndefined:
      g_printf("Value undefined!\n");
      break;
    case kJSTypeNull:
      g_printf("Value null!\n");
      break;
    case kJSTypeBoolean:
      g_printf("Value boolean!\n");
      break;
    case kJSTypeNumber:
      g_printf("Value number!\n");
      break;
    case kJSTypeString:
      g_printf("Value string!\n");
      break;
    case kJSTypeObject:
      g_printf("Value object!\n");
      break;
  }

  if (JSValueIsString (context, value)) {
    g_printf("I'm here innit 3\n");
    JSStringRef js_str_value = JSValueToStringCopy(context, value, NULL);
    gsize       str_length = JSStringGetMaximumUTF8CStringSize(js_str_value);
    gchar      *str_value;

    g_printf("String length: %d\n", str_length);
    str_value = (gchar *)g_malloc (str_length);
    JSStringGetUTF8CString (js_str_value, str_value, str_length);
    JSStringRelease (js_str_value);

    g_print ("Script result: %s\n", str_value);
    g_free (str_value);
  } else {
    g_warning ("Error running javascript: unexpected return value");
  }
  webkit_javascript_result_unref (js_result);
}

static gchar* load_javascript(gchar *file_name) {
  FILE *script_file;
  gchar *script_value;

  script_file = fopen(file_name, "r");
  if (script_file != NULL) {
  } else {
    g_printf("File not found: %s\n", file_name);
  }

  return script_value;
}

static void run_some_javascript(WebKitWebView *web_view) {
  gchar script[2000];
  gchar *file_name = "script/request_capture.js";
  FILE *script_file = fopen(file_name, "r");
  int res;

  if (script_file != NULL) {
    res = fread(script, 1, 2000, script_file);
    g_printf("Read %u bytes.\n", res);
    fclose(script_file);

  } else {
    g_printf("File not found: %s\n", file_name);
  }

  webkit_web_view_run_javascript(web_view, script, NULL, get_javascript_result, NULL);
}

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
      run_some_javascript(web_view);
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
