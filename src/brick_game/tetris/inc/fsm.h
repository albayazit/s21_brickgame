#ifndef SRC_BRICK_GAME_TETRIS_INC_FSM_H
#define SRC_BRICK_GAME_TETRIS_INC_FSM_H

#include "game.h"
#include "objects.h"

int get_random_tet();
void moving_state(UserAction_t *action, bool *hold, int *is_down);
void spawn_state();
void shifting_state(int *is_attached, int *is_down);
void attaching_state();
void pause_state(UserAction_t *action);
void updateGame();

#endif