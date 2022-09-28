#include <cairo.h>
#include <gtk/gtk.h>
#include <locale.h>
#include <string.h>

#include "s21_smartcalc.h"

/* graph */
#define WIDTH 640
#define HEIGHT 480
#define ZOOM_X 100.0
#define ZOOM_Y 100.0

GtkWidget *graph_window;
gfloat f(gfloat x);
static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);

/* main part */
GtkEntry *entry_exp;
GtkEntry *entry_x;
GtkWidget *label_result;
/* has to be more than bigest button input length */
char input[S21_MAX_INPUT + 8] = {};
double x_value = S21_NAN;

/* bonus 1*/
void calculate_credit();
GtkEntry *b1_entry_total_credit;
GtkEntry *b1_entry_term;
GtkEntry *b1_entry_interest;
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

  builder = gtk_builder_new_from_file("GUI/s21_smartcal_gui.glade");

  window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
  label_result = GTK_WIDGET(gtk_builder_get_object(builder, "label_result"));
  entry_exp = GTK_ENTRY(gtk_builder_get_object(builder, "entry_exp"));
  entry_x = GTK_ENTRY(gtk_builder_get_object(builder, "entry_x"));

  /* bonus 1 */
  b1_entry_total_credit =
      GTK_ENTRY(gtk_builder_get_object(builder, "bonus1_entry_total_credit"));
  b1_entry_term =
      GTK_ENTRY(gtk_builder_get_object(builder, "bonus1_entry_term"));
  b1_entry_interest =
      GTK_ENTRY(gtk_builder_get_object(builder, "bonus1_entry_interest"));
  b1_label_error =
      GTK_WIDGET(gtk_builder_get_object(builder, "bonus1_label_error"));
  b1_label_monthly_payment = GTK_WIDGET(
      gtk_builder_get_object(builder, "bonus1_label_monthly_payment"));
  b1_label_total_payment =
      GTK_WIDGET(gtk_builder_get_object(builder, "bonus1_label_total_payment"));
  b1_label_overpay_on_credit = GTK_WIDGET(
      gtk_builder_get_object(builder, "bonus1_label_overpay_on_credit"));
  /* bonus 1 */

  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit),
                   NULL);

  gtk_builder_connect_signals(builder, NULL);

  /*  graph */
  GtkWidget *da;
  // gtk_init(&argc, &argv);
  graph_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(graph_window), WIDTH, HEIGHT);
  gtk_window_set_title(GTK_WINDOW(graph_window), "Graph drawing");
  g_signal_connect(G_OBJECT(graph_window), "destroy", gtk_main_quit, NULL);
  da = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(graph_window), da);
  g_signal_connect(G_OBJECT(da), "draw", G_CALLBACK(on_draw), NULL);
  /*  graph */

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
  if (x_buffer[0] == '\0')
    x_value = S21_NAN;
  else
    sscanf(x_buffer, "%lf", &x_value);
}

/* ============= BUTTONS WITH LOGIC ============= */

void button_ce_clicked_cb() {
  input[0] = '\0';
  gtk_entry_set_text(entry_exp, (const gchar *)input);
  gtk_label_set_text(GTK_LABEL(label_result), (const gchar *)"0");
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
    calculation(input, x,  output_string);
    gtk_label_set_text(GTK_LABEL(label_result), output_string);
  }
  free (output_string);
}

/* ============= REGULAR BUTTONS ============= */

void button_1_clicked_cb() {
  strcat(input, "1");
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

/*  DRAWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW*/

void button_draw_graph_clicked_cb() { gtk_widget_show_all(graph_window); }

static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
  GdkRectangle da;            /* GtkDrawingArea size */
  gdouble dx = 2.0, dy = 2.0; /* Pixels between each point */
  gdouble i, clip_x1 = 0.0, clip_y1 = 0.0, clip_x2 = 0.0, clip_y2 = 0.0;
  GdkWindow *window = gtk_widget_get_window(widget);

  /* Determine GtkDrawingArea dimensions */
  gdk_window_get_geometry(window, &da.x, &da.y, &da.width, &da.height);

  /* Draw on a black background */
  cairo_set_source_rgb(cr, 0.51, 0.78, 0.78);
  cairo_paint(cr);

  /* Change the transformation matrix */
  cairo_translate(cr, da.width / 2, da.height / 2);
  cairo_scale(cr, ZOOM_X, -ZOOM_Y);

  /* Determine the data points to calculate (ie. those in the clipping zone */
  cairo_device_to_user_distance(cr, &dx, &dy);
  cairo_clip_extents(cr, &clip_x1, &clip_y1, &clip_x2, &clip_y2);
  cairo_set_line_width(cr, dx);

  /* Draws x and y axis */
  cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
  cairo_move_to(cr, clip_x1, 0.0);
  cairo_line_to(cr, clip_x2, 0.0);
  cairo_move_to(cr, 0.0, clip_y1);
  cairo_line_to(cr, 0.0, clip_y2);
  cairo_stroke(cr);

  /* Link each data point */
  for (i = clip_x1; i < clip_x2; i += dx / 5) {
    gdouble x_value = (gdouble) calculation(input, &i, NULL);
    cairo_move_to(cr, i, x_value);
    cairo_line_to(cr, i + dx / 5, x_value + dy / 5);
  }

  /* Draw the curve */
  cairo_set_source_rgba(cr, 0.72, 0.0, 1, 1);
  cairo_stroke(cr);

  return FALSE;
}
