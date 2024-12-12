#include "main.h"

/**
 * @brief Основная функция программы.
 *
 * Инициализирует окна, GUI, запускает главное меню, завершает GUI перед
 * выходом.
 *
 * @return Код завершения программы.
 */
int main() {
  Windows *windows = getWindows();
  srand(time(0));
  setlocale(LC_ALL, "en_US.UTF-8");
  init_gui();
  init_windows(windows);
  show_menu(windows->game_win);
  endwin();
  return 0;
}

/**
 * @brief Отображает главное меню программы.
 *
 * Обрабатывает ввод пользователя для выбора игры или выхода из программы.
 *
 * @param win Указатель на окно для отображения меню.
 */
void show_menu(WINDOW *win) {
  draw_menu(win);
  int input;
  while (input != 'q') {
    input = getch();
    if (input == '1') {
      runTetrisGame();
      break;
    } else if (input == '2') {
      runSnakeGame();
      break;
    }
  }
}

/**
 * @brief Запуск игры Tetris.
 *
 * Выполняет игровой цикл для Tetris, обрабатывая ввод пользователя,
 * обновляя состояние игры и отображая окна с игрой.
 */
void runTetrisGame() {
  Windows *windows = getWindows();
  GameInfo_t game_info;
  UserAction_t action;
  bool hold = FALSE;
  int input;
  int start = 1;
  while (tetrisControllerIsGameOver()) {
    game_info = tetrisControllerUpdateGame();
    if (start) {
      draw_start_win(windows->game_win);
    } else {
      draw_game_win(windows->game_win, &game_info);
    }
    draw_figure_win(windows->next_fig_win, &game_info);
    draw_info_win(windows->info_win, &game_info);
    if (game_info.pause == 1) {
      draw_status(windows->info_win, 1);
    } else {
      draw_status(windows->info_win, 0);
    }
    if (get_input(&action)) {
      tetrisControllerUserInput(&action, hold);
      if (action == Start) {
        start = 0;
      } else if (action == Terminate) {
        break;
      }
    }
    usleep(10000);
  }
  if (!tetrisControllerIsGameOver()) {
    draw_gameover_win(windows->game_win);
    while (getch() == EOF) {
    }
  }
  tetrisControllerFreeMemory();
}

/**
 * @brief Запуск игры Snake.
 *
 * Выполняет игровой цикл для Snake, обрабатывая ввод пользователя,
 * обновляя состояние игры и отображая окна с игрой.
 */
void runSnakeGame() {
  SnakeControllerWrapper *controller = createSnakeController();
  Windows *windows = getWindows();
  GameInfo_t state = snakeControllerUpdateState(controller);
  draw_start_win(windows->game_win);
  while (!snakeControllerIsGameOver(controller) &&
         !snakeControllerIsWinner(controller)) {
    UserAction_t action;
    if (get_input(&action)) {
      snakeControllerUserInput(controller, action, FALSE);
    }
    if (snakeControllerIsPlaying(controller)) {
      state = snakeControllerUpdateState(controller);
      draw_game_win(windows->game_win, &state);
    }
    if (action == Terminate) {
      break;
    }
    draw_info_win(windows->info_win, &state);
    if (state.pause == 1) {
      draw_status(windows->info_win, 1);
    } else {
      draw_status(windows->info_win, 0);
    }
    usleep(10000);
  }
  if (snakeControllerIsWinner(controller)) {
    draw_winner_win(windows->game_win);
  } else {
    draw_gameover_win(windows->game_win);
  }
  while (getch() == EOF) {
  }
  destroySnakeController(controller);
}

/**
 * @brief Получает пользовательский ввод и преобразует его в действие.
 *
 * @param action Указатель на переменную типа UserAction_t для хранения
 * результата.
 * @return Возвращает true, если был введен допустимый символ, иначе false.
 */
bool get_input(UserAction_t *action) {
  int act = getch();
  switch (act) {
    case '\n':
    case '\r':
    case KEY_ENTER:
      *action = Start;
      return true;
    case 'p':
      *action = Pause;
      return true;
    case KEY_UP:
      *action = Up;
      return true;
    case KEY_LEFT:
      *action = Left;
      return true;
    case KEY_DOWN:
      *action = Down;
      return true;
    case KEY_RIGHT:
      *action = Right;
      return true;
    case ' ':
      *action = Action;
      return true;
    case 'q':
      *action = Terminate;
      return true;
    default:
      return false;  // Нет ввода или неподдерживаемый символ
  }
}