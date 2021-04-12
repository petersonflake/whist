#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "whist.h"
#include "game_round.h"

int main()
{
    srand(time(0));
    game_round *game = create_game(init_human_player(), init_ai_player(), init_ai_player(), init_ai_player());
    game->play_game(game);
    free_game_round(game);
    return 0;
}

