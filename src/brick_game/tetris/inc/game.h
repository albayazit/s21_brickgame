#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "fsm.h"
#include "objects.h"

void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();

void init_game(Tetris *tetris);
int** allocate_field(int height, int width);
void game_loop();
void place_next(int shape[4][4]);
Tetris* get_tetris();
int game_over();
void init_tetro(Tetris *tetris);

#endif