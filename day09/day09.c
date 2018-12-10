#include <stdlib.h>
#include <stdio.h>

#include <glib.h>


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


MarbleGame* marble_game_new(int num_players) {
    MarbleGame* game = g_new(MarbleGame, 1);
    game->circle = NULL;
    game->length = 0;
    game->current = 0;
    game->num_players = num_players;
    game->scores = g_new(int, num_players);
    for (int player = 0; player < num_players; player++) {
	game->scores[player] = 0;
    }
    return game;
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


void marble_game_print_scores(MarbleGame *game) {
    for (int player = 0; player < game->num_players; player++) {
	printf("%d ", game->scores[player]);
    }
}

void marble_game_add(MarbleGame* game, Marble* marble) {
    int position;
    if (game->length == 0) {
	game->circle = g_slist_append(game->circle, marble);
	game->current = 0;
    } else {
	position = game->current+2;
	if (position > game->length) {
	    position -= game->length;
	}
	game->circle = g_slist_insert(game->circle, marble, position);
	game->current = position;
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
    for (int i = 0; i < num_marbles; ++i) {
	marbles[i].number = i;
    }


    MarbleGame* game = marble_game_new(num_players);

    for (int i = 0; i < num_marbles; i++) {
    	if ((i == 0) || (i % 23 != 0)) {
    	    marble_game_add(game, marbles+i);
    	} else {
	    const int player = i % game->num_players;
    	    game->scores[player] += i;
    	    int position = game->current-7;
    	    if (position < 0) position += game->length;
    	    Marble* to_remove = (Marble*)(g_slist_nth(game->circle, position)->data);
    	    game->scores[player] += to_remove->number;
    	    game->circle = g_slist_remove(game->circle, to_remove);
    	    game->length -= 1;
    	    if (position > game->length) {position -= game->length;}
    	    game->current = position;
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
    int highest_score = -1;
    for (int player = 0; player < game->num_players; ++player) {
	const int score = game->scores[player];
	if (score > highest_score) {
	    winner = player;
	    highest_score = score;
	}
    }
    printf("Player %d wins with score: %d\n", winner, highest_score);

    /* There is something wrong with these deallocations. */
    /*
    g_free(game->scores);
    marble_game_free(game);
    g_free(marbles);
    */

    return 0;
}
