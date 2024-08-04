#include "gui.h"

/**
 * @brief Initializes the graphical user interface.
 *
 * This function sets up the ncurses library for displaying the game.
 * It configures input settings, starts color support, and initializes
 * keyboard input handling.
 */
void init_gui() {
  initscr();  // отключение отображения вводимых символов
  noecho();  // ввод без буфера
  cbreak();  // скрытие курсора
  curs_set(0);
  start_color();
  nodelay(stdscr, TRUE);
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  keypad(stdscr, TRUE);
}

/**
 * @brief Draws the game windows.
 *
 * This function renders the game windows based on the current game state.
 *
 * @param windows Pointer to the Windows structure containing window pointers.
 * @param game Pointer to the GameInfo_t structure containing game data.
 * @param is_game Flag indicating if the game is in progress (1) or not (0).
 */
void draw_windows(Windows *windows, GameInfo_t *game, int is_game) {
  if (is_game == 0) draw_game_win(windows->game_win, game);
  draw_figure_win(windows->next_fig_win, game);
  draw_info_win(windows->info_win, game);
  refresh();
  wrefresh(windows->next_fig_win);
  wrefresh(windows->info_win);
  wrefresh(windows->game_win);
}

/**
 * @brief Initializes the game windows.
 *
 * This function creates and configures the windows for the game,
 * next figure, and information display based on terminal size.
 *
 * @param windows Pointer to the Windows structure to be initialized.
 */
void init_windows(Windows *windows) {
  int term_height, term_width;
  getmaxyx(stdscr, term_height, term_width);
  int center_y = (term_height - FIELD_HEIGHT) / 2;
  int center_x = ((term_width - FIELD_WIDTH) / 2);

  windows->game_win =
      newwin(FIELD_HEIGHT + 2, FIELD_WIDTH * 2 + 2, center_y, center_x - 14);
  windows->next_fig_win = newwin(9, 18, center_y, center_x + 9);
  windows->info_win = newwin(13, 18, center_y + 9, center_x + 9);
}

/**
 * @brief Draws the game window.
 *
 * This function renders the game grid and the current tetromino blocks.
 *
 * @param win Pointer to the game window.
 * @param game Pointer to the GameInfo_t structure containing game data.
 */
void draw_game_win(WINDOW *win, GameInfo_t *game) {
  wbkgd(win, COLOR_PAIR(1));
  for (int row = 0; row < FIELD_HEIGHT + 1; row++) {
    for (int col = 0; col < FIELD_WIDTH; col++) {
      if (game->field[row][col] == 1) {
        mvwprintw(win, row, 2 * col + 1, "%s", "█");  // +1 - границы
        mvwprintw(win, row, 2 * col + 2, "%s", "█");
      } else {
        mvwprintw(win, row, 2 * col + 1, "%s", " ");
        mvwprintw(win, row, 2 * col + 2, "%s", " ");
      }
    }
  }
  box(win, 0, 0);
}

/**
 * @brief Draws the game status message.
 *
 * This function displays the current status of the game (e.g., "Tetris",
 * "Pause", "Game Over").
 *
 * @param win Pointer to the window where the status will be displayed.
 * @param status The current status of the game (0: Tetris, 1: Pause, 2: Game
 * Over).
 */
void draw_status(WINDOW *win, int status) {
  switch (status) {
    case 0:
      mvwprintw(win, 2, 3, "%s", "🎮 Tetris 🎮");
      break;
    case 1:
      mvwprintw(win, 2, 3, "%s", "              ");
      mvwprintw(win, 2, 6, "%s", "Pause");
      break;
    case 2:
      mvwprintw(win, 2, 3, "%s", "              ");
      mvwprintw(win, 2, 4, "%s", "Game Over");
      break;
    default:
      break;
  }
}

/**
 * @brief Draws the starting screen.
 *
 * This function displays the instructions and controls on the starting screen.
 *
 * @param win Pointer to the window where the start screen will be displayed.
 */
void draw_start(WINDOW *win) {
  wbkgd(win, COLOR_PAIR(1));
  mvwprintw(win, 3, 5, "%s", "TETRIS GAME");
  mvwprintw(win, 4, 2, "%s", "-----------------");
  mvwprintw(win, 5, 2, "%s", "Control:");
  mvwprintw(win, 6, 2, "%s", "-----------------");
  mvwprintw(win, 7, 2, "%s", "Enter - start");
  mvwprintw(win, 8, 2, "%s", "Space - rotation");
  mvwprintw(win, 9, 2, "%s", "p - pause");
  mvwprintw(win, 10, 2, "%s", "c - quickly down");
  mvwprintw(win, 11, 2, "%s", "← - to left");
  mvwprintw(win, 12, 2, "%s", "→ - to right");
  mvwprintw(win, 13, 2, "%s", "↓ - slow down");
  mvwprintw(win, 14, 2, "%s", "q - terminate");
  box(win, 0, 0);
}

/**
 * @brief Draws the next figure window.
 *
 * This function renders the next tetromino shape in the game.
 *
 * @param win Pointer to the next figure window.
 * @param game Pointer to the GameInfo_t structure containing game data.
 */
void draw_figure_win(WINDOW *win, GameInfo_t *game) {
  wbkgd(win, COLOR_PAIR(1));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->next[i][j] == 1) {
        mvwprintw(win, i + 3, 2 * j + 5, "%s", "█");
        mvwprintw(win, i + 3, 2 * j + 6, "%s", "█");
      } else {
        mvwprintw(win, i + 3, 2 * j + 5, "%s", " ");
        mvwprintw(win, i + 3, 2 * j + 6, "%s", " ");
      }
    }
  }
  box(win, 0, 0);
}

/**
 * @brief Draws the information window.
 *
 * This function displays the current game statistics such as score, level,
 * speed, and high score.
 *
 * @param win Pointer to the information window.
 * @param game Pointer to the GameInfo_t structure containing game data.
 */
void draw_info_win(WINDOW *win, GameInfo_t *game) {
  wbkgd(win, COLOR_PAIR(1));
  mvwprintw(win, 4, 2, "%s", "SCORE |");
  mvwprintw(win, 4, 10, "%d", game->score);

  mvwprintw(win, 6, 2, "%s", "LEVEL |");
  mvwprintw(win, 6, 10, "%d", game->level);

  mvwprintw(win, 8, 2, "%s", "SPEED |");
  mvwprintw(win, 8, 10, "%d", game->speed);

  mvwprintw(win, 10, 2, "%s", "RECORD |");
  mvwprintw(win, 10, 11, "%d", game->high_score);
  box(win, 0, 0);
}