#include "main.h"
#include <locale.h>

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
    UserAction_t user_action;
    GameInfo_t game_info;
    init_gui();
    init_windows(&windows);

    while (game_over()) {
        game_info = updateCurrentState();
        get_input(&user_action);
        userInput(user_action, FALSE);
        draw_windows(&windows, &game_info);
    }
}

void get_input(UserAction_t *user_action) {
    switch (getch())
    {
    case '\n':
    case '\r':
    case KEY_ENTER:
        *user_action = Start;
        break;
    case 'p':
        *user_action = Pause;
        break;
    case KEY_LEFT:
        *user_action = Left;
        break;
    case KEY_RIGHT:
        *user_action = Right;
        break;
    case KEY_DOWN:
        *user_action = Down;
        break;
    case ' ':
        *user_action = Action;
        break;
    case 'q':
        *user_action = Terminate;
        break;
    default:
        break;
    }
} 