#include <cairo.h>
#include <pango/pangocairo.h>

#include "s21_smartcalc.h"
#include "s21_smartcalc_gtk.h"

#define DA_HEIGHT 600
#define DA_WIDTH 600
#define ROTATE 1
#define DONT_ROTATE 0

char *expression;
GtkWidget *drawing_area;
GtkWidget *graph_error_label;
GtkWidget *domain_max_spin;
GtkWidget *codomain_max_spin;
GtkWidget *domain_min_spin;
GtkWidget *codomain_min_spin;

int graph_output(char *input) {
  GtkBuilder *builder;
  GtkWidget *close_button;
  GtkWidget *draw_button;
  GtkEntry *graph_entry;
  GtkWidget *graph_window;

  expression = input;

  builder = gtk_builder_new_from_file("GUI/s21_smartcalc_graph_gui.glade");
  gtk_builder_connect_signals(builder, NULL);

  graph_window = GTK_WIDGET(gtk_builder_get_object(builder, "graph_window"));
  drawing_area = GTK_WIDGET(gtk_builder_get_object(builder, "graph_da"));
  graph_entry = GTK_ENTRY(gtk_builder_get_object(builder, "graph_entry"));
  close_button = GTK_WIDGET(gtk_builder_get_object(builder, "graph_close"));
  draw_button =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_draw_button"));
  graph_error_label =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_error_label"));

  domain_max_spin =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_spin_domain"));
  codomain_max_spin =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_spin_codomain"));
  domain_min_spin =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_spin_domain"));
  codomain_min_spin =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_spin_codomain"));

  gtk_entry_set_text(graph_entry, (const gchar *)input);
  gtk_widget_set_size_request(drawing_area, DA_HEIGHT,
                              DA_WIDTH);  // size in pixels

  g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw), NULL);

  g_signal_connect(G_OBJECT(draw_button), "clicked",
                   G_CALLBACK(button_draw_clicked), G_OBJECT(graph_entry));

  g_signal_connect(G_OBJECT(close_button), "clicked", G_CALLBACK(close_window),
                   G_OBJECT(graph_window));

  gtk_window_set_position(GTK_WINDOW(graph_window), GTK_WIN_POS_CENTER);
  gtk_widget_show_all(graph_window);

  return 0;
}

static gboolean on_draw(GtkWidget *widget, cairo_t *cairo) {
  s_graph_properties gp = {};
  gp.cr = cairo;

  /* Draw on a background */
  cairo_set_source_rgb(gp.cr, 0.41, 0.78, 0.78);
  cairo_paint(gp.cr);

  cairo_device_to_user_distance(gp.cr, &gp.dx, &gp.dy);

  gp.max_x = 400;
  gp.max_y = 300;
  //if (gp.max_y > gp.max_x) gp.max_y = gp.max_x;
  gp.min_x = -100;
  gp.min_y = -200;

  gp.dx = (gp.max_x - gp.min_x) / DA_HEIGHT;
  gp.dy = (gp.max_x - gp.min_x) / DA_WIDTH; /* Pixels between each point */

  //printf("dx = %lf, dy = %lf\n", gp.dx, gp.dy);

  double x_middle = (fabs(gp.min_x)/(gp.max_x + fabs(gp.min_x))) * 600;
  double y_middle = (fabs(gp.min_y)/(gp.max_y + fabs(gp.min_y))) * 600;

  cairo_translate(gp.cr, x_middle, y_middle);

  cairo_scale(gp.cr, 1 / gp.dx, 1 / gp.dy);

  draw_axis(&gp);

  if (input_validation(expression) == S21_CORRECT_INPUT) {
    char buffer[64];
    sprintf(buffer, "scale px/un: ");
    gtk_label_set_text(GTK_LABEL(graph_error_label), buffer);
    draw_graph_line(&gp);
  } else {
    gtk_label_set_text(GTK_LABEL(graph_error_label),
                       (const gchar *)"INCORRECT INPUT");
  }
  return FALSE;
}

void draw_graph_line(s_graph_properties *gp) {
  setlocale(LC_NUMERIC, "C");
  int vector = -1;  // 1 or -1
  cairo_set_line_width(gp->cr, gp->dx);
  gdouble step = gp->dx / 10;
  for (gdouble x = gp->min_x; x < gp->max_x; x += step) {
    gdouble y_value = calculation(expression, &x, NULL);
    if (y_value > gp->max_y || y_value < gp->min_y) {
      if ((y_value > gp->max_y)) {
        cairo_move_to(gp->cr, x, vector * gp->max_y);
      } else if ((y_value < gp->min_y)) {
        cairo_move_to(gp->cr, x, vector * gp->min_y);
      }
    } else if (!isnan(y_value) && !isinf(y_value)) {
      cairo_line_to(gp->cr, x, vector * y_value);
    }
  }
  /* Draw the curve */
  cairo_set_source_rgba(gp->cr, 0.72, 0.0, 1, 1);
  cairo_stroke(gp->cr);
}

void roundTo2digits(gdouble *step) {
  char buffer[64];
  sprintf(buffer, "%lf", *step);
  for (int i = 1; i < strlen(buffer); i++) {
    if (buffer[i] != '.') buffer[i] = '0';
  }
  sscanf(buffer, "%lf", step);
}

gdouble set_axis_step(s_graph_properties *gp) {
  gdouble step = 20 * gp->dx;
  for (gdouble i = 0.1; i <= 1; i += 0.1) {
    if (step < i) {
      step = i;
      break;
    }
  }
  if (step > 10)
    roundTo2digits(&step);
  else if (step > 5)
    step = 10;
  else if (step > 2)
    step = 5;
  else if (step > 1)
    step = 2;
  return step;
}

void draw_axys_text(s_graph_properties *gp, gdouble value, char rotate) {
  char buffer[64];
  sprintf(buffer, "%g", value);
  if (rotate) {
    cairo_save(gp->cr);
    cairo_rotate(gp->cr, -1);
    cairo_show_text(gp->cr, buffer);
    cairo_restore(gp->cr);
  } else {
    cairo_show_text(gp->cr, buffer);
  }
}

void draw_axis(s_graph_properties *gp) {
  char buffer[64];
  gdouble step = set_axis_step(gp);
  gdouble middleX = 0;
  gdouble middleY = 0;
  int flag = 1;
  gdouble text_offset = 2 * gp->dx;

  cairo_set_source_rgb(gp->cr, 0.0, 0.0, 0.0);
  cairo_set_line_width(gp->cr, gp->dx / 10);

  cairo_select_font_face(gp->cr, "Arial", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(gp->cr, 10 * gp->dx);

  for (gdouble i = middleX; i < gp->max_x; i += step) {
    cairo_move_to(gp->cr, i, gp->min_x);
    cairo_line_to(gp->cr, i, gp->max_x - text_offset);
    draw_axys_text(gp, i, ROTATE);
  }
  for (gdouble i = middleX; i > gp->min_x; i -= step) {
    cairo_move_to(gp->cr, i, gp->min_x);
    cairo_line_to(gp->cr, i, gp->max_x);
    cairo_move_to(gp->cr, i, gp->max_x - text_offset);
    draw_axys_text(gp, i, ROTATE);
  }
  for (gdouble i = middleY; i < gp->max_y; i += step) {
    cairo_move_to(gp->cr, gp->min_x, i);
    cairo_line_to(gp->cr, gp->max_x, i);
    cairo_move_to(gp->cr, gp->min_x, i);
    draw_axys_text(gp, -i, DONT_ROTATE);
  }
  for (gdouble i = middleY; i > gp->min_y; i -= step) {
    cairo_move_to(gp->cr, gp->min_x, i);
    cairo_line_to(gp->cr, gp->max_x, i);
    cairo_move_to(gp->cr, gp->min_x, i);
    draw_axys_text(gp, -i, DONT_ROTATE);
  }

  cairo_stroke(gp->cr);

  cairo_set_line_width(gp->cr, gp->dx / 2);
  cairo_move_to(gp->cr, gp->min_x, middleY);
  cairo_line_to(gp->cr, gp->max_x, middleY);
  cairo_move_to(gp->cr, middleX, gp->min_y);
  cairo_line_to(gp->cr, middleX, gp->max_y);

  cairo_set_font_size(gp->cr, 15 * gp->dx);
  cairo_move_to(gp->cr, gp->max_x - 15 * gp->dx, middleY + 15 * gp->dx);
  sprintf(buffer, "X");
  cairo_show_text(gp->cr, buffer);
  cairo_move_to(gp->cr, middleX + 5 * gp->dx, gp->min_x + 15 * gp->dx);
  sprintf(buffer, "Y");
  cairo_show_text(gp->cr, buffer);

  cairo_stroke(gp->cr);
}

void button_draw_clicked(GtkWidget *button, gpointer entry) {
  sprintf(expression, "%s", gtk_entry_get_text(entry));
  gtk_widget_queue_draw(drawing_area);
}

void close_window(GtkWidget *widget, gpointer window) {
  gtk_widget_destroy(GTK_WIDGET(window));
}

// void draw_graph_line(s_graph_properties gp) {
//   int flag = 0;
//   setlocale(LC_NUMERIC, "C");
//   for (gdouble x = gp.min_x; x < gp.max_x; x += gp.dx / 10) {
//     gdouble y_value = calculation(expression, &x, NULL);
//     if (isnan(y_value) || isinf(y_value) || y_value > gp.max_y ||
//         y_value < gp.min_y) {
//       if ((y_value > gp.max_y) && flag) {
//         cairo_line_to(gp.cr, x, gp.max_y);
//       } else if ((y_value < gp.min_y) && flag) {
//         cairo_line_to(gp.cr, x, gp.max_y);
//       } else if ((y_value > gp.max_y)) {
//         cairo_move_to(gp.cr, x, gp.max_y);
//       } else if ((y_value < gp.min_y)) {
//         cairo_move_to(gp.cr, x, gp.min_y);
//       }
//       flag = 0;
//     } else {
//       flag = 1;
//       cairo_line_to(gp.cr, x, y_value);
//     }
//   }
// cairo_set_source_rgba(gp.cr, 0.72, 0.0, 1, 1);
// cairo_stroke(gp.cr);
// }
