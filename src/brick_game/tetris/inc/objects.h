#ifndef SRC_BRICK_GAME_TETRIS_INC_OBJECTS_H
#define SRC_BRICK_GAME_TETRIS_INC_OBJECTS_H

#define FIELD_HEIGHT 20
#define FIELD_WIDTH 10
#define INFO_HEIGHT 20
#define INFO_WIDTH 50

#include "brickgame_api.h"

/**
 * @brief Enum representing the different states of the game.
 */
typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
  PAUSE,
  EXIT_STATE
} TetrisState;

/**
 * @brief Structure representing a tetromino.
 */
typedef struct {
  int x, y;  // Позиция фигуры
  int shape[4][4];  // Массив, представляющий форму фигуры
} Tetromino;

/**
 * @brief Structure representing the overall Tetris game state.
 */
typedef struct {
  GameInfo_t info;
  TetrisState state;
  Tetromino tetro;
  int is_start;
  int is_down;
} Tetris;

#endif