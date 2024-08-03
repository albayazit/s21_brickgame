#include "fsm.h"

static int tetrominoes[7][4][4] = {
    // I +
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // O +
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    },
    // T +
    {
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    },
    // S +
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0}
    },
    // Z +
    {
        {0, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    },
    // J +
    {
        {0, 0, 0, 0},
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0}
    },
    // L
    {
        {0, 0, 0, 0},
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0}
    }
};

int get_random_tet() {
    int res = 0;
    res = rand() % 8 - 1;
    if (res < 0) res = 0;
    return res;
}

void userInput(UserAction_t action, bool hold) {
    (void) hold;
    Tetris* tetris = get_tetris();

    static int is_attached = 0;
    static int is_down = 0;

    switch (tetris->state)
    {
        case START:
            start_state(&action);
            break;
        case SPAWN:
            spawn_state();
            break;
        case MOVING:
            moving_state(&action, &hold, &is_down);
            break;
        case SHIFTING:
            shifting_state(&is_attached, &is_down);
            break;
        case ATTACHING:
            attaching_state();
            break;
        case PAUSE:
            pause_state(&action);
            break;
        default:
            break;
    }
}

void pause_state(UserAction_t *action) {
    Tetris *tetris = get_tetris();
    if (*action == Pause) {
        tetris->state = MOVING;
        tetris->info.pause = 0;
    } else if (*action == Terminate) {
        tetris->state = EXIT_STATE;
    }
}

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

void start_state(UserAction_t *action) {
    Tetris *tetris = get_tetris();
    switch (*action)
    {
        case Start:
            tetris->state = SPAWN;
            break;
        case Terminate:
            tetris->state = EXIT_STATE;
            break;
        default:
            tetris->state = START;
            break;
    }
}

void moving_state(UserAction_t *action, bool *hold, int *is_down) {
    Tetris *tetris = get_tetris();
    switch (*action)
    {
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
    if (check_timeout()) {
        tetris->state = SHIFTING;
    }
}