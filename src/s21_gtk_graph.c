#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>

#include "s21_smartcalc.h"

#define WIDTH 640
#define HEIGHT 640

#define ZOOM_X 50.0
#define ZOOM_Y 50.0

static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void close_window(GtkWidget *widget, gpointer window);
void draw_axis(cairo_t *cr, gdouble clip_x1, gdouble clip_x2, gdouble clip_y1,
               gdouble clip_y2, gdouble dx);

char *expression;
GtkWidget *drawing_area;
GtkEntry *graph_entry;
GtkWidget *graph_window;
GtkWidget *graph_error_label;

int graph_output(char *input) {
  GtkBuilder *builder;
  GtkWidget *graph_close_button;
  GtkWidget *graph_draw_button;

  builder = gtk_builder_new_from_file("GUI/s21_smartcalc_graph_gui.glade");

  graph_window = GTK_WIDGET(gtk_builder_get_object(builder, "graph_window"));
  drawing_area = GTK_WIDGET(gtk_builder_get_object(builder, "graph_da"));
  graph_entry = GTK_ENTRY(gtk_builder_get_object(builder, "graph_entry"));
  graph_close_button =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_close"));
  graph_draw_button =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_draw_button"));
  graph_error_label =
      GTK_WIDGET(gtk_builder_get_object(builder, "graph_error_label"));

  gtk_entry_set_text(graph_entry, (const gchar *)input);

  gtk_builder_connect_signals(builder, NULL);

  expression = input;

  g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw), NULL);

  g_signal_connect(G_OBJECT(graph_close_button), "clicked",
                   G_CALLBACK(close_window), G_OBJECT(graph_window));

  g_signal_connect(G_OBJECT(graph_draw_button), "clicked",
                   G_CALLBACK(gtk_widget_queue_draw), G_OBJECT(drawing_area));

  gtk_widget_show_all(graph_window);

  return 0;
}

static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
  int window_width = WIDTH;
  int window_height = HEIGHT;
  int scale_x = ZOOM_X;
  int scale_y = -ZOOM_Y;

  GdkRectangle da;            /* GtkDrawingArea size */
  gdouble dx = 1.0, dy = 1.0; /* Pixels between each point */
  gdouble x, clip_x1 = 0.0, clip_y1 = 0.0, clip_x2 = 0.0, clip_y2 = 0.0;
  GdkWindow *window = gtk_widget_get_window(widget);

  /* Determine GtkDrawingArea dimensions */
  gdk_window_get_geometry(window, &da.x, &da.y, &da.width, &da.height);

  /* Draw on a background */
  cairo_set_source_rgb(cr, 0.41, 0.78, 0.78);
  cairo_paint(cr);

  /* Change the transformation matrix */
  cairo_translate(cr, da.width / 2, da.height / 2);
  cairo_scale(cr, scale_x, scale_y);

  /* Determine the data points to calculate (ie. those in the clipping zone */
  cairo_device_to_user_distance(cr, &dx, &dy);
  // printf("%lf %lf \n", dx, dy);
  cairo_clip_extents(cr, &clip_x1, &clip_y1, &clip_x2, &clip_y2);

  draw_axis(cr, clip_x1, clip_x2, clip_y1, clip_y2, dx);

  cairo_set_line_width(cr, dx * 2);

  gtk_widget_set_size_request(drawing_area, window_width, window_height);

  if (input_validation(expression) == S21_CORRECT_INPUT) {
    /* Link each data point */
    gdouble y_value_buffer = calculation(expression, &clip_x1, NULL);

    for (x = clip_x1; x < clip_x2; x += dx) {
      gdouble y_value = calculation(expression, &x, NULL);
      if (fabs(y_value_buffer - y_value) < 1000 * dx) {
        cairo_line_to(cr, x, y_value);
      } else {
        cairo_move_to(cr, x, y_value);
      }
      y_value_buffer = y_value;
    }
    /* Draw the curve */
    cairo_set_source_rgba(cr, 0.72, 0.0, 1, 1);
    cairo_stroke(cr);
    gtk_label_set_text(GTK_LABEL(graph_error_label),
                       (const gchar *)"");
  } else {
    gtk_label_set_text(GTK_LABEL(graph_error_label),
                       (const gchar *)"INCORRECT INPUT");
  }
  return FALSE;
}

void draw_axis(cairo_t *cr, gdouble clip_x1, gdouble clip_x2, gdouble clip_y1,
               gdouble clip_y2, gdouble dx) {
  // printf("%lf %lf %lf %lf %lf \n", clip_x1, clip_x2, clip_y1, clip_y2, dx);

  cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
  cairo_set_line_width(cr, dx / 10);
  for (gdouble i = 0; i <= clip_x2; i += 0.5) {
    cairo_move_to(cr, i, clip_y1);
    cairo_line_to(cr, i, clip_y2);
  }
  for (gdouble i = 0; i >= clip_x1; i -= 0.5) {
    cairo_move_to(cr, i, clip_y1);
    cairo_line_to(cr, i, clip_y2);
  }
  for (gdouble i = 0; i <= clip_y2; i += 0.5) {
    cairo_move_to(cr, clip_x1, i);
    cairo_line_to(cr, clip_x2, i);
  }
  for (gdouble i = 0; i >= clip_y1; i -= 0.5) {
    cairo_move_to(cr, clip_x1, i);
    cairo_line_to(cr, clip_x2, i);
  }

  cairo_stroke(cr);

  cairo_set_line_width(cr, dx);
  cairo_move_to(cr, clip_x1, 0.0);
  cairo_line_to(cr, clip_x2, 0.0);
  cairo_move_to(cr, 0.0, clip_y1);
  cairo_line_to(cr, 0.0, clip_y2);

  cairo_stroke(cr);
}

void button_draw_clicked_cb() { gtk_widget_queue_draw(drawing_area); }

void graph_entry_changed_cb(GtkEntry *entry) {
  sprintf(expression, "%s", gtk_entry_get_text(entry));
}

void close_window(GtkWidget *widget, gpointer window) {
  gtk_widget_destroy(GTK_WIDGET(window));
}