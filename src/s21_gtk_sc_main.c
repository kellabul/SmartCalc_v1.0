#include <string.h>

#include "s21_smartcalc.h"
#include "s21_smartcalc_gtk.h"

/* main part */
GtkEntry *entry_exp;
GtkEntry *entry_x;
GtkWidget *label_result;
/* has to be more than bigest button input length */
char input[S21_MAX_INPUT + 8] = {};
double x_value = NAN;

/* bonus 1*/
void calculate_credit();
GtkWidget *b1_label_monthly_payment;
GtkWidget *b1_label_total_payment;
GtkWidget *b1_label_overpay_on_credit;
long double b1_loan = 1000;
long double b1_interestRate = 10;
int b1_term = 1;
int b1_type = 1;

/* bonus 2*/
GtkWidget *b2_accured_interest;
GtkWidget *b2_tax_amount;
GtkWidget *b2_deposit_amount;
s_deposit deposit = {10000, 10, 10, 10, 1};

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
  b1_label_monthly_payment =
      GTK_WIDGET(gtk_builder_get_object(builder, "b1_label_monthly_payment"));
  b1_label_total_payment =
      GTK_WIDGET(gtk_builder_get_object(builder, "b1_label_total_payment"));
  b1_label_overpay_on_credit =
      GTK_WIDGET(gtk_builder_get_object(builder, "b1_label_overpay_on_credit"));

  /* bonus 2 */
  b2_accured_interest =
      GTK_WIDGET(gtk_builder_get_object(builder, "b2_label_accured_interest"));
  b2_tax_amount =
      GTK_WIDGET(gtk_builder_get_object(builder, "b2_label_tax_amount"));
  b2_deposit_amount =
      GTK_WIDGET(gtk_builder_get_object(builder, "b2_label_deposit_amount"));

  g_object_unref(builder);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  gtk_widget_show(window);

  gtk_main();

  return EXIT_SUCCESS;
}

void entry_exp_changed_cb(GtkEntry *entry) {
  sprintf(input, "%s", gtk_entry_get_text(entry));
}

void entry_x_changed_cb(GtkEntry *entry) {
  char x_buffer[S21_MAX_INPUT + 1] = {};
  sprintf(x_buffer, "%s", gtk_entry_get_text(entry));
  if (x_buffer[0] == '\0') {
    x_value = NAN;
  } else if (isNotNumberInString(x_buffer)) {
    x_value = INFINITY;
  } else {
    sscanf(x_buffer, "%lf", &x_value);
  }
}

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
  if (!isnan(x_value)) x = &x_value;
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

void b1_button_clicked_cb() {
  setlocale(LC_NUMERIC, "C");
  long double total_payment_output = 0;
  long double first_payment = 0;
  long double last_payment = 0;
  char string_buffer[64] = {};
  char output[64] = {};
  total_payment_output = total_payment(b1_loan, b1_interestRate, b1_term,
                                       b1_type, &first_payment, &last_payment);
  sprintf(string_buffer, "%.2Lf", total_payment_output);
  convertToFinancialOutputNumber(string_buffer, output);
  gtk_label_set_text(GTK_LABEL(b1_label_total_payment), output);
  if (first_payment == last_payment) {
    sprintf(string_buffer, "%.2Lf", first_payment);
    convertToFinancialOutputNumber(string_buffer, output);
    gtk_label_set_text(GTK_LABEL(b1_label_monthly_payment), output);
  } else {
    char buffer_2[64];
    sprintf(string_buffer, "%.2Lf", first_payment);
    convertToFinancialOutputNumber(string_buffer, output);
    strcat(output, " ... ");
    sprintf(string_buffer, "%.2Lf", first_payment);
    convertToFinancialOutputNumber(string_buffer, buffer_2);
    strcat(output, buffer_2);
    gtk_label_set_text(GTK_LABEL(b1_label_monthly_payment), output);
  }
  sprintf(string_buffer, "%.2Lf", total_payment_output - b1_loan);
  convertToFinancialOutputNumber(string_buffer, output);
  gtk_label_set_text(GTK_LABEL(b1_label_overpay_on_credit), output);
}

void b1_entry_total_credit_changed_cb(GtkEntry *entry) {
  char buffer[S21_MAX_INPUT + 1] = {};
  sprintf(buffer, "%s", gtk_entry_get_text(entry));
  if (buffer[0] == '\0') {
    b1_loan = NAN;
  } else {
    sscanf(buffer, "%Lf", &b1_loan);
  }
}

void b1_entry_term_changed_cb(GtkEntry *entry) {
  char buffer[S21_MAX_INPUT + 1] = {};
  sprintf(buffer, "%s", gtk_entry_get_text(entry));
  if (buffer[0] == '\0')
    b1_term = -1;
  else
    sscanf(buffer, "%d", &b1_term);
}

void b1_entry_interest_changed_cb(GtkEntry *entry) {
  char buffer[S21_MAX_INPUT + 1] = {};
  sprintf(buffer, "%s", gtk_entry_get_text(entry));
  if (buffer[0] == '\0')
    b1_interestRate = NAN;
  else
    sscanf(buffer, "%Lf", &b1_interestRate);
}
void on_b1_differentiated_toggled() { b1_type = S21_DIFFERENTIATED; }

void on_b1_anuity_toggled() { b1_type = S21_ANNUITANTS; }

/* ============= PART 3 BONUS ============= */

void b2_spin_deposit_amount_value_changed_cb(GtkSpinButton *button) {
  deposit.amount = gtk_spin_button_get_value(button);
}

void b2_spin_deposit_term_value_changed_cb(GtkSpinButton *button) {
  deposit.term = gtk_spin_button_get_value(button);
}

void b2_spin_interest_value_changed_cb(GtkSpinButton *button) {
  deposit.interest_rate = gtk_spin_button_get_value(button);
}

void b2_spin_tax_value_changed_cb(GtkSpinButton *button) {
  deposit.tax_rate = gtk_spin_button_get_value(button);
}

void b2_spin_period_payments_value_changed_cb(GtkSpinButton *button) {
  deposit.periodicity = gtk_spin_button_get_value(button);
}

void check_capitalization_toggled_cb(GtkToggleButton *button) {
  gboolean status = gtk_toggle_button_get_active(button);
  if (status)
    deposit.capitalization = 1;
  else
    deposit.capitalization = 0;
}

void b2_spin_replen_value_changed_cb(GtkSpinButton *button) {
  deposit.replenishment = gtk_spin_button_get_value(button);
}

void b2_spin_replen_periodicity_value_changed_cb(GtkSpinButton *button) {
  deposit.freq_of_replen = gtk_spin_button_get_value(button);
}

void b2_spin_withdraw_value_changed_cb(GtkSpinButton *button) {
  deposit.withdrawal = gtk_spin_button_get_value(button);
}

void b2_spin_withdraw_periodicity_value_changed_cb(GtkSpinButton *button) {
  deposit.freq_of_withd = gtk_spin_button_get_value(button);
}

void b2_button_clicked_cb(GtkSpinButton *button) {
  char output[64];
  deposit_calculation(&deposit);
  if (deposit.amount_by_end < 0) {
    gtk_label_set_text(GTK_LABEL(b2_accured_interest), "TOO MUCH WITHDRAWALS");
    gtk_label_set_text(GTK_LABEL(b2_tax_amount), "");
    gtk_label_set_text(GTK_LABEL(b2_deposit_amount), "");
  } else {
    printDepositOutput(deposit.accrued_interest, output);
    gtk_label_set_text(GTK_LABEL(b2_accured_interest), output);
    printDepositOutput(deposit.tax_amount, output);
    gtk_label_set_text(GTK_LABEL(b2_tax_amount), output);
    printDepositOutput(deposit.amount_by_end, output);
    gtk_label_set_text(GTK_LABEL(b2_deposit_amount), output);
  }
}

void printDepositOutput(long double value, char *output) {
  char buffer[64];
  if (isnan(value) || isinf(value)) {
    sprintf(output, "INPUT VALUES ARE TOO HIGH");
  } else if (value < 10e33) {
    sprintf(buffer, "%.2Lf", value);
    convertToFinancialOutputNumber(buffer, output);
  } else {
    sprintf(output, "%.2Le", value);
  }
}