#include <stdlib.h>
#include <stdio.h>


/* The sum of all metadata entries is: 42768 */
/* The value of the root node is: 34348 */

int sum_of_entries(FILE *f) {
    int num_children, num_entries;
    fscanf(f, "%d %d", &num_children, &num_entries);
    int out = 0;
    for (int child = 0; child < num_children; child++) {
	out += sum_of_entries(f);
    }
    for (int entry = 0; entry < num_entries; entry++) {
	int val;
	fscanf(f, "%d", &val);
	out += val;
    }
    return out;
}

int value_of_node(FILE *f) {
    int num_children, num_entries;
    fscanf(f, "%d %d", &num_children, &num_entries);
    if (num_children == 0) {
	int out = 0;
	for (int entry = 0; entry < num_entries; ++entry) {
	    int val;
	    fscanf(f, "%d", &val);
	    out += val;
	}
	return out;
    }
    int *child_value = malloc(num_children*sizeof(int));
    for (int child = 0; child < num_children; child++) {
	child_value[child] = value_of_node(f);
    }
    int out = 0;
    for (int entry = 0; entry < num_entries; entry++) {
	int child;
	fscanf(f, "%d", &child);
	child -= 1;
	if ((child >= 0) && (child < num_children)) {
	    out += child_value[child];
	}
    }
    free(child_value);
    return out;
};

int main() {
    FILE *f = fopen("input.txt", "r");
    printf("The sum of all metadata entries is: %d\n", sum_of_entries(f));
    rewind(f);
    printf("The value of the root node is: %d\n", value_of_node(f));
    fclose(f);
}
