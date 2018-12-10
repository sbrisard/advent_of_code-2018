#include <stdlib.h>
#include <stdio.h>

#include <glib.h>


typedef struct {
    int number;
} Marble;


typedef struct {
    GList* circle;
    GList* current;
    int length;
    long long *scores;
    int num_players;
} MarbleGame;


MarbleGame* marble_game_new(int num_players) {
    MarbleGame* game = g_new(MarbleGame, 1);
    game->circle = NULL;
    game->current = NULL;
    game->length = 0;
    game->num_players = num_players;
    game->scores = g_new(long long, num_players);
    for (int player = 0; player < num_players; player++) {
	game->scores[player] = 0;
    }
    return game;
}


void marble_game_free(MarbleGame* game) {
    g_list_free(game->circle);
    g_free(game->scores);
    g_free(game);
}


void marble_game_print(MarbleGame* game) {
    for (GList* i = game->circle; i != NULL; i = i->next) {
	char *template = i == game->current ? "(%d) " : "%d ";
	printf(template, ((Marble *) i->data)->number);
    }
}


void marble_game_print_scores(MarbleGame *game) {
    for (int player = 0; player < game->num_players; player++) {
	printf("%d ", game->scores[player]);
    }
}

void marble_game_add(MarbleGame* game, Marble* marble) {
    int position;
    if (game->length == 0) {
	game->circle = g_list_append(game->circle, marble);
	game->current = game->circle;
    } else {
	GList* position = game->current->next;
	if (position == NULL) position = game->circle;
	game->circle = g_list_insert_before(game->circle, position->next,
					    marble);
	game->current = position->next;
    }
    game->length += 1;
}


int main() {
    /* Answer is 385820. */
    /* const int num_players = 468; */
    /* const int num_marbles = 71844; */

    const int num_players = 468;
    const int num_marbles = 71843*100+1;

    _Bool verbose = 0;
    Marble* marbles = g_new(Marble, num_marbles);
    for (int i = 0; i < num_marbles; ++i) { marbles[i].number = i; }


    MarbleGame* game = marble_game_new(num_players);

    for (int i = 0; i < num_marbles; i++) {
    	if ((i == 0) || (i % 23 != 0)) {
    	    marble_game_add(game, marbles+i);
    	} else {
	    const int player = i % game->num_players;
    	    game->scores[player] += i;
    	    GList* to_remove = game->current;
	    for (int j = 0; j < 7; j++) {
		to_remove = g_list_previous(to_remove);
		if (to_remove == NULL) to_remove = g_list_last(game->circle);
	    }
    	    game->scores[player] += ((Marble*)to_remove->data)->number;
	    game->current = to_remove->next;
	    if (game->current == NULL) game->current = game->circle;
    	    game->circle = g_list_remove_link(game->circle, to_remove);
    	    game->length -= 1;
    	}
	if (verbose) {
	    printf("[%d] ", i % game->num_players);
	    marble_game_print(game);
	    printf(" --- ");
	    marble_game_print_scores(game);
	    printf("\n");
	    fflush(stdout);
	}
    }

    int winner = -1;
    long long highest_score = -1;
    for (int player = 0; player < game->num_players; ++player) {
	const long long score = game->scores[player];
	if (score > highest_score) {
	    winner = player;
	    highest_score = score;
	}
    }
    printf("Player %d wins with score: %lld\n", winner, highest_score);

    /* There is something wrong with these deallocations. */
    /*
    g_free(game->scores);
    marble_game_free(game);
    g_free(marbles);
    */

    return 0;
}
