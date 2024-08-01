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
    init_gui();
    init_windows(&windows);

    while (game_over()) {
        game_info = updateCurrentState();
        userInput(user_action, FALSE);
        draw_windows(&windows, &game_info);
        get_input(&user_action);
        usleep(10000);
    }
}

void get_input(UserAction_t *action) {
    switch (getch())
    {
    case '\n':
    case '\r':
    case KEY_ENTER:
        *action = Start;
        break;
    case 'p':
        *action = Pause;
        break;
    case KEY_LEFT:
        *action = Left;
        break;
    case KEY_RIGHT:
        *action = Right;
        break;
    case KEY_DOWN:
        *action = Down;
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
} 