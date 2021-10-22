#include <gtk/gtk.h>
#include <stdio.h>
#include <time.h>

gboolean dirty = FALSE;

static gboolean
handleDirtyTimer(gpointer data)
{
  if (!dirty) {
    return;
  }

  GtkTextBuffer *textBuffer = (GtkTextBuffer *)data;

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  char filename[256];
  memset(&filename, 0x0, 256);
  snprintf(filename, 255, "%d-%02d-%02d.md", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

  GtkTextIter start = {
      0,
  };
  GtkTextIter end = {
      0,
  };
  gtk_text_buffer_get_start_iter(textBuffer, &start);
  gtk_text_buffer_get_end_iter(textBuffer, &end);
  gchar *text;
  text = gtk_text_buffer_get_text(textBuffer, &start, &end, FALSE);

  FILE *fp;
  fp = fopen(filename, "w");
  fprintf(fp, "%s", text);
  fclose(fp);

  g_free(text);

  dirty = FALSE;

  return TRUE;
}

void handleTextChanged() {
  dirty = TRUE;
}

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

  g_signal_connect(buffer, "changed", G_CALLBACK(handleTextChanged), NULL);

  gtk_text_buffer_set_text(buffer, "", -1);

  gtk_container_add(GTK_WINDOW(window), view);

  g_timeout_add_seconds(3, handleDirtyTimer, buffer);

  gtk_widget_show_all(window);
}

int main(int argc,
         char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new("com.xyzdigital.engram.typewriter", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  gtk_main_quit();

  return status;
}