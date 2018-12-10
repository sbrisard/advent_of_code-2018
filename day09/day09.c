#include <stdlib.h>
#include <stdio.h>

#include <glib.h>

#define NUM_MARBLES 100
#define NUM_PLAYERS 9

typedef struct {
    int number;
} Marble;


typedef struct {
    GSList* circle;
    int length;
    int current;
    int *scores;
    int num_players;
} MarbleGame;


MarbleGame* marble_game_new() {
    MarbleGame* game = g_new(MarbleGame, 1);
    game->circle = NULL;
    game->length = 0;
    game->current = 0;
    game->num_players = NUM_PLAYERS;
    game->scores = g_new(gint, NUM_PLAYERS);
    for (int player = 0; player < NUM_PLAYERS; player++) {
	game->scores[player] = 0;
    }
}


void marble_game_free(MarbleGame* game) {
    g_slist_free(game->circle);
    g_free(game->scores);
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
    int player = 0;
    for (int i = 0; i < 23; i++) {
	if (i % 23 != 0) {
	    marble_game_add(game, marbles+i); marble_game_print(game);
	} else {
	    game->scores[player] += i;
	    int position = game->current-7;
	    if (position < 0) position += game->length;
	    GSList* to_remove = g_slist_nth(game->circle, position);
	    game->scores[player] += ((Marble*)to_remove->data)->number;
	    game->circle = g_slist_remove(game->circle, to_remove);
	    game->length -= 1;
	    if (position > game->length) {position -= game->length;}
	    game->current = position;
	}
	printf("\n");
	player = (player+1) % NUM_PLAYERS;
    }

    g_free(game->scores);
    marble_game_free(game);
    g_free(marbles);

    return 0;
}
