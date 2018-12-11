#include <stdlib.h>
#include <stdio.h>

#include <glib.h>

typedef struct {
    gint x, y, vx, vy;
} Point;


typedef struct {
    gint xmin, ymin, xmax, ymax, stride;
    guint8* data;
} Grid;


Grid* grid_new(gint xmin, gint ymin, gint xmax, gint ymax) {
    Grid* grid = g_new(Grid, 1);
    grid->xmin = xmin;
    grid->ymin = ymin;
    grid->xmax = xmax;
    grid->ymax = ymax;
    grid->stride = xmax-xmin+1;
    guint length = (xmax-xmin+1)*(ymax-ymin+1);
    grid->data = g_new(guint8, length);
    for (int i = 0; i < length; i++) {
	grid->data[i] = 0;
    }
    return grid;
}


void grid_free(Grid* grid) {
    g_free(grid->data);
    g_free(grid);
}


void grid_print(Grid* grid) {
    gint width = grid->xmax-grid->xmin+1;
    gint height = grid->ymax-grid->ymin+1;
    printf("%d %d\n", width, height);
    fflush(stdout);
    guint8* data = grid->data;
    for (int j = 0; j < height; j++) {
	for (int i = 0; i < width; i++) {
	    char c = data[0] ? '#' : '.';
	    printf(&c);
	    data += 1;
	}
	printf("\n");
    }
}


void fill_grid__update_bounds(gpointer data, gpointer user_data) {
    Point* point = (Point*)data;
    gint *bounds = (gint*)user_data;
    if (point->x < bounds[0]) bounds[0] = point->x;
    if (point->y < bounds[1]) bounds[1] = point->y;
    if (point->x > bounds[2]) bounds[2] = point->x;
    if (point->y > bounds[3]) bounds[3] = point->y;
}


void fill_grid__update_grid(gpointer data, gpointer user_data) {
    Point* point = (Point*)data;
    Grid* grid = (Grid*)user_data;
    gint x = point->x-grid->xmin;
    gint y = point->y-grid->ymin;
    grid->data[grid->stride*y+x] = 1;
}


Grid* fill_grid(GSList* points) {
    gint bounds[] = {G_MAXINT, G_MAXINT, G_MININT, G_MININT};
    g_slist_foreach(points, fill_grid__update_bounds, bounds);
    printf("%d <= x <= %d    %d <= y <= %d\n", bounds[0], bounds[2], bounds[1], bounds[3]);
    Grid* grid = grid_new(bounds[0], bounds[1], bounds[2], bounds[3]);
    g_slist_foreach(points, fill_grid__update_grid, grid);
    return grid;
}


GSList* read_points(char* filename) {
    GSList* points = NULL;
    FILE* f = fopen(filename, "r");
    while (!feof(f)) {
	Point* p = g_new(Point, 1);
	fscanf(f, "position=<%d,%d> velocity=<%d,%d>\n",
	       &(p->x), &(p->y), &(p->vx), &(p->vy));
	points = g_slist_append(points, p);
    }
    return points;
}



int main() {
    GSList* points = read_points("reference.txt");
    Grid* grid = fill_grid(points);
    grid_print(grid);
    /*grid_free(grid);*/
}
