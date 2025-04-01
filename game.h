typedef enum
{
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
} Type;

const char *type_names[] = {
    "Copas",
    "Diamantes",
    "Paus",
    "Espadas"};

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
    int winner_id;
    Card player1_card;
    Card player2_card;
} Turn;

typedef struct
{
    int winner_id;
    bool is_challenge;
    int last_player_challenger_id;
    bool is_challenge_accepted;
    int point_value;
    Turn turn[3];
} Round;

void startGame();
void startNewRound(Game *game);
Type getRandomType();
int getRandomValue();
void getCards(Game *game);
void printCards(Player *player);
int getCardHierarchy(Card card);
int compareCards(Card card1, Card card2);
void endRound(Game *game, int winner_id, int point_value);
void turnAction(Game *game, Turn *turn, bool is_player_1, Round *round, bool *break_while);
void startATurn(Game *game, Round *round, Turn *turn, bool *break_while);
void removeCard(Player *player, int card_index);
bool checkRoundWinner(Round *round, int last_turn);
void showLastPlayerCard(Round *round, int last_turn, int player_id);