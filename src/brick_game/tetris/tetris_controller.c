#ifndef SRC_BRICK_GAME_TETRIS_TETRIS_CONTROLLER_C
#define SRC_BRICK_GAME_TETRIS_TETRIS_CONTROLLER_C

#include "tetris_controller.h"

int tetrisControllerIsGameOver() { return game_over(); }

GameInfo_t tetrisControllerUpdateGame() {
  updateGame();
  GameInfo_t info = updateCurrentState();
  return info;
}

void tetrisControllerUserInput(UserAction_t *user_action, bool hold) {
  if (*user_action == Down) {
    hold = true;
  } else if (*user_action == Up) {
    *user_action = Down;
    hold = false;
  }
  userInput(*user_action, hold);
}

void tetrisControllerFreeMemory() { free_fields(); }

#endif