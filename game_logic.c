#include "game.h"
#define total_cards_per_player 3
#define total_players 2

Type getRandomType()
{
    return (Type)(arc4random_uniform(4));
}

int getRandomValue()
{
    int value;
    do
    {
        value = (arc4random_uniform(12) + 1);
    } while (value == 8 || value == 9);
    return value;
}

void getCards(Game *game)
{
    for (int p = 0; p < total_players; p++)
    {
        game->players[p].cards = malloc(total_cards_per_player * sizeof(Card));
        game->players[p].cards_count = total_cards_per_player;
    }

    for (int p = 0; p < total_players; p++)
    {
        for (int i = 0; i < total_cards_per_player; i++)
        {
            Card newCard;
            int isDuplicate;

            do
            {
                isDuplicate = 0;
                newCard.type = getRandomType();
                newCard.value = getRandomValue();

                for (int j = 0; j < i; j++)
                {
                    if (game->players[p].cards[j].type == newCard.type &&
                        game->players[p].cards[j].value == newCard.value)
                    {
                        isDuplicate = 1;
                        break;
                    }
                }

                for (int other_p = 0; other_p < p && !isDuplicate; other_p++)
                {
                    for (int j = 0; j < total_cards_per_player; j++)
                    {
                        if (game->players[other_p].cards[j].type == newCard.type &&
                            game->players[other_p].cards[j].value == newCard.value)
                        {
                            isDuplicate = 1;
                            break;
                        }
                    }
                }

            } while (isDuplicate);

            game->players[p].cards[i] = newCard;
        }
    }
}

const char *getTypeString(Type type)
{
    return type_names[type];
}

void printCards(Player *player)
{
    for (int i = 0; i < player->cards_count; i++)
    {
        printf("(%d)[Tipo %s, Valor %d]    ", i ,getTypeString(player->cards[i].type), player->cards[i].value);
    }
    printf("\n");
}

int getCardHierarchy(Card card)
{
    int value = card.value;
    Type type = card.type;

    switch (value)
    {
    case 7:
        if (type == HEARTS)
        {
            return 13;
        } else {
            return 1;
        }
        break;
    case 6:
        return 2;
        break;
    case 5:
        return 3;
        break;
    case 4:
        if (type == CLUBS)
        {
            return 14;
        } else {
            return 4;
        }
        break;
    case 3:
        return 5;
        break;
    case 2:
        return 6;
        break;
    case 12:
        return 8;
        break;
    case 11:
        return 9;
        break;
    case 10:
        return 10;
        break;
    case 1:
        if (type == CLUBS)
        {
            return 11;
        } else if (type == DIAMONDS)
        {
            return 12;
        } else {
            return 7;
        }
        break;
    default:
        return 0;
        break;
    }
}

