#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

static void activate(GtkStatusIcon* status_icon, gpointer user_data) {
g_debug("'activate' signal triggered");
}

static void destroy_event(GtkWidget *widget, gpointer data) {
gtk_main_quit();
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
gtk_main_quit();
return FALSE;
}

static void callback( GtkWidget *widget, gpointer data ) {
gtk_main_quit();
}

static void killed( GtkWidget *widget, GdkEvent *event, gpointer data ) {
execl("/usr/bin/killall", "killall", "watch", (char *)NULL);
gtk_main_quit();
}

//To compile append `pkg-config --cflags --libs gtk+-2.0`
int main(int argc, char *argv[]) {

GtkWidget *window;
GtkWidget *table;
GtkWidget *button;
GtkWidget *button2;
GtkWidget *frame;
GtkWidget *view;
GtkWidget *scrolledwindow;
GtkWidget *image;

GtkTooltips *tooltips;

GdkPixbuf *pixbuf;
GtkStatusIcon *status_icon;

GtkTextBuffer *buffer;
GtkTextIter iter;

gchar *array;
gchar *utf8;
GError *err = NULL;
gsize *bytes_read = NULL; 
gsize *bytes_written = NULL;
gsize length;

gtk_init(&argc, &argv);

g_file_get_contents("/home/amboxer21/Documents/netstat2.txt", &array, &length, NULL);

window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(window), "SECURITY WARNING");
gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
gtk_widget_show(window);

table = gtk_table_new(10, 10, TRUE);
gtk_container_add(GTK_CONTAINER(window), table);
gtk_widget_show(table);

view = gtk_text_view_new ();
//gtk_table_attach_defaults(GTK_TABLE(table), view, 1, 9, 2, 5);
buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));      

utf8 = g_convert(array, length, "UTF-8", "us-ascii", bytes_read, bytes_written, &err);
     if (err != NULL) {
     printf("G_CONVERT(!NULL) error --> %s \n", strerror(errno));
     g_error_free(err);
     }

gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
gtk_text_buffer_insert(buffer, &iter, utf8, -1);
//gtk_text_buffer_set_text(buffer, utf8, -1);
gtk_widget_show(view);

scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
gtk_container_add(GTK_CONTAINER(scrolledwindow), view);
gtk_table_attach_defaults(GTK_TABLE(table), scrolledwindow, 1, 9, 2, 5);
gtk_widget_show(scrolledwindow);

frame = gtk_frame_new("PORTS------------------------------PROCESS");
gtk_table_attach_defaults(GTK_TABLE(table), frame, 1, 9, 1, 9);
gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
gtk_widget_show(frame);

button = gtk_button_new_with_label("CONFIRM");
gtk_table_attach_defaults(GTK_TABLE(table), button, 2, 5, 6, 8);
gtk_widget_show(button);

tooltips = gtk_tooltips_new();
gtk_tooltips_set_tip(tooltips, button, "Click to confirm that you understand that there are suspicious ports open on your computer.", NULL);

button2 = gtk_button_new_with_label("KILL");
gtk_table_attach_defaults(GTK_TABLE(table), button2, 5, 8, 6, 8);
gtk_widget_show(button2);

tooltips = gtk_tooltips_new();
gtk_tooltips_set_tip(tooltips, button2, "Click to stop this program entirely.", NULL);

  pixbuf = gdk_pixbuf_new_from_file("/home/amboxer21/Pictures/warning_icon.png", NULL);
  image = gtk_image_new_from_pixbuf(pixbuf);
  //gtk_table_attach_defaults(GTK_TABLE(table), image, 1, 3, 1, 3);
  gtk_widget_show(image);

  status_icon = gtk_status_icon_new_from_pixbuf(pixbuf);
  gtk_status_icon_set_tooltip_text(GTK_STATUS_ICON(status_icon), "Suspicious ports are open.");
  gtk_status_icon_set_visible(status_icon, TRUE);
  //gtk_widget_show(status_icon);
  g_debug("embedded: %s", gtk_status_icon_is_embedded(status_icon) ? "yes" : "no");

  g_signal_connect_swapped(G_OBJECT(window), "destroy-event",
      G_CALLBACK(destroy_event), NULL);

  g_signal_connect_swapped(G_OBJECT(window), "delete-event",
      G_CALLBACK(delete_event), NULL);

  g_signal_connect(button, "clicked",
      G_CALLBACK (callback), (gpointer) "button");

  g_signal_connect_swapped(button2, "clicked",
      G_CALLBACK(killed), (gpointer)"killed");
  g_signal_connect_swapped(status_icon, "activate",
      G_CALLBACK(activate), (gpointer)"activate");

gtk_main_iteration();
gtk_main();

return 0;
}

