#include "game.h"

/**
 * @brief Initializes the Tetris game state.
 *
 * This function initializes the game state by allocating memory for the game
 * field, setting the initial score and level, and loading the high score from a
 * file.
 *
 * @param tetris Pointer to the Tetris structure to be initialized.
 */
void init_game(Tetris *tetris) {
  tetris->info.field = allocate_field(FIELD_HEIGHT + 3, FIELD_WIDTH);
  tetris->info.next = allocate_field(4, 4);
  tetris->info.score = 0;
  tetris->info.high_score = load_high_score("db.txt");
  tetris->info.level = 1;
  tetris->info.pause = 0;
  tetris->info.speed = 1;

  tetris->is_start = 1;
}

void free_fields() {
  Tetris *tetris = get_tetris();
  for (int i = 0; i < FIELD_HEIGHT + 3; i++) {
    free(tetris->info.field[i]);
  }
  for (int j = 0; j < 4; j++) {
    free(tetris->info.next[j]);
  }
  free(tetris->info.field);
  free(tetris->info.next);
}

/**
 * @brief Initializes the current tetromino.
 *
 * This function sets the initial position of the current tetromino and copies
 * its shape from the next tetromino to the current tetromino.
 *
 * @param tetris Pointer to the Tetris structure containing the current
 * tetromino.
 */
void init_tetro(Tetris *tetris) {
  tetris->tetro.x = 3;
  tetris->tetro.y = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tetris->tetro.shape[i][j] = tetris->info.next[i][j];
    }
  }
}

/**
 * @brief Places the next tetromino shape into the next field.
 *
 * This function updates the next tetromino shape in the Tetris structure based
 * on the given shape.
 *
 * @param shape A 4x4 array representing the shape of the next tetromino.
 */
void place_next(int shape[4][4]) {
  Tetris *tetris = get_tetris();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tetris->info.next[i][j] = 0;
      if (shape[i][j] == 1) {
        tetris->info.next[i][j] = 1;
      }
    }
  }
}

/**
 * @brief Allocates memory for the game field.
 *
 * This function allocates a 2D array of integers for the game field,
 * initializing all elements to zero.
 *
 * @param height The height of the field to allocate.
 * @param width The width of the field to allocate.
 * @return A pointer to the allocated 2D array.
 */
int **allocate_field(int height, int width) {
  int **field = malloc(height * sizeof(int *));
  for (int i = 0; i < height; i++) {
    field[i] = calloc(width, sizeof(int));
  }
  return field;
}

/**
 * @brief Plants the current tetromino in the game field.
 *
 * This function updates the game field by placing the current tetromino shape
 * at its current position.
 */
void plant_tetro() {
  Tetris *tetris = get_tetris();

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetris->tetro.shape[i][j] == 1) {
        tetris->info.field[i + tetris->tetro.y][j + tetris->tetro.x] = 1;
      }
    }
  }
}

/**
 * @brief Clears the current tetromino from the game field.
 *
 * This function removes the current tetromino shape from the game field at its
 * current position.
 */
void clear_tetro() {
  Tetris *tetris = get_tetris();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetris->info.field[i + tetris->tetro.y][j + tetris->tetro.x] == 1 &&
          tetris->tetro.shape[i][j] == 1) {
        tetris->info.field[i + tetris->tetro.y][j + tetris->tetro.x] = 0;
      }
    }
  }
}

/**
 * @brief Checks for filled lines in the game field.
 *
 * This function checks for complete lines in the game field, clears them, and
 * updates the score accordingly. If the score exceeds the high score, it saves
 * the new high score to a file.
 */
void check_line_fill() {
  int count = 0;
  Tetris *tetris = get_tetris();
  int lines = 0;
  for (int i = 0; i < FIELD_HEIGHT + 1; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (tetris->info.field[i][j] == 1) {
        count++;
      }
    }
    if (count == 10) {
      for (int j = 0; j < FIELD_WIDTH; j++) {
        tetris->info.field[i][j] = 0;
      }
      move_lines(i);
      lines++;
    }
    count = 0;
  }
  if (lines == 1)
    tetris->info.score += 100;
  else if (lines == 2)
    tetris->info.score += 300;
  else if (lines == 3)
    tetris->info.score += 700;
  else if (lines == 4)
    tetris->info.score += 1500;

  if (tetris->info.score > tetris->info.high_score) {
    tetris->info.high_score = tetris->info.score;
    save_high_score("db.txt", tetris->info.high_score);
  }
  int new_level = tetris->info.score / 600 + 1;
  if ((new_level >= tetris->info.level)) {
    if (new_level > 10) new_level = 10;
    tetris->info.level = new_level;
    tetris->info.speed = new_level;
  }
}

/**
 * @brief Moves filled lines down in the game field.
 *
 * This function shifts all lines above the specified height down by one line,
 * effectively clearing the filled line at the specified height.
 *
 * @param height The height of the filled line to move down.
 */
void move_lines(int height) {
  Tetris *tetris = get_tetris();
  for (int i = height; i > 1; i--) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (tetris->info.field[i - 1][j] == 1) {
        tetris->info.field[i][j] = 1;
        tetris->info.field[i - 1][j] = 0;
      }
    }
  }
}

/**
 * @brief Clears the current tetromino shape.
 *
 * This function sets all elements of the current tetromino shape to zero,
 * effectively clearing it.
 */
void clear_current_tetro() {
  Tetris *tetris = get_tetris();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tetris->tetro.shape[i][j] = 0;
    }
  }
}

/**
 * @brief Places the current tetromino in the game field.
 *
 * This function plants the current tetromino in the game field based on its
 * shape and current position.
 */
void place_tetro() {
  Tetris *tetris = get_tetris();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetris->tetro.shape[i][j] == 1) {
        tetris->info.field[i + tetris->tetro.y][j + tetris->tetro.x] = 1;
      }
    }
  }
}

/**
 * @brief Checks if the current tetromino has timed out.
 *
 * This function checks if enough time has elapsed since the last tetromino
 * movement based on the current game level.
 *
 * @return 1 if the tetromino has timed out, 0 otherwise.
 */
int check_timeout() {
  int result = 0;
  Tetris *tetris = get_tetris();
  static struct timeval last_time;
  struct timeval current_time;
  gettimeofday(&current_time, NULL);
  if (last_time.tv_sec == 0 && last_time.tv_usec == 0) {
    last_time = current_time;
  }
  long elapsed_time = (current_time.tv_sec - last_time.tv_sec) * 1000000 +
                      (current_time.tv_usec - last_time.tv_usec);

  if (elapsed_time >= 1000000 - 80000 * tetris->info.level) {
    last_time = current_time;
    result = 1;
  }

  return result;  // Время еще не прошло
}

/**
 * @brief Moves the current tetromino to the left.
 *
 * This function attempts to move the given tetromino left by one position.
 * It first clears the tetromino from the game field, checks for
 * collisions, and if there are none, updates the tetromino's position.
 *
 * @param tetro Pointer to the Tetromino structure to be moved.
 */
void move_left(Tetromino *tetro) {
  clear_tetro();
  if (!is_x_collision(tetro->shape, tetro->x - 1)) {
    tetro->x -= 1;
  }
  plant_tetro();
}

/**
 * @brief Moves the current tetromino to the right.
 *
 * This function attempts to move the given tetromino right by one position.
 * It first clears the tetromino from the game field, checks for
 * collisions, and if there are none, updates the tetromino's position.
 *
 * @param tetro Pointer to the Tetromino structure to be moved.
 */
void move_right(Tetromino *tetro) {
  clear_tetro();
  if (!is_x_collision(tetro->shape, tetro->x + 1)) {
    tetro->x += 1;
  }
  plant_tetro();
}

/**
 * @brief Rotates the current tetromino.
 *
 * This function attempts to rotate the given tetromino. It first clears
 * the tetromino from the game field, generates a temporary shape for
 * the rotated tetromino, checks for collisions, and if there are none,
 * updates the tetromino's shape.
 *
 * @param tetro Pointer to the Tetromino structure to be rotated.
 */
void rotation(Tetromino *tetro) {
  int temp[4][4];
  clear_tetro();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      temp[j][4 - 1 - i] = tetro->shape[i][j];
    }
  }
  if (check_rotate_collision(temp, tetro->x, tetro->y)) {
    // Копирование временной матрицы обратно в исходную
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        tetro->shape[i][j] = temp[i][j];
      }
    }
  }
  plant_tetro();
}

/**
 * @brief Checks for collision when rotating the tetromino.
 *
 * This function checks if rotating the tetromino to a temporary shape would
 * result in a collision with the walls or other blocks in the game field.
 *
 * @param temp The temporary shape of the tetromino after rotation.
 * @param x The x-coordinate of the tetromino in the game field.
 * @param y The y-coordinate of the tetromino in the game field.
 * @return 1 if there is no collision, 0 if there is a collision.
 */
int check_rotate_collision(int temp[4][4], int x, int y) {
  int result = 1;
  Tetris *tetris = get_tetris();
  if (is_x_collision(temp, x)) {
    result = 0;
  } else if (is_y_collision(temp, y)) {
    result = 0;
  } else {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if ((temp[i][j] == 1 && tetris->info.field[i + y][j + x] == 1) ||
            tetris->tetro.y == 0) {
          result = 0;
          break;
        }
      }
    }
  }
  return result;
}

/**
 * @brief Checks for collision with the bottom of the game field.
 *
 * This function checks if the current tetromino shape collides with the bottom
 * of the game field or another block in the game field.
 *
 * @param shape The current shape of the tetromino.
 * @param y The y-coordinate of the tetromino in the game field.
 * @return 1 if there is a collision, 0 otherwise.
 */
int is_y_collision(int shape[4][4], int y) {
  int result = 0;
  Tetris *tetris = get_tetris();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (shape[i][j] == 1 &&
          (y + i >= 20 || tetris->info.field[y + i + 1][tetris->tetro.x + j])) {
        result = 1;
        break;
      }
    }
  }
  return result;
}

/**
 * @brief Checks for collision with the sides of the game field.
 *
 * This function checks if the current tetromino shape collides with the left
 * or right walls of the game field or another block in the game field.
 *
 * @param shape The current shape of the tetromino.
 * @param x The x-coordinate of the tetromino in the game field.
 * @return 1 if there is a collision, 0 otherwise.
 */
int is_x_collision(int shape[4][4], int x) {
  int result = 0;
  Tetris *tetris = get_tetris();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (shape[i][j] == 1 &&
          (j + x > 9 || j + x < 0 ||
           tetris->info.field[i + tetris->tetro.y][j + x] == 1)) {
        result = 1;
        break;
      }
    }
  }
  return result;
}

/**
 * @brief Updates the current game state.
 *
 * This function retrieves the current game state from the Tetris structure.
 *
 * @return The current game information.
 */
GameInfo_t updateCurrentState() { return get_tetris()->info; }

/**
 * @brief Gets the current game state.
 *
 * This function retrieves the current game state from the Tetris structure.
 *
 * @return The current game state.
 */
GameState get_state() {
  Tetris *tetris = get_tetris();
  return tetris->state;
}

/**
 * @brief Gets the pointer to the Tetris structure.
 *
 * This function returns a pointer to the static Tetris structure,
 * initializing it if necessary.
 *
 * @return Pointer to the Tetris structure.
 */
Tetris *get_tetris() {
  static Tetris tetris;
  if (tetris.info.field == NULL) {
    init_game(&tetris);
    tetris.state = START;
  }
  return &tetris;
}

/**
 * @brief Checks if the game is over.
 *
 * This function checks if the game is over by determining if there are any
 * blocks in the first row of the game field.
 *
 * @return 1 if the game is over, 0 otherwise.
 */
int is_game_over() {
  int result = 0;
  Tetris *tetris = get_tetris();
  for (int i = 0; i < FIELD_WIDTH; i++) {
    if (tetris->info.field[1][i] == 1) {
      result = 1;
      break;
    }
  }
  return result;
}

/**
 * @brief Checks if the game is currently in a game over state.
 *
 * This function checks the state of the game and determines if it is
 * in the GAMEOVER or EXIT_STATE.
 *
 * @return 1 if the game is still active, 0 otherwise.
 */
int game_over() {
  int flag = 1;
  Tetris *tetris = get_tetris();
  if (tetris->state == GAMEOVER || tetris->state == EXIT_STATE) {
    flag = 0;
  }
  return flag;
}

/**
 * @brief Saves the high score to a file.
 *
 * This function writes the provided high score to a specified file.
 *
 * @param filename The name of the file to save the high score to.
 * @param high_score The high score to be saved.
 */
void save_high_score(const char *filename, int high_score) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Unable to open file for writing");
    return;
  }
  fprintf(file, "%d\n", high_score);
  fclose(file);
}

/**
 * @brief Loads the high score from a file.
 *
 * This function reads the high score from a specified file. If the file
 * cannot be opened, it returns 0.
 *
 * @param filename The name of the file to load the high score from.
 * @return The loaded high score, or 0 if the file cannot be opened.
 */
int load_high_score(const char *filename) {
  int high_score = 0;
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    high_score = 0;  // Если файл не найден, возвращаем 0
  } else {
    fscanf(file, "%d", &high_score);
  }
  fclose(file);
  return high_score;
}