#ifndef OBJECTS_H
#define OBJECTS_H

#define FIELD_HEIGHT 20
#define FIELD_WIDTH 10
#define INFO_HEIGHT 20
#define INFO_WIDTH 50

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
} GameState;

/**
 * @brief Enum representing user actions during the game.
 */
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

/**
 * @brief Structure containing game information.
 */
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

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
  GameState state;
  Tetromino tetro;
  int is_start;
} Tetris;

#endif