#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <sys/time.h>

#include "game.h"
#include "gui.h"

void game_loop();
void get_input(UserAction_t *action, int *ch, bool *hold, int *start);

#endif