#include <stdlib.h>
#include <stdio.h>

#include <glib.h>

#define NUM_MARBLES 100

typedef struct {
    int number;
} Marble;


typedef struct {
    GSList* circle;
    int length;
    int current;
} MarbleGame;


MarbleGame* marble_game_new() {
    MarbleGame* game = g_new(MarbleGame, 1);
    game->circle = NULL;
    game->length = 0;
    game->current = 0;
}


void marble_game_free(MarbleGame* game) {
    g_slist_free(game->circle);
    g_free(game);
}


void marble_game_print(MarbleGame* game) {
    GSList* iterator = game->circle;
    for (int i = 0; i < game->length; i++) {
	char *template = i == game->current ? "(%d) " : "%d ";
	printf(template, ((Marble *) iterator->data)->number);
	iterator = iterator->next;
    }
}


void marble_game_add(MarbleGame* game, Marble* marble) {
    int position;
    if (game->length == 0) {
	position = 0;
    } else {
	position = game->current+2;
	if (position > game->length) {
	    position -= game->length;
	}
    }
    game->circle = g_slist_insert(game->circle, marble, position);
    game->length += 1;
    game->current = position;
}


int main() {
    Marble* marbles = g_new(Marble, NUM_MARBLES);
    for (int i = 0; i < NUM_MARBLES; ++i) {
	marbles[i].number = i;
    }
    MarbleGame* game = marble_game_new();
    for (int i = 0; i < 23; i++) {
	marble_game_add(game, marbles+i); marble_game_print(game);
	printf("\n");
    }

    marble_game_free(game);
    g_free(marbles);

    return 0;
}
