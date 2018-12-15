#include <stdlib.h>
#include <stdio.h>

#include <glib.h>

int main() {
    const long long int num_generations = 20;
    const int num_rules = 1 << 5;

    FILE *f = fopen("input.txt", "r");

    char s[255];
    fscanf(f, "initial state: %s\n", s);

    gint num_pots = strlen(s);
    gint first_pot_index = 0;
    gint buffer_size = 10000000;
    guint8* buffer_start = g_new(guint8, buffer_size);
    guint8* buffer_end = buffer_start+buffer_size;
    guint8* first_pot = buffer_start+(buffer_size-num_pots)/2;
    guint8* last_pot = first_pot+num_pots;
    for (int i = 0; i < num_pots; i++) {
	first_pot[i] = s[i] == '#' ? 1 : 0;
    }

    fscanf(f, "\n");
    guint8 rules[num_rules];
    for (int i = 0; i < num_rules; i++) {
    	char key[6], value[2];
    	fscanf(f, "%s => %s\n", key, value);
	guint8 hash = 0;
	if (key[0] == '#') hash += 1;
	if (key[1] == '#') hash += 2;
	if (key[2] == '#') hash += 4;
	if (key[3] == '#') hash += 8;
	if (key[4] == '#') hash += 16;
    	rules[hash] = value[0] == '#' ? 1 : 0;
    }

    fclose(f);

    /* From a specific pattern, we build a binary hash that is used to
     * retrieve the corresponding rule. The left-most pot is the least
     * significant bit
     * ##..# -> 1+2+16 = 19
     *
     * As we compute the next generation, the key can be updated:
     * 1. if the next pot on the right is full, then the key becomes
     *    #..## -> 1+8+16 = 25 = 19 >> 1 + 16
     * 2. if the next pot on the right is empty, then the key becomes
     *    #..#. -> 1+8 = 25 = 19 >> 1
     * the update rule is therefore
     *
     *     key = key >> 1 + 16*(*right)
     *
     * where right points to the next pot on the right.
     *
     * To compute the next generation, we start two pots on the left of the
     * left-most pot, and the corresponding pattern is ....? where ? is the
     * state of the left-most pot. So the initial value of the key is just
     * 16*state[0].
     */
    for (int generation = 1; generation <= num_generations; generation++) {
	guint8* right = first_pot;

	gint hash = 16*right[0]; right++;
	const guint8 pot_m2 = rules[hash];
	hash = (hash >> 1) + 16*right[0]; right++;
	const guint8 pot_m1 = rules[hash];
	for (guint8* pot = first_pot; pot != last_pot; pot++) {
	    hash = hash >> 1;
      	    if (right != last_pot) { hash += 16*right[0]; right += 1; }
	    *pot = rules[hash];
	}
	hash = hash >> 1;
	const guint8 pot_p1 = rules[hash];
	hash = hash >> 1;
	const guint8 pot_p2 = rules[hash];

	const gint left_expansion = pot_m2 ? 2 : (pot_m1 ? 1 : 0);
	const gint right_expansion = pot_p2 ? 2 : (pot_p1 ? 1 : 0);

	if ((first_pot - left_expansion < buffer_start) ||
	    (last_pot + right_expansion > buffer_end)) {
	    printf("Buffer is not large enough\n");
	    return 0;
	}

	first_pot -= left_expansion;
	if (left_expansion == 1) {
	    first_pot[0] = pot_m1;
	} else if (left_expansion == 2) {
	    first_pot[0] = pot_m2;
	    first_pot[1] = pot_m1;
	}

	last_pot += right_expansion;
	if (right_expansion == 1) {
	    last_pot[-1] = pot_p1;
	} else if (right_expansion == 2) {
	    last_pot[-1] = pot_p2;
	    last_pot[-2] = pot_p1;
	}

	first_pot_index -= left_expansion;
	num_pots += left_expansion+right_expansion;
    }

    long long int answer = 0;
    for (int i = 0; i < num_pots; i++) {
	answer += first_pot[i]*(i+first_pot_index);
    }
    printf("Answer after %lld generations: %lld\n", num_generations, answer);

    g_free(buffer_start);

}