int compareCards(Card card1, Card card2)
{
    int hierarchy1 = getCardHierarchy(card1);
    int hierarchy2 = getCardHierarchy(card2);

    if (hierarchy1 > hierarchy2)
    {
        return 0;
    }
    else if (hierarchy1 < hierarchy2)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

void removeCard(Player *player, int card_index)
{
    if (card_index < 0 || card_index >= player->cards_count)
    {
        printf("Índice inválido para remoção de carta.\n");
        return;
    }

    for (int i = card_index; i < player->cards_count - 1; i++)
    {
        player->cards[i] = player->cards[i + 1];
    }

    player->cards_count--;

    player->cards = realloc(player->cards, player->cards_count * sizeof(Card));
    if (player->cards == NULL && player->cards_count > 0)
    {
        printf("Erro ao realocar memória para as cartas.\n");
        exit(1);
    }
}

void turnAction(Game *game, Turn *turn, bool is_player_1, Round *round, bool *break_while)
{
    if (round->is_challenge && round->last_player_challenger_id != game->current_player_id && !round->is_challenge_accepted)
    {
        int i = 0;
        if (round->point_value == 1)
        {
            printf("Você aceita o aumento de %d pontos para 3?\n", round->point_value);
            printf("1. Sim\n");
            printf("2. Não\n");
            scanf("%d", &i);
            if (i == 1)
            {
                round->is_challenge_accepted = true;
                round->point_value = 3;
                round->is_challenge = false;
            } else {
                round->is_challenge_accepted = false;
                round->point_value = 1;
                round->is_challenge = false;
                *break_while = true;
            }
        } else if (round->point_value == 3)
        {
            printf("Você aceita o aumento de %d pontos para 3?\n", round->point_value);
            printf("1. Sim\n");
            printf("2. Não\n");
            scanf("%d", &i);
            if (i == 1)
            {
                round->is_challenge_accepted = true;
                round->point_value = 6;
                round->is_challenge = false;
            } else {
                round->is_challenge_accepted = false;
                round->point_value = 3;
                round->is_challenge = false;
                *break_while = true;
            }
        } else if (round->point_value == 6)
        {
            printf("Você aceita o aumento de %d pontos para 3?\n", round->point_value);
            printf("1. Sim\n");
            printf("2. Não\n");
            scanf("%d", &i);
            if (i == 1)
            {
                round->is_challenge_accepted = true;
                round->point_value = 9;
                round->is_challenge = false;
            } else {
                round->is_challenge_accepted = false;
                round->point_value = 6;
                round->is_challenge = false;
                *break_while = true;
            }
        }
        
        return;
    }

    bool is_turn = true;
    bool removed_card = false;
    while (is_turn) {
        printf("Opção do turno:\n\n");

        if (!removed_card)
        {
            printf("1. Jogar carta\n");
        }
        
        printf("2. Terminar o turno\n");
        if (round->point_value != 9 && !round->is_challenge) {
            printf("3. Aumentar\n");
        }
    
        fflush(stdin); 
    
        char option = getchar();
        fflush(stdin); 
        switch (option)
        {
            case '1':
                printf("Selecione uma carta: \n");
    
                int i = 0;
    
                scanf("%d", &i);
    
                if (is_player_1)
                {
                    turn->player1_card = game->players[game->current_player_id].cards[0];
                }
                else
                {
                    turn->player2_card = game->players[game->current_player_id].cards[0];
                }
    
                removeCard(&game->players[game->current_player_id], i);
                removed_card = true;
                break;
            case '2':
                if (!removed_card)
                {
                    printf("Você não jogou nenhuma carta!\n");
                    break;
                } else {
                    printf("Terminando o turno...\n");
                    is_turn = false;
                }
                break;
            case '3':
                round->is_challenge = true;
                round->last_player_challenger_id = game->current_player_id;
                    printf("TRUCOOOOOOOO...\n");
                break;
            default:
                printf("Opção inválida!\n");
                break;
        }
    }
}

bool checkRoundWinner(Round *round, int last_turn)
{
    int first_turn_winner = round->turn[0].winner_id;
    int second_turn_winner = round->turn[1].winner_id;
    int third_turn_winner = round->turn[2].winner_id;

    if (last_turn == 3) {
        if (first_turn_winner == -1 && second_turn_winner == -1) {
            round->winner_id = -1;
            return true;
        } 
        if(first_turn_winner == -1) {
            round->winner_id = second_turn_winner;
            return true;
        }
        if(first_turn_winner == third_turn_winner && second_turn_winner != -1) {
            round->winner_id = first_turn_winner;
            return true;
        }
    } else if (last_turn == 2) {
        if (first_turn_winner == -1 && second_turn_winner != -1) {
            round->winner_id = second_turn_winner;
            return true;
        } else if(first_turn_winner == second_turn_winner && first_turn_winner != -1) {
            round->winner_id = first_turn_winner;
            return true;
        } else if (first_turn_winner == second_turn_winner && first_turn_winner == -1) {
            round->winner_id = -1;
            return true;
        }
    } 

    return false;
}