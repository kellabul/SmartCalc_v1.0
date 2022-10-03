#include <cairo.h>
#include <math.h>

#include "s21_smartcalc.h"
#include "s21_smartcalc_gtk.h"

#define ZOOM_X 100.0
#define ZOOM_Y 100.0

char *expression;
GtkWidget *drawing_area;
GtkWidget *graph_error_label;

int graph_output(char *input) {
  GtkBuilder *builder;
  GtkWidget *graph_close_button;
  GtkEntry *graph_entry;
  GtkWidget *graph_window;

  builder = gtk_builder_new_from_file("GUI/s21_smartcalc_graph_gui.glade");

  graph_window = GTK_WIDGET(gtk_builder_get_object(builder, "graph_window"));
  drawing_area = GTK_WIDGET(gtk_builder_get_object(builder, "graph_da"));
  graph_entry = GTK_ENTRY(gtk_builder_get_object(builder, "graph_entry"));
  graph_close_button =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_close"));
  graph_error_label =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_error_label"));

  gtk_entry_set_text(graph_entry, (const gchar *)input);

  gtk_builder_connect_signals(builder, NULL);

  expression = input;

  g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw), NULL);

  g_signal_connect(G_OBJECT(graph_close_button), "clicked",
                   G_CALLBACK(close_window), G_OBJECT(graph_window));

  gtk_widget_show_all(graph_window);

  return 0;
}

static gboolean on_draw(GtkWidget *widget, cairo_t *cairo) {
  int window_width = 750;   // pixels
  int window_height = 750;  // pixels
  int scale_x = ZOOM_X;
  int scale_y = -ZOOM_Y;
  GdkRectangle da; /* GtkDrawingArea size */
  s_graph_properties gp = {};
  gp.dx = 1.0;
  gp.dy = 1.0; /* Pixels between each point */
  gp.cr = cairo;

  GdkWindow *window = gtk_widget_get_window(widget);
  /* Determine GtkDrawingArea dimensions */
  gdk_window_get_geometry(window, &da.x, &da.y, &da.width, &da.height);

  /* Draw on a background */
  cairo_set_source_rgb(gp.cr, 0.41, 0.78, 0.78);
  cairo_paint(gp.cr);

  /* Change the transformation matrix */
  cairo_translate(gp.cr, da.width / 2, da.height / 2);
  cairo_scale(gp.cr, scale_x, scale_y);

  /* Determine the data points to calculate (ie. those in the clipping zone */
  cairo_device_to_user_distance(gp.cr, &gp.dx, &gp.dy);
  // printf("%lf %lf \n", gp.dx, gp.dy);
  cairo_clip_extents(gp.cr, &gp.min_x, &gp.min_y, &gp.max_x, &gp.max_y);

  draw_axis(gp);
  gtk_widget_set_size_request(drawing_area, window_width, window_height);

  if (input_validation(expression) == S21_CORRECT_INPUT) {
    cairo_set_line_width(gp.cr, gp.dx * 2);
    draw_graph_line(gp);
    gtk_label_set_text(GTK_LABEL(graph_error_label), (const gchar *)"");
  } else {
    gtk_label_set_text(GTK_LABEL(graph_error_label),
                       (const gchar *)"INCORRECT INPUT");
  }
  return FALSE;
}

void draw_graph_line(s_graph_properties gp) {
  // int flag = 0;
  setlocale(LC_NUMERIC, "C");
  for (gdouble x = gp.min_x; x < gp.max_x; x += gp.dx / 10) {
    gdouble y_value = calculation(expression, &x, NULL);
    if (s21_isnan(y_value) || s21_isinf(y_value) || y_value > gp.max_y ||
        y_value < gp.min_y) {
      // if ((y_value > gp.max_y) && flag) {
      //   cairo_line_to(gp.cr, x, gp.max_y);
      // } else if ((y_value < gp.min_y) && flag) {
      //   cairo_line_to(gp.cr, x, gp.max_y);
      // } else
      if ((y_value > gp.max_y)) {
        cairo_move_to(gp.cr, x, gp.max_y);
      } else if ((y_value < gp.min_y)) {
        cairo_move_to(gp.cr, x, gp.min_y);
      }
      //  flag = 0;
    } else {
      //    flag = 1;
      cairo_line_to(gp.cr, x, y_value);
    }
  }
  /* Draw the curve */
  cairo_set_source_rgba(gp.cr, 0.72, 0.0, 1, 1);
  cairo_stroke(gp.cr);
}

void draw_axis(s_graph_properties gp) {
  printf("%lf %lf %lf %lf %lf \n", gp.min_x, gp.max_x, gp.min_y, gp.max_y,
         gp.dx);

  cairo_set_source_rgb(gp.cr, 0.0, 0.0, 0.0);
  cairo_set_line_width(gp.cr, gp.dx / 10);
  for (gdouble i = 0; i <= gp.max_x; i += 0.5) {
    cairo_move_to(gp.cr, i, gp.min_y);
    cairo_line_to(gp.cr, i, gp.max_y);
  }
  for (gdouble i = 0; i >= gp.min_x; i -= 0.5) {
    cairo_move_to(gp.cr, i, gp.min_y);
    cairo_line_to(gp.cr, i, gp.max_y);
  }
  for (gdouble i = 0; i <= gp.max_y; i += 0.5) {
    cairo_move_to(gp.cr, gp.min_x, i);
    cairo_line_to(gp.cr, gp.max_x, i);
  }
  for (gdouble i = 0; i >= gp.min_y; i -= 0.5) {
    cairo_move_to(gp.cr, gp.min_x, i);
    cairo_line_to(gp.cr, gp.max_x, i);
  }

  cairo_stroke(gp.cr);

  cairo_set_line_width(gp.cr, gp.dx);
  cairo_move_to(gp.cr, gp.min_x, 0.0);
  cairo_line_to(gp.cr, gp.max_x, 0.0);
  cairo_move_to(gp.cr, 0.0, gp.min_y);
  cairo_line_to(gp.cr, 0.0, gp.max_y);

  cairo_stroke(gp.cr);
}

void button_draw_clicked_cb() { gtk_widget_queue_draw(drawing_area); }

void graph_entry_changed_cb(GtkEntry *entry) {
  sprintf(expression, "%s", gtk_entry_get_text(entry));
}

void close_window(GtkWidget *widget, gpointer window) {
  gtk_widget_destroy(GTK_WIDGET(window));
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  //
void graph_toggle_button_toggled_cb() {}