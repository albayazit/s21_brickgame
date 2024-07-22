#ifndef GAME_H
#define GAME_H

#include <stdbool.h>


#define FIELD_HEIGHT 20
#define FIELD_WIDTH 10
#define INFO_HEIGHT 20
#define INFO_WIDTH 50


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

void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();


#endif