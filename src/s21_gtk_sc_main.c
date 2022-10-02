#include <cairo.h>
#include <gtk/gtk.h>
#include <locale.h>
#include <string.h>

#include "s21_smartcalc.h"

/* main part */
GtkEntry *entry_exp;
GtkEntry *entry_x;
GtkWidget *label_result;
/* has to be more than bigest button input length */
char input[S21_MAX_INPUT + 8] = {};
double x_value = S21_NAN;

/* bonus 1*/
void calculate_credit();
GtkWidget *b1_label_error;
GtkWidget *b1_label_monthly_payment;
GtkWidget *b1_label_total_payment;
GtkWidget *b1_label_overpay_on_credit;
double b1_loan = S21_NAN;
double b1_interestRate = S21_NAN;
int b1_term = -1;
int b1_type = 1;


int main(int argc, char *argv[]) {
  GtkWidget *window;
  GtkBuilder *builder;
  gtk_init(&argc, &argv);

  builder = gtk_builder_new_from_file("GUI/s21_smartcalc_main_gui.glade");
  window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit),
                   NULL);
  gtk_builder_connect_signals(builder, NULL);

  label_result = GTK_WIDGET(gtk_builder_get_object(builder, "label_result"));
  entry_exp = GTK_ENTRY(gtk_builder_get_object(builder, "entry_exp"));
  entry_x = GTK_ENTRY(gtk_builder_get_object(builder, "entry_x"));

  /* bonus 1 */
  b1_label_error =
      GTK_WIDGET(gtk_builder_get_object(builder, "bonus1_label_error"));
  b1_label_monthly_payment = GTK_WIDGET(
      gtk_builder_get_object(builder, "bonus1_label_monthly_payment"));
  b1_label_total_payment =
      GTK_WIDGET(gtk_builder_get_object(builder, "bonus1_label_total_payment"));
  b1_label_overpay_on_credit = GTK_WIDGET(
      gtk_builder_get_object(builder, "bonus1_label_overpay_on_credit"));
  /* bonus 1 */

  g_object_unref(builder);

  gtk_widget_show(window);

  gtk_main();

  return EXIT_SUCCESS;
}


/* ============= ENTRY ============= */

void entry_exp_changed_cb(GtkEntry *entry) {
  sprintf(input, "%s", gtk_entry_get_text(entry));
}

void entry_x_changed_cb(GtkEntry *entry) {
  char x_buffer[S21_MAX_INPUT + 1] = {};
  sprintf(x_buffer, "%s", gtk_entry_get_text(entry));
  if (x_buffer[0] == '\0') {
    x_value = S21_NAN;
  } else if (isNotNumberInString(x_buffer)) {
    x_value = S21_INF;
  } else {
    sscanf(x_buffer, "%lf", &x_value);
  }
}

/* ============= BUTTONS WITH LOGIC ============= */

void button_ce_clicked_cb() {
  input[0] = '\0';
  gtk_entry_set_text(entry_exp, (const gchar *)input);
  gtk_label_set_text(GTK_LABEL(label_result), (const gchar *)"0.00");
}

void button_clear_x_clicked_cb() {
  gtk_entry_set_text(entry_x, (const gchar *)"");
}

void button_delete_clicked_cb() {
  int input_size = strlen(input);
  if (input_size > 0) {
    input[input_size - 1] = '\0';
    gtk_entry_set_text(entry_exp, (const gchar *)input);
  }
}

void button_rersult_clicked_cb() {
  setlocale(LC_NUMERIC, "C");
  double *x = NULL;
  if (!s21_isnan(x_value)) x = &x_value;
  char *output_string = calloc(S21_MAX_INPUT, sizeof(char));
  if (output_string != NULL) {
    calculation(input, x, output_string);
    gtk_label_set_text(GTK_LABEL(label_result), output_string);
  }
  free(output_string);
}

void button_draw_graph_clicked_cb() {
  if (input_validation(input) == S21_INCORRECT_INPUT) {
    gtk_label_set_text(GTK_LABEL(label_result),
                       (const gchar *)"INCORRECT INPUT");
  } else {
    graph_output(input);
  }
}

void regular_button_clicked(GtkButton *button) {
  const gchar *text = gtk_button_get_label(button);
  strcat(input, text);
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

void function_button_clicked(GtkButton *button) {
  const gchar *text = gtk_button_get_label(button);
  strcat(input, text);
  strcat(input, "(");
  gtk_entry_set_text(entry_exp, (const gchar *)input);
}

/* ============= PART 2 BONUS ============= */

void bonus1_button_clicked_cb() {
  char error_string[128] = {};
  setlocale(LC_NUMERIC, "C");

  gtk_label_set_text(GTK_LABEL(b1_label_total_payment), "");
  gtk_label_set_text(GTK_LABEL(b1_label_monthly_payment), "");
  gtk_label_set_text(GTK_LABEL(b1_label_overpay_on_credit), "");

  if (s21_isnan(b1_loan)) {
    sprintf(error_string, "ENTER TOTAL CREDIT AMOUNT");
  } else if (b1_term == -1) {
    sprintf(error_string, "ENTER TERM");
  } else if (s21_isnan(b1_interestRate)) {
    sprintf(error_string, "ENTER INTEREST RATE");
  } else if (b1_loan < 0.01) {
    sprintf(error_string, "LOAN VALUE IS TOO LOW");
  } else if (b1_term < 1) {
    sprintf(error_string, "CREDIT PERIOD VALUE IS TOO LOW");
  } else if ((int)b1_term - b1_term > 0) {
    sprintf(error_string, "CREDIT PERIOD VALUE MUST BE INTEGER");
  } else if (b1_interestRate < 0.1) {
    sprintf(error_string, "INTEREST VALUE IS TOO LOW");
  } else {
    calculate_credit();
  }
  gtk_label_set_text(GTK_LABEL(b1_label_error), error_string);
}

void calculate_credit() {
  double total_payment_output = 0;
  double first_payment = 0;
  double last_payment = 0;
  char string_buffer[128] = {};
  total_payment_output = total_payment(b1_loan, b1_interestRate, b1_term,
                                       b1_type, &first_payment, &last_payment);
  sprintf(string_buffer, "%.2lf", total_payment_output);
  gtk_label_set_text(GTK_LABEL(b1_label_total_payment), string_buffer);
  if (first_payment == last_payment) {
    sprintf(string_buffer, "%.2lf", first_payment);
    gtk_label_set_text(GTK_LABEL(b1_label_monthly_payment), string_buffer);
  } else {
    sprintf(string_buffer, "%.2lf ... %.2lf", first_payment, last_payment);
    gtk_label_set_text(GTK_LABEL(b1_label_monthly_payment), string_buffer);
  }
  sprintf(string_buffer, "%.2lf", total_payment_output - b1_loan);
  gtk_label_set_text(GTK_LABEL(b1_label_overpay_on_credit), string_buffer);
}

void bonus1_entry_total_credit_changed_cb(GtkEntry *entry) {
  char x_buffer[S21_MAX_INPUT + 1] = {};
  sprintf(x_buffer, "%s", gtk_entry_get_text(entry));
  if (x_buffer[0] == '\0')
    b1_loan = S21_NAN;
  else
    sscanf(x_buffer, "%lf", &b1_loan);
}

void bonus1_entry_term_changed_cb(GtkEntry *entry) {
  char x_buffer[S21_MAX_INPUT + 1] = {};
  sprintf(x_buffer, "%s", gtk_entry_get_text(entry));
  if (x_buffer[0] == '\0')
    b1_term = -1;
  else
    sscanf(x_buffer, "%d", &b1_term);
}

void bonus1_entry_interest_changed_cb(GtkEntry *entry) {
  char x_buffer[S21_MAX_INPUT + 1] = {};
  sprintf(x_buffer, "%s", gtk_entry_get_text(entry));
  if (x_buffer[0] == '\0')
    b1_interestRate = S21_NAN;
  else
    sscanf(x_buffer, "%lf", &b1_interestRate);
}
void on_bonus1_differentiated_toggled(GtkRadioButton *button) {
  b1_type = S21_DIFFERENTIATED;
}

void on_bonus1_anuity_toggled(GtkRadioButton *button) {
  b1_type = S21_ANNUITANTS;
}

/* ============= PART 2 BONUS ============= */

void bonus2_entry_capitalization_of_interest_changed_cb() {}

void bonus2_entry_periodicity_of_payments_changed_cb() {}

void bonus2_entry_tax_rate_changed_cb() {}

void bonus2_entry_interest_rate_changed_cb() {}

void bonus2_entry_deposit_term_changed_cb() {}

void bonus2_entry_deposit_amount_changed_cb() {}

void bonus2_button_clicked_cb() {}