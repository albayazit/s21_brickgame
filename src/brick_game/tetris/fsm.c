#ifndef SRC_BRICK_GAME_TETRIS_FSM_C
#define SRC_BRICK_GAME_TETRIS_FSM_C

#include "fsm.h"

/**
 * @brief Array of Tetris tetromino shapes.
 *
 * This static array contains the representations of the 7 different types of
 * tetrominoes used in Tetris. Each tetromino is represented as a 4x4 grid,
 * where a value of 1 indicates the presence of a block, and a value of 0
 * indicates an empty space.
 *
 * The tetrominoes are defined as follows:
 * - Tetromino 0: I (straight piece)
 * - Tetromino 1: O (square piece)
 * - Tetromino 2: T (T-shaped piece)
 * - Tetromino 3: S (S-shaped piece)
 * - Tetromino 4: Z (Z-shaped piece)
 * - Tetromino 5: J (J-shaped piece)
 * - Tetromino 6: L (L-shaped piece)
 */
static int tetrominoes[7][4][4] = {
    // I +
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    // O +
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    // T +
    {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
    // S +
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
    // Z +
    {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    // J +
    {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
    // L
    {{0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}}};

/**
 * @brief Generates a random Tetris piece index.
 *
 * This function generates a random integer representing one of the Tetris
 * pieces (tetrominoes). The pieces are indexed from 0 to 7, and the function
 * ensures that the returned value is always within this range.
 *
 * @return An integer representing the index of a randomly selected Tetris
 * piece, ranging from 0 to 7.
 */
int get_random_tet() {
  int res = 0;
  res = rand() % 8 - 1;
  if (res < 0) res = 0;
  return res;
}

/**
 * @brief Handles user input actions in the Tetris game.
 *
 * This function processes the user input based on the current state of the
 * Tetris game. It changes the game state according to the user's actions such
 * as moving, rotating, pausing, or terminating the game.
 *
 * @param action The user action to be processed.
 * @param hold A boolean indicating whether the action is a hold action.
 */
void userInput(UserAction_t action, bool hold) {
  Tetris *tetris = get_tetris();
  tetris->is_down = 0;
  switch (tetris->state) {
    case START:
      if (action == Start) {
        tetris->state = SPAWN;
      }
      break;
    case MOVING:
      moving_state(&action, &hold, &tetris->is_down);
      break;
    case PAUSE:
      pause_state(&action);
      break;
    default:
      break;
  }
}

void updateGame() {
  Tetris *tetris = get_tetris();
  static int is_attached = 0;

  switch (tetris->state) {
    case SPAWN:
      spawn_state();
      break;
    case MOVING:
      if (check_timeout()) {
        tetris->state = SHIFTING;
      }
      break;
    case SHIFTING:
      shifting_state(&is_attached, &tetris->is_down);
      break;
    case ATTACHING:
      attaching_state();
      break;
    default:
      break;
  }
}

/**
 * @brief Handles the pause state in the Tetris game.
 *
 * This function processes the user's action while the game is paused.
 * If the action is to resume the game, it changes the state to MOVING.
 * If the action is to terminate the game, it changes the state to EXIT_STATE.
 *
 * @param action Pointer to the user action to be processed.
 */
void pause_state(UserAction_t *action) {
  Tetris *tetris = get_tetris();
  if (*action == Pause) {
    tetris->state = MOVING;
    tetris->info.pause = 0;
  } else if (*action == Terminate) {
    tetris->state = EXIT_STATE;
  }
}

/**
 * @brief Handles the attaching state in the Tetris game.
 *
 * This function finalizes the placement of the current tetromino.
 * It places the tetromino on the board, clears it, checks for line fills,
 * and updates the game state to either SPAWN or GAMEOVER based on the game's
 * status.
 */
void attaching_state() {
  Tetris *tetris = get_tetris();
  place_tetro();
  clear_current_tetro();
  check_line_fill();
  if (is_game_over()) {
    tetris->state = GAMEOVER;
  } else {
    tetris->state = SPAWN;
  }
}

/**
 * @brief Handles the shifting state in the Tetris game.
 *
 * This function manages the movement of the tetromino downwards or attaching it
 * to the board. It checks for collisions and updates the game state
 * accordingly.
 *
 * @param is_attached Pointer to an integer indicating if the tetromino is
 * attached.
 * @param is_down Pointer to an integer indicating if the tetromino has moved
 * down.
 */
void shifting_state(int *is_attached, int *is_down) {
  Tetris *tetris = get_tetris();
  if (*is_attached) {
    tetris->state = ATTACHING;
    *is_attached = 0;
  } else if (*is_down) {
    clear_tetro();
    while (is_y_collision(tetris->tetro.shape, tetris->tetro.y) != 1) {
      tetris->tetro.y += 1;
    }
    *is_down = 0;
    tetris->state = ATTACHING;
    plant_tetro();
  } else {
    clear_tetro();
    if (is_y_collision(tetris->tetro.shape, tetris->tetro.y)) {
      tetris->state = ATTACHING;
      plant_tetro();
      return;
    } else {
      tetris->tetro.y += 1;
    }
    plant_tetro();
    tetris->state = MOVING;
  }
}

/**
 * @brief Handles the spawn state in the Tetris game.
 *
 * This function initializes a new tetromino and places it on the board.
 * If it's the first spawn, it initializes the game tetromino.
 *
 * @note This function calls `place_next` twice to prepare the tetromino for
 * play.
 */
void spawn_state() {
  Tetris *tetris = get_tetris();
  if (tetris->is_start) {
    place_next(tetrominoes[get_random_tet()]);
    tetris->is_start = 0;
  }
  init_tetro(tetris);
  place_next(tetrominoes[get_random_tet()]);
  plant_tetro();
  tetris->state = MOVING;
}

/**
 * @brief Handles the moving state in the Tetris game.
 *
 * This function processes user actions when the tetromino is moving.
 * It allows the user to move left, right, down, rotate the tetromino,
 * pause, or terminate the game. It also checks for timeouts to automatically
 * shift the tetromino down.
 *
 * @param action Pointer to the user action to be processed.
 * @param hold Pointer to a boolean indicating if the down action is a hold.
 * @param is_down Pointer to an integer indicating if the tetromino has moved
 * down.
 */
void moving_state(UserAction_t *action, bool *hold, int *is_down) {
  Tetris *tetris = get_tetris();
  switch (*action) {
    case Down:
      if (*hold) {
        if (tetris->tetro.y <= 21) {
          tetris->state = SHIFTING;
        }
      } else {
        tetris->state = SHIFTING;
        *is_down = 1;
      }
      break;
    case Right:
      move_right(&tetris->tetro);
      break;
    case Left:
      move_left(&tetris->tetro);
      break;
    case Action:
      rotation(&tetris->tetro);
      break;
    case Terminate:
      tetris->state = EXIT_STATE;
      break;
    case Pause:
      tetris->info.pause = 1;
      tetris->state = PAUSE;
      break;
    default:
      break;
  }
}

#endif