#ifndef FSM_H
#define FSM_H

#include "game.h"
#include "objects.h"

void move_down(Tetromino *tetro);
void move_left(Tetromino *tetro);
void move_right(Tetromino *tetro);
void rotation(Tetromino *tetro);

#endif