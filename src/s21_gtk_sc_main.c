#include <gtk/gtk.h>
#include <string.h>

#include "s21_smartcalc.h"


GtkEntry *entry_exp;
GtkWidget *main_label_error;
char input[S21_MAX_INPUT] = {};
int input_index = 0;

int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkBuilder *builder;
  gtk_init(&argc, &argv);

  builder = gtk_builder_new_from_file("GUI/s21_smartcal_gui.glade");

  window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
  main_label_error = GTK_WIDGET(gtk_builder_get_object(builder, "main_label_error"));
  entry_exp = GTK_ENTRY(gtk_builder_get_object(builder, "entry_exp"));
  
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_builder_connect_signals(builder, NULL);

  gtk_widget_show(window);

  gtk_main();

  return EXIT_SUCCESS;
}

/* ============= BUTTONS ============= */ 

void button_1_clicked_cb() {
  input_index++;
  strcat(input, "1");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_delete_clicked_cb() {
  input_index++;
  input[strlen(input) - 1] = '\0';
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_log_clicked_cb() {
  input_index += 4;
  strcat(input, "log(");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}


/* ============= ENTRY ============= */ 


void entry_exp_changed_cb(GtkEntry *entry){
  const char *buffer = gtk_entry_get_text(entry);
  if (strlen(buffer) > S21_MAX_INPUT) {
    gtk_label_set_text (GTK_LABEL(main_label_error), (const gchar* ) "TOO MUCH ARGUMENTS");
  } else {
    sprintf(input, "%s", buffer);
    gtk_label_set_text (GTK_LABEL(main_label_error), (const gchar* ) "");
  }
}
