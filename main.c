#include <stdio.h>
#include <stdlib.h>
#define total_cards_per_player 3
#define total_players 2

typedef enum
{
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
} Type;

const char *type_names[] = {
    "Corações",
    "Diamantes",
    "Paus",
    "Espadas"
};

typedef struct
{
    Type type;
    int value;
} Card;

typedef struct
{
    int id;
    char name[50];
    int score;
    int cards_count;
    Card *cards;
} Player;

typedef struct
{
    Player *players;
    int current_player_id;
    int round_number;
    int turn_number;
} Game;

typedef struct
{
    int first_winner_id;    
    int second_winner_id;
    int third_winner_id;
} Round;

void startGame();
void startNewRound(Game *game);
Type getRandomType();
int getRandomValue();
void getCards(Game *game);
void printCards(Player *player);

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

    for (int i = 0; i < num_players; i++)
    {
        game->players[i].id = i + 1;
        game->players[i].score = 0;
        game->players[i].cards_count = 0;
        game->players[i].cards = NULL;

        printf("Coloque o nome do jogador %d: ", i + 1);
        scanf("%49s", game->players[i].name);
        system("clear");
    }

    startNewRound(game);

    free(game->players);
    free(game);
}

void startNewRound(Game *game)
{
    getCards(game);

    printf("Turno %d\nJogador %s\n", game->round_number, game->players[game->current_player_id].name);

    printCards(&game->players[game->current_player_id]);
}

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
    printf("Cartas do jogador %s:\n", player->name);
    for (int i = 0; i < player->cards_count; i++)
    {
        printf("Carta %d: Tipo %s, Valor %d\n", i + 1, getTypeString(player->cards[i].type), player->cards[i].value);
    }
}