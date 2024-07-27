#ifndef OBJECTS_H
#define OBJECTS_H


#define FIELD_HEIGHT 20
#define FIELD_WIDTH 10
#define INFO_HEIGHT 20
#define INFO_WIDTH 50

#define RANDOM_TETRO rand()%7

typedef enum
{
    START = 0,
    SPAWN,
    MOVING,
    SHIFTING,
    ATTACHING,
    GAMEOVER,
    EXIT_STATE
} GameState;

typedef enum {
    Start,
    Pause,
    Terminate,
    Left,
    Right,
    Up,
    Down,
    Action
} UserAction_t;

typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;

typedef struct {
    int x, y; // Позиция фигуры
    int shape[4][4]; // Массив, представляющий форму фигуры
} Tetromino;

typedef struct {
    GameInfo_t info;
    GameState state;
    Tetromino tetro;
} Tetris;

#endif