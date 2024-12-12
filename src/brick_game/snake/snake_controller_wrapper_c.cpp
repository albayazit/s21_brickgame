#include "snake_controller_wrapper_c.h"

#include "snake_controller.h"

using namespace s21;

struct SnakeControllerWrapper {
  SnakeController controller;
};

extern "C" {

SnakeControllerWrapper* createSnakeController() {
  return new SnakeControllerWrapper();
}

void destroySnakeController(SnakeControllerWrapper* controller) {
  delete controller;
}

void snakeControllerUserInput(SnakeControllerWrapper* controller,
                              UserAction_t action, bool hold) {
  if (controller) {
    controller->controller.userInput(action, hold);
  }
}

GameInfo_t snakeControllerUpdateState(SnakeControllerWrapper* controller) {
  if (controller) {
    return controller->controller.updateCurrentState();
  }
  GameInfo_t temp = {};
  return temp;
}

int snakeControllerIsGameOver(SnakeControllerWrapper* controller) {
  if (controller) {
    return controller->controller.isGameOver();
  }
  return 0;
}

int snakeControllerIsWinner(SnakeControllerWrapper* controller) {
  if (controller) {
    return controller->controller.isWinner();
  }
  return 0;
}

int snakeControllerIsPlaying(SnakeControllerWrapper* controller) {
  if (controller) {
    return controller->controller.isPlaying();
  }
  return 0;
}
}