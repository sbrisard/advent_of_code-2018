#include <stdlib.h>
#include <stdio.h>

#include <glib.h>

typedef struct {
    gint* data;
    gint width, height, serial;
} Grid;


int power_level(int x, int y, int serial_number) {
    int rack_ID = x+10;
    int out = rack_ID*y+serial_number;
    out *= rack_ID;
    out = (out/100) % 10 - 5;
    return out;
}


Grid* grid_new(int width, int height, int serial) {
    Grid* grid = g_new(Grid, 1);
    grid->data = g_new(int, width*height);
    grid->width = width;
    grid->height = height;
    grid->serial = serial;
    return grid;
}


void grid_free(Grid* grid) {
    g_free(grid->data);
    g_free(grid);
}


void grid_compute_power(Grid* grid) {
    int* start = grid->data;
    int* end = start+grid->width*grid->height;
    int x = 0;
    int y = 0;
    for (int* cell = start; cell != end; cell++) {
	*cell = power_level(x+1, y+1, grid->serial);
	x += 1;
	if (x == grid->width) { x = 0; y += 1; }
    }
}

void grid_find_best_square(Grid* grid, int size, int *x, int *y, int *power) {
    int w = grid-> width;
    int jmax = grid->height+1-size;
    int imax = w+1-size;
    int* offset_start = g_new(int, size*size);
    int* offset_end = offset_start+size*size;
    for (int jj = 0; jj < size; jj++) {
	for (int ii = 0; ii < size; ii++) {
	    offset_start[jj*size+ii] = jj*w+ii;
	}
    }

    *x = 0;
    *y = 0;
    *power = G_MININT;
    for (int j = 0; j < jmax; j++) {
	for (int i = 0; i < imax; i++) {
	    int k0 = w*j+i;
	    int val = 0;
	    for (int* k = offset_start; k != offset_end; k++) {
		val += grid->data[k0+*k];
	    }
	    if (val > *power) {
		*x = i+1;*y = j+1; *power = val;
	    }
	}
    }
    g_free(offset_start);
}


void print_grid(Grid* grid) {
    int* start = grid->data;
    int* end  = grid->data+(grid->width*grid->height);
    int x = 0;
    for (int *cell = start; cell != end; cell++) {
	printf("%d  ", *cell);
	x += 1;
	if (x == grid-> width) {x = 0; printf("\n");}
    }
}


void test(int x, int y, int serial_number, int expected) {
    printf("Fuel cell at (%d, %d), grid serial number %d --- ",
	   x, y, serial_number);
    int actual = power_level(x, y, serial_number);
    printf(actual == expected ? "PASS\n" : "FAIL\n");
}


void test_3x3(int serial, int expected_x, int expected_y, int expected_power) {
    Grid* grid = grid_new(300, 300, serial);
    grid_compute_power(grid);
    int x, y, power;
    grid_find_best_square(grid, 3, &x, &y, &power);
    printf("Serial number: %d --- ", serial);
    if ((x == expected_x) && (y == expected_y) && (power == expected_power)) {
	printf("PASS\n");
    } else {
	printf("FAIL\n");
    }
    grid_free(grid);
}


void all_tests() {
    test(3, 5, 8, 4);
    test(122, 79, 57, -5);
    test(217, 196, 39, 0);
    test(101, 153, 71, 4);
    test_3x3(18, 33, 45, 29);
    test_3x3(42, 21, 61, 30);
}


int main() {
    all_tests();
    int serial = 6878;
    Grid* grid = grid_new(300, 300, serial);
    grid_compute_power(grid);
    int x, y, power;
    grid_find_best_square(grid, 3, &x, &y, &power);
    printf("The 3x3 square with highest power is located at (%d, %d), with power: %d.", x, y, power);

    int best_x, best_y, best_power, best_size;
    best_power = G_MININT;
    for (int n = 1; n <= 300; n++) {
	grid_find_best_square(grid, n, &x, &y, &power);
	printf("%d: (%d, %d), with power: %d\n", n, x, y, power);
	fflush(stdout);
	if (power > best_power) {
	    best_x = x;
	    best_y = y;
	    best_power = power;
	    best_size = n;
	}
    }

    printf("Solution to part II; %d,%d,%d\n", best_x, best_y, best_size);
    grid_free(grid);
}
