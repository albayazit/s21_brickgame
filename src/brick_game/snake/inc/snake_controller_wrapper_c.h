#ifndef SRC_BRICK_GAME_SNAKE_INC_SNAKE_CONTROLLER_WRAPPER_C_H
#define SRC_BRICK_GAME_SNAKE_INC_SNAKE_CONTROLLER_WRAPPER_C_H

#include <stdbool.h>

#include "brickgame_api.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SnakeControllerWrapper SnakeControllerWrapper;
SnakeControllerWrapper* createSnakeController();
void destroySnakeController(SnakeControllerWrapper* controller);
void snakeControllerUserInput(SnakeControllerWrapper* controller,
                              UserAction_t action, bool hold);
GameInfo_t snakeControllerUpdateState(SnakeControllerWrapper* controller);
int snakeControllerIsGameOver(SnakeControllerWrapper* controller);
int snakeControllerIsPlaying(SnakeControllerWrapper* controller);
int snakeControllerIsWinner(SnakeControllerWrapper* controller);

#ifdef __cplusplus
}
#endif

#endif