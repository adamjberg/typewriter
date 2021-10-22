#include <gtk/gtk.h>
#include <stdio.h>


static void
activate(GtkApplication *app,
         gpointer user_data)
{
  GtkWidget *window;

  window = gtk_application_window_new(app);
  // gtk_window_fullscreen(GTK_WINDOW(window));
  gtk_window_set_title(GTK_WINDOW(window), "engram typewriter");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

  GtkWidget *view;
  GtkTextBuffer *buffer;

  view = gtk_text_view_new();

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

  gtk_text_buffer_set_text(buffer, "Hello, this is some text", -1);

  gtk_container_add (GTK_WINDOW (window), view);

  gtk_widget_show_all(window);
}

int main(int argc,
         char **argv)
{
  GtkApplication *app;
  int status;

  FILE *fp;
  fp = fopen("2021-10-21.md", "w");
  fprintf(fp, "Hello World\n");
  fclose(fp);

  app = gtk_application_new("com.xyzdigital.engram.typewriter", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}