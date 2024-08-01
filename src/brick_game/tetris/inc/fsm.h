#ifndef FSM_H
#define FSM_H

#include "game.h"
#include <sys/time.h>
#include "objects.h"

void move_left(Tetromino *tetro);
void move_right(Tetromino *tetro);
void rotation(Tetromino *tetro);
int check_timeout();
int check_rotate_collision(int temp[4][4], int x, int y);
int is_y_collision(int shape[4][4], int y);
void place_tetro();
void clear_current_tetro();
int is_x_collision(int shape[4][4], int x);
void check_line_fill();
void move_lines(int height);
int is_game_over();

#endif