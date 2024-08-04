#ifndef GUI_H
#define GUI_H

#include <ncurses.h>

#include "objects.h"

/**
 * @brief Structure representing the ncurses windows.
 */
typedef struct {
  WINDOW *game_win;
  WINDOW *next_fig_win;
  WINDOW *info_win;
} Windows;

void init_gui();
void draw_windows(Windows *windows, GameInfo_t *game, int is_game);
void init_windows(Windows *windows);
void draw_game_win(WINDOW *win, GameInfo_t *game);
void draw_figure_win(WINDOW *win, GameInfo_t *game);
void draw_info_win(WINDOW *win, GameInfo_t *game);
void draw_status(WINDOW *win, int status);
void draw_start(WINDOW *win);

#endif