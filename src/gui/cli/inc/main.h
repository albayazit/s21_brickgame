#ifndef MAIN_H
#define MAIN_H

#include "game.h"
#include "gui.h"
#include <stdlib.h>
#include <sys/time.h>


void game_loop();
void get_input(UserAction_t *action, int *ch, bool *hold, int *start);


#endif