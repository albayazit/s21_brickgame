#include "main.h"
#include <locale.h>

int main() {
    int status = EXIT_SUCCESS;
    setlocale(LC_ALL, "en_US.UTF-8");
    init_gui();
    init_windows();


    getch();
    endwin();

    return status;
}


void init_gui() {
    initscr(); // отключение отображения вводимых символов
    noecho(); // ввод без буфера
    cbreak(); // скрытие курсора
    curs_set(0);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK); 
    keypad(stdscr, TRUE);
}


void init_windows() {
    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width);
    int center_y = (term_height - FIELD_HEIGHT) / 2;
    int center_x = ((term_width - FIELD_WIDTH) / 2);

    WINDOW *game_win = newwin(FIELD_HEIGHT + 2, FIELD_WIDTH * 2 + 2, center_y, center_x - 14);
    WINDOW *next_fig_win = newwin(9, 18, center_y, center_x + 9);
    WINDOW *info_win = newwin(13, 18, center_y + 9, center_x + 9);
    refresh();
    draw_game_win(game_win);
    draw_figure_win(next_fig_win);
    draw_info_win(info_win);
    wrefresh(next_fig_win);
    wrefresh(info_win);
    wrefresh(game_win);
}

void draw_game_win(WINDOW *win) {
    wbkgd(win, COLOR_PAIR(1));
    for (int row = 0; row < FIELD_HEIGHT; row++) {
        for (int col = 0; col < FIELD_WIDTH; col++) {
            mvwprintw(win, row + 1, 2 * col + 1, "%s", "█");
            mvwprintw(win, row + 1, 2 * col + 2, "%s", "█");
        }
    }
    box(win, 0, 0);
}

void draw_figure_win(WINDOW *win) {
    wbkgd(win, COLOR_PAIR(1));
    box(win, 0, 0);
}

void draw_info_win(WINDOW *win) {
    wbkgd(win, COLOR_PAIR(1));
    mvwprintw(win, 3, 2, "%s", "SCORE |");
    mvwprintw(win, 3, 10, "%s", "1");

    mvwprintw(win, 5, 2, "%s", "LEVEL |");
    mvwprintw(win, 5, 10, "%s", "7");

    mvwprintw(win, 7, 2, "%s", "SPEED |");
    mvwprintw(win, 7, 10, "%s", "1");

    mvwprintw(win, 9, 2, "%s", "RECORD |");
    mvwprintw(win, 9, 11, "%s", "9999");

    box(win, 0, 0);
}

