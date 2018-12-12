#include <stdlib.h>
#include <stdio.h>

#include <glib.h>

typedef struct {
    gint x, y, vx, vy;
} Point;


typedef struct {
    gint xmin, ymin, xmax, ymax, stride;
    gint *hproj, *vproj;
} Projections;


typedef struct {
    gint xmin, ymin, xmax, ymax, stride;
    guint8* data;
} Grid;


void points_bounds__(gpointer data, gpointer user_data) {
    Point* point = (Point*)data;
    gint *bounds = (gint*)user_data;
    if (point->x < bounds[0]) bounds[0] = point->x;
    if (point->y < bounds[1]) bounds[1] = point->y;
    if (point->x > bounds[2]) bounds[2] = point->x;
    if (point->y > bounds[3]) bounds[3] = point->y;
}


void points_bounds(GSList* points, gint* bounds) {
    bounds[0] = G_MAXINT;
    bounds[1] = G_MAXINT;
    bounds[2] = G_MININT;
    bounds[3] = G_MININT;
    g_slist_foreach(points, points_bounds__, bounds);
}


Projections* projections_new(gint xmin, gint ymin, gint xmax, gint ymax) {
    Projections* proj = g_new(Projections, 1);
    proj->xmin = xmin;
    proj->ymin = ymin;
    proj->xmax = xmax;
    proj->ymax = ymax;

    gint width = xmax-xmin+1;
    proj->vproj = g_new(gint, width);
    for (int i = 0; i < width; i++) proj->vproj[i] = 0;

    gint height = ymax-ymin+1;
    proj->hproj = g_new(gint, height);
    for (int i = 0; i < height; i++) proj->hproj[i] = 0;

    return proj;
}


void projections_free(Projections* proj) {
    g_free(proj->hproj);
    g_free(proj->vproj);
    g_free(proj);
}


void projections__(gpointer data, gpointer user_data) {
    Point* point = (Point*)data;
    Projections* proj = (Projections*)user_data;
    proj->hproj[point->y-proj->ymin] += 1;
    proj->vproj[point->x-proj->xmin] += 1;
}


Projections* points_projections(GSList* points) {
    gint bounds[4];
    points_bounds(points, bounds);
    Projections *proj = projections_new(bounds[0], bounds[1],
					bounds[2], bounds[3]);
    g_slist_foreach(points, projections__, proj);
    return proj;
}


void points_forward__update(gpointer data, gpointer user_data) {
    Point* point = (Point*)data;
    gint num_steps = ((gint*)user_data)[0];
    point->x += num_steps*point->vx;
    point->y += num_steps*point->vy;
}


GSList* points_forward(GSList* points, gint num_steps) {
    g_slist_foreach(points, points_forward__update, &num_steps);
}


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
    guint8* data = grid->data;
    for (int j = 0; j < height; j++) {
	for (int i = 0; i < width; i++) {
	    if (data[0]) {
		printf("#");
	    } else {
		printf(".");
	    }
	    data += 1;
	}
	printf("\n");
    }
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
    points_bounds(points, bounds);
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
    GSList* points = read_points("input.txt");
    for (int t = 1;; t++) {
	points_forward(points, 1);
	Projections* proj = points_projections(points);
	int width = proj->xmax-proj->xmin+1;
	int candidate = 0;
	for (int i = 0; i < width; i++) {
	    if (proj->vproj[i] == 8) {
		candidate = 1;
		break;
	    }
	}
	projections_free(proj);
	if (candidate) {
	    Grid* grid = fill_grid(points);
	    printf("t = %d -- Is this a readable message (y/n)?\n", t);
	    printf("\n");
	    grid_print(grid);
	    printf("\n");
	    grid_free(grid);
	    fflush(stdout);
	    int answer = fgetc(stdin);
	    if (answer == 'y') break;
	}
    }
}
