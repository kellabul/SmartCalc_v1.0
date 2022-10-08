#include <cairo.h>

#include "s21_smartcalc.h"
#include "s21_smartcalc_gtk.h"

void button_draw_clicked(GtkWidget *button, gpointer entry);
void set_values_from_spin_buttons(s_graph_properties *gp);

char *expression;
GtkWidget *drawing_area;
gdouble scale = 100.0;
GtkWidget *graph_error_label;

int write_dom_codom = 0;
GtkWidget *min_domain_spin;
GtkWidget *min_codomain_spin;
GtkWidget *max_domain_spin;
GtkWidget *max_codomain_spin;
GtkWidget *scale_label;

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

  min_domain_spin =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_spin_min_domain"));
  min_codomain_spin =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_spin_min_codomain"));
  max_domain_spin =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_spin_max_domain"));
  max_codomain_spin =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_spin_max_codomain"));

  graph_error_label =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_error_label"));
  scale_label = GTK_WIDGET(gtk_builder_get_object(builder, "label_scale"));

  gtk_entry_set_text(graph_entry, (const gchar *)input);
  gtk_widget_set_size_request(drawing_area, 600, 600);  // size in pixels

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
  int scale_x = scale;
  int scale_y = -scale;
  // GdkRectangle da; /* GtkDrawingArea size */
  s_graph_properties gp = {};
  gp.dx = 1.0;
  gp.dy = 1.0; /* Pixels between each point */
  gp.cr = cairo;

  // GdkWindow *window = gtk_widget_get_window(widget);
  // /* Determine GtkDrawingArea dimensions */
  // gdk_window_get_geometry(window, &da.x, &da.y, &da.width, &da.height);

  /* Draw on a background */
  cairo_set_source_rgb(gp.cr, 0.41, 0.78, 0.78);
  cairo_paint(gp.cr);

  /* Change the transformation matrix */
  // cairo_translate(gp.cr, da.width / 2, da.height / 2);
  // cairo_scale(gp.cr, scale_x, scale_y);

  /* Determine the data points to calculate (ie. those in the clipping zone */
  cairo_device_to_user_distance(gp.cr, &gp.dx, &gp.dy);
  // printf("%lf %lf \n", gp.dx, gp.dy);

  

  gp.min_x = -300;
  gp.min_y = -300;
  gp.max_x = 300;
  gp.max_y = 300;

  cairo_translate(gp.cr, (gp.max_x - gp.min_x) / 2, (gp.max_y - gp.min_y) / 2);

  draw_axis(&gp);

  if (input_validation(expression) == S21_CORRECT_INPUT) {
    if (!write_dom_codom) set_values_from_spin_buttons(&gp);
    cairo_set_line_width(gp.cr, gp.dx * 3);
    draw_graph_line(&gp);
    gtk_label_set_text(GTK_LABEL(graph_error_label), (const gchar *)"");
  } else {
    gtk_label_set_text(GTK_LABEL(graph_error_label),
                       (const gchar *)"INCORRECT INPUT");
  }
  return FALSE;
}

void set_values_from_spin_buttons(s_graph_properties *gp) {
  gdouble min_domain =
      gtk_spin_button_get_value(GTK_SPIN_BUTTON(min_domain_spin));
  gdouble min_codomain =
      gtk_spin_button_get_value(GTK_SPIN_BUTTON(min_codomain_spin));
  gdouble max_domain =
      gtk_spin_button_get_value(GTK_SPIN_BUTTON(max_domain_spin));
  gdouble max_codomain =
      gtk_spin_button_get_value(GTK_SPIN_BUTTON(max_codomain_spin));
  if (gp->min_x < min_domain) gp->min_x = min_domain;
  if (gp->min_y < min_domain) gp->min_y = min_codomain;
  if (gp->max_x > min_domain) gp->max_x = max_domain;
  if (gp->max_y > min_domain) gp->max_y = max_codomain;
}

void draw_graph_line(s_graph_properties *gp) {
  setlocale(LC_NUMERIC, "C");
  for (gdouble x = gp->min_x; x < gp->max_x; x += gp->dx) {
    gdouble y_value = calculation(expression, &x, NULL);
    if (y_value > gp->max_y || y_value < gp->min_y) {
      if ((y_value > gp->max_y)) {
        cairo_move_to(gp->cr, x, gp->max_y);
      } else if ((y_value < gp->min_y)) {
        cairo_move_to(gp->cr, x, gp->min_y);
      }
    } else if (!isnan(y_value) && !isinf(y_value)) {
      cairo_line_to(gp->cr, x, y_value);
    }
  }
  /* Draw the curve */
  cairo_set_source_rgba(gp->cr, 0.72, 0.0, 1, 1);
  cairo_stroke(gp->cr);
}

void draw_axis(s_graph_properties *gp) {
  cairo_set_source_rgb(gp->cr, 0.0, 0.0, 0.0);
  cairo_set_line_width(gp->cr, gp->dx / 10);
  int step = 20;  // 0.5
  gdouble middleX = 0;
  gdouble middleY = 0;
  for (gdouble i = middleX; i <= gp->max_x; i += step) {
    cairo_move_to(gp->cr, i, gp->min_y);
    cairo_line_to(gp->cr, i, gp->max_y);
  }
  cairo_move_to(gp->cr, middleX, middleY);
  cairo_show_text(gp->cr, "0");
  double x_axys_value = 0;
  char buffer[64];
  for (gdouble i = middleX; i >= gp->min_x; i -= step) {
    cairo_move_to(gp->cr, i, middleY);
    sprintf(buffer, "%.lf", x_axys_value);
    cairo_show_text(gp->cr, buffer);
    cairo_move_to(gp->cr, i, gp->min_y);
    cairo_line_to(gp->cr, i, gp->max_y);
  }
  for (gdouble i = middleY; i <= gp->max_y; i += step) {
    cairo_move_to(gp->cr, gp->min_x, i);
    cairo_line_to(gp->cr, gp->max_x, i);
  }
  for (gdouble i = middleY; i >= gp->min_y; i -= step) {
    cairo_move_to(gp->cr, gp->min_x, i);
    cairo_line_to(gp->cr, gp->max_x, i);
  }

  cairo_stroke(gp->cr);

  cairo_set_line_width(gp->cr, gp->dx);
  cairo_move_to(gp->cr, gp->min_x, middleY);
  cairo_line_to(gp->cr, gp->max_x, middleY);
  cairo_move_to(gp->cr, middleX, gp->min_y);
  cairo_line_to(gp->cr, middleX, gp->max_y);

  cairo_stroke(gp->cr);
}

void button_draw_clicked(GtkWidget *button, gpointer entry) {
  sprintf(expression, "%s", gtk_entry_get_text(entry));
  gtk_widget_queue_draw(drawing_area);
}

void close_window(GtkWidget *widget, gpointer window) {
  gtk_widget_destroy(GTK_WIDGET(window));
}

void scale_plus_clicked_cb() {
  scale += 10;
  if (scale > 200) scale = 200;
  char scale_label_buffer[10] = {};
  sprintf(scale_label_buffer, "1/%d", (int)scale);
  gtk_label_set_text(GTK_LABEL(scale_label), scale_label_buffer);
}

void scale_minus_clicked_cb() {
  scale -= 10;
  if (scale < 10) scale = 10;
  char scale_label_buffer[10] = {};
  sprintf(scale_label_buffer, "1/%d", (int)scale);
  gtk_label_set_text(GTK_LABEL(scale_label), scale_label_buffer);
}

void graph_toggle_button_toggled_cb(GtkToggleButton *button) {
  gboolean status = gtk_toggle_button_get_active(button);
  if (status)
    write_dom_codom = 1;
  else
    write_dom_codom = 0;
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
