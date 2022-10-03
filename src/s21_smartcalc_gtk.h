#ifndef SRC_S21_SMARTCALC_GTK_H_
#define SRC_S21_SMARTCALC_GTK_H_

#include <gtk/gtk.h>
#include <locale.h>
#include <math.h>

typedef struct cairo_graph_properties {
  cairo_t *cr;
  gdouble min_x;
  gdouble max_x;
  gdouble min_y;
  gdouble max_y;
  gdouble dx;
  gdouble dy;
} s_graph_properties;

// graph
int graph_output(char *input);
static gboolean on_draw(GtkWidget *widget, cairo_t *cairo,
                        gpointer error_label);
void close_window(GtkWidget *widget, gpointer window);
void draw_axis(s_graph_properties gp);
void draw_graph_line(s_graph_properties gp);

#endif  // SRC_S21_SMARTCALC_H_