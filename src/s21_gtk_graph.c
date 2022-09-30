#include <cairo.h>
#include <gtk/gtk.h>

#include "s21_smartcalc.h"

#define WIDTH 640
#define HEIGHT 480

#define ZOOM_X 100.0
#define ZOOM_Y 100.0

char expression[S21_MAX_INPUT + 8] = {};

static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
  GdkRectangle da;            /* GtkDrawingArea size */
  gdouble dx = 2.0, dy = 2.0; /* Pixels between each point */
  gdouble x, clip_x1 = 0.0, clip_y1 = 0.0, clip_x2 = 0.0, clip_y2 = 0.0;
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

  cairo_set_line_width(cr, dx * 2);

  /* Link each data point */
  gdouble y_value_buffer = calculation(expression, &clip_x1, NULL);
  ;

  for (x = clip_x1; x < clip_x2; x += dx) {
    gdouble y_value = calculation(expression, &x, NULL);
    if (y_value_buffer - y_value < 100 * dx) {
      cairo_line_to(cr, x, y_value);
    } else {
      cairo_move_to(cr, x, y_value);
    }
    y_value_buffer = y_value;
  }

  /* Draw the curve */
  cairo_set_source_rgba(cr, 0.72, 0.0, 1, 1);
  cairo_stroke(cr);

  return FALSE;
}

int graph_output(char *input) {
  for (int i = 0; input[i]; i++) {
    expression[i] = input[i];
  }

  GtkWidget *window;
  GtkWidget *da;

  gtk_init(NULL, NULL);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
  gtk_window_set_title(GTK_WINDOW(window), "Graph drawing");
  g_signal_connect(G_OBJECT(window), "destroy", gtk_main_quit, NULL);

  da = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), da);

  g_signal_connect(G_OBJECT(da), "draw", G_CALLBACK(on_draw), NULL);

  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
