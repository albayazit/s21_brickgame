#include "main.h"
#include <locale.h>
#include <unistd.h>

int main() {
    srand(time(0));
    setlocale(LC_ALL, "en_US.UTF-8");
    init_gui();
    game_loop();
    endwin();
    return 0;
}

void game_loop() {
    Windows windows;
    GameInfo_t game_info;
    UserAction_t user_action;
    bool hold = FALSE;
    int ch;
    int start = 1;
    init_gui();
    init_windows(&windows);
    while (game_over()) {
        game_info = updateCurrentState();
        if (start) {
            draw_start(windows.game_win);
        }
        if (game_info.pause == 1) {
            draw_status(windows.info_win, 1);
        } else {
            draw_status(windows.info_win, 0);
        }
        userInput(user_action, hold);
        draw_windows(&windows, &game_info, start);
        ch = getch();
        get_input(&user_action, &ch, &hold, &start);
        usleep(10000);
    }
    if (get_state() == GAMEOVER) {
        draw_status(windows.info_win, 2);
        draw_windows(&windows, &game_info, start);
        while (getch() == EOF) {}
    }
}

void get_input(UserAction_t *action, int *ch, bool *hold, int *start) {
    switch (*ch)
    {
    case '\n':
    case '\r':
    case KEY_ENTER:
        *action = Start;
        *start = 0;
        break;
    case 'p':
        *action = Pause;
        break;
    case KEY_LEFT:
        *action = Left;
        break;
    case KEY_DOWN:
        *action = Down;
        *hold = TRUE;
        break;
    case 'c':
        *action = Down;
        *hold = FALSE;
        break;
    case KEY_RIGHT:
        *action = Right;
        break;
    case ' ':
        *action = Action;
        break;
    case 'q':
        *action = Terminate;
        break;
    default:
        *action = Up;
        break;
    }
};