#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "game_logic.c"

int main(int argc, const char *argv[])
{
    startGame();
    return 0;
};

void startGame()
{
    int num_players = 2;
    Game *game = malloc(sizeof(Game));
    game->players = malloc(num_players * sizeof(Player));
    game->current_player_id = 0;
    game->round_number = 1;
    game->turn_number = 1;

    clearTerminal();

    for (int i = 0; i < num_players; i++)
    {
        game->players[i].id = i + 1;
        game->players[i].score = 0;
        game->players[i].cards_count = 0;
        game->players[i].cards = NULL;

        printf("Coloque o nome do jogador %d: ", i + 1);
        scanf("%49s", game->players[i].name);
        clearTerminal();
    }

    while (game->players[0].score < 12 && game->players[1].score < 12)
    {
        startNewRound(game);
    }

    free(game->players);
    free(game);
}

void startNewRound(Game *game)
{
    bool break_while = false;
    Round round = {
        .winner_id = -1,
        .is_challenge = false,
        .last_player_challenger_id = -1,
        .is_challenge_accepted = false,
        .point_value = 1,
        .turn = {
            {.winner_id = -1, .player1_card = {.value = -1, .type = -1}, .player2_card = {.value = -1, .type = -1}},
            {.winner_id = -1, .player1_card = {.value = -1, .type = -1}, .player2_card = {.value = -1, .type = -1}},
            {.winner_id = -1, .player1_card = {.value = -1, .type = -1}, .player2_card = {.value = -1, .type = -1}}
        }
    };
    getCards(game);
    
    for (int i = 0; i < 3; i++)
    {
        startATurn(game, &round, &round.turn[i], &break_while);
        if (break_while)
        {
            break;
        }
        game->turn_number++;

        if (checkRoundWinner(&round, i))
        {
            break;
        }
    }

    if (break_while) {
        endRound(game, round.last_player_challenger_id, round.point_value);
    } else {
        endRound(game, round.winner_id, round.point_value);
    }
}

void endRound(Game *game, int winner_id, int point_value)
{
    printf("Fim da rodada %d\n", game->round_number);
    game->turn_number = 1;
    game->round_number++;

    if (winner_id == -1)
    {
        printf("Empate da rodada\n");
    } else {
        game->players[winner_id].score += point_value;
        printf("Jogador %s ganhou a rodada! +%d pontos\n", game->players[winner_id].name, point_value);
    }

    printf("Pressione qualquer tecla para continuar...\n");
    fflush(stdin); 
    getchar();
    fflush(stdin); 
    clearTerminal();
}

void startATurn(Game *game, Round *round, Turn *turn, bool *break_while)
{
    while (turn->player1_card.value == -1 || turn->player2_card.value == -1) {
        
        printf("Rodada %d\n", game->round_number);
        printf("Turno %d\nJogador %s\n", game->turn_number, game->players[game->current_player_id].name);
        printf("Pontuação: Jogador 1 - %d\nPontuação: Jogador 2 - %d\n\n", game->players[0].score, game->players[1].score);

        showLastPlayerCard(round, game->turn_number - 1, game->current_player_id);

        printCards(&game->players[game->current_player_id]);
    
        turnAction(game, turn, game->current_player_id == 0, round, break_while);
    
        if (game->current_player_id == 0) {
            game->current_player_id = 1;
        } else {
            game->current_player_id = 0;
        }

        clearTerminal();

        if (break_while)
        {
            break;
        }
    }

    if (break_while)
    {
        turn->winner_id = round->last_player_challenger_id;
        return;
    } else {
    int winner_id = compareCards(turn->player1_card, turn->player2_card);
    turn->winner_id = winner_id;
    if (winner_id == -1)
    {
        printf("Empate da rodada");
    }
    else
    {
        printf("Jogador %s ganhou o turno!\n", game->players[winner_id].name);
        printf("Pressione qualquer tecla para continuar...\n");
        fflush(stdin); 
        getchar();
        fflush(stdin); 
        clearTerminal();
    }
}
}

void showLastPlayerCard(Round *round, int last_turn, int player_id)
{
    if (player_id == 1) {
        if (round->turn[last_turn].player1_card.type == -1) {
            return;
        } else {
        printf("Ultima carta jogada [%s - %d]\n\n", getTypeString(round->turn[last_turn].player1_card.type), round->turn[last_turn].player1_card.value);
        }
    } else {
        if (round->turn[last_turn].player2_card.type == -1) {
            return;
        } else {
        printf("Ultima carta jogada [%s - %d]\n\n", getTypeString(round->turn[last_turn].player2_card.type), round->turn[last_turn].player2_card.value);
        }
    }
}