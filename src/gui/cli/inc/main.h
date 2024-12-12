#ifndef SRC_GUI_CLI_INC_MAIN_H
#define SRC_GUI_CLI_INC_MAIN_H

#include <locale.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "gui.h"
#include "snake_controller_wrapper_c.h"
#include "tetris_controller.h"

void show_menu(WINDOW* win);
bool get_input(UserAction_t* action);
void runSnakeGame();
void runTetrisGame();

#endif