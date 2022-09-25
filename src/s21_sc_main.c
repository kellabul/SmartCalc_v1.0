#include <gtk/gtk.h>

GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *button1;
GtkWidget *label1;
GtkBuilder *builder;

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);

  builder = gtk_builder_new_from_file("GUI/s21_smartcal_gui.glade");

  window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_builder_connect_signals(builder, NULL);

  gtk_widget_show(window);

  gtk_main();

  return EXIT_SUCCESS;
}