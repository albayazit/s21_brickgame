#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdlib.h>
#include "objects.h"
#include <sys/time.h>
#include <stdio.h>

void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();
void init_game(Tetris *tetris);
int** allocate_field(int height, int width);
void game_loop();
void place_next(int shape[4][4]);
Tetris* get_tetris();
int game_over();
void init_tetro(Tetris *tetris);
void plant_tetro();
void clear_tetro();
GameState get_state();
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
void save_high_score(const char *filename, int high_score);
int load_high_score(const char *filename);

#endif