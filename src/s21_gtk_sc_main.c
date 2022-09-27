#include <gtk/gtk.h>
#include <string.h>

#include "s21_smartcalc.h"

GtkEntry *entry_exp;
GtkEntry *entry_x;
GtkWidget *main_label_error;
GtkWidget *label_result;
/* has to be more than bigest button input length */
char input[S21_MAX_INPUT + 8] = {};
double x_value = S21_NAN;

int main(int argc, char *argv[]) {
  GtkWidget *window;
  GtkBuilder *builder;
  gtk_init(&argc, &argv);

  builder = gtk_builder_new_from_file("GUI/s21_smartcal_gui.glade");

  window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
  main_label_error =
      GTK_WIDGET(gtk_builder_get_object(builder, "main_label_error"));
  label_result = GTK_WIDGET(gtk_builder_get_object(builder, "label_result"));
  entry_exp = GTK_ENTRY(gtk_builder_get_object(builder, "entry_exp"));
  entry_x = GTK_ENTRY(gtk_builder_get_object(builder, "entry_x"));

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_builder_connect_signals(builder, NULL);

  gtk_widget_show(window);

  gtk_main();

  return EXIT_SUCCESS;
}

/* ============= BUTTONS WITH LOGIC ============= */

void button_ce_clicked_cb() {
  input[0] = '\0';
  gtk_entry_set_text(entry_exp, (const gchar *)input);
  gtk_label_set_text(GTK_LABEL(label_result), (const gchar *)"0");
}

void button_delete_clicked_cb() {
  int input_size = strlen(input);
  if (input_size > 0) {
    input[input_size - 1] = '\0';
    gtk_entry_set_text(entry_exp, (const gchar *)input);
  }
}

void button_rersult_clicked_cb() {
  char *output_string = NULL;
  s_tokens *infix = NULL;
  s_tokens *postfix = NULL;

  infix = calloc(S21_MAX_TKN, sizeof(s_tokens));

  if (input_conversion(input, infix) == S21_INCORRECT_INPUT) {
    gtk_label_set_text(GTK_LABEL(label_result),
                       (const gchar *)"INCORRECT INPUT");
  } else {
    postfix = calloc(S21_MAX_TKN, sizeof(s_tokens));
    output_string = calloc(S21_MAX_INPUT, sizeof(char));

    infix_to_postfix(infix, postfix);
    if (!(__builtin_isnan(x_value))) replaceX(postfix, x_value);
    calculation(postfix, output_string);
    gtk_label_set_text(GTK_LABEL(label_result), output_string);

    free(output_string);
    free(postfix);
  }
  free(infix);
}

/* ============= ENTRY ============= */

void entry_exp_changed_cb(GtkEntry *entry) {
  sprintf(input, "%s", gtk_entry_get_text(entry));
}

void entry_x_changed_cb(GtkEntry *entry) {
  char x_buffer[S21_MAX_INPUT + 1] = {};
  sprintf(x_buffer, "%s", gtk_entry_get_text(entry));
  if (x_buffer[0] == '\0')
    x_value = S21_NAN;
  else
    sscanf(x_buffer, "%lf", &x_value);
}

/* ============= REGULAR BUTTONS ============= */

void button_1_clicked_cb() {
  strcat(input, "1");
  // printf("%s\n", input);
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_2_clicked_cb() {
  strcat(input, "2");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_3_clicked_cb() {
  strcat(input, "3");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_4_clicked_cb() {
  strcat(input, "4");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_5_clicked_cb() {
  strcat(input, "5");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_6_clicked_cb() {
  strcat(input, "6");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_7_clicked_cb() {
  strcat(input, "7");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_8_clicked_cb() {
  strcat(input, "8");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_9_clicked_cb() {
  strcat(input, "9");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_dot_clicked_cb() {
  strcat(input, ".");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_zero_clicked_cb() {
  strcat(input, "0");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_x_clicked_cb() {
  strcat(input, "x");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_pow_clicked_cb() {
  strcat(input, "^");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_left_paren_clicked_cb() {
  strcat(input, "(");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_right_paren_clicked_cb() {
  strcat(input, ")");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_div_clicked_cb() {
  strcat(input, "/");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_mult_clicked_cb() {
  strcat(input, "*");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_minus_clicked_cb() {
  strcat(input, "-");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_plus_clicked_cb() {
  strcat(input, "+");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_ln_clicked_cb() {
  strcat(input, "ln(");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_mod_clicked_cb() {
  strcat(input, "mod");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_log_clicked_cb() {
  strcat(input, "log(");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_cos_clicked_cb() {
  strcat(input, "cos(");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_sin_clicked_cb() {
  strcat(input, "sin(");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_tan_clicked_cb() {
  strcat(input, "tan(");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_acos_clicked_cb() {
  strcat(input, "acos(");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_asin_clicked_cb() {
  strcat(input, "asin(");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_atan_clicked_cb() {
  strcat(input, "atan(");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void button_sqrt_clicked_cb() {
  strcat(input, "sqrt(");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}
