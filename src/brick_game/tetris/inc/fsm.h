#ifndef FSM_H
#define FSM_H

#include "game.h"
#include "objects.h"

void moving_state(UserAction_t *action, bool *hold, int *is_down);
void start_state(UserAction_t *action);
void spawn_state();
void shifting_state(int *is_attached, int *is_down);
void attaching_state();
void pause_state(UserAction_t *action);

#endif