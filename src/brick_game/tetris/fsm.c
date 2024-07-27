#include "fsm.h"

static int tetrominoes[7][4][4] = {
    // I
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // O
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    },
    // T
    {
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    },
    // S
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0}
    },
    // Z
    {
        {0, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    },
    // J
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

void userInput(UserAction_t action, bool hold) {
    (void) hold;
    Tetris* tetris = get_tetris();

    switch (tetris->state)
    {
        case START:
            switch (action)
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
            
            break;
        case SPAWN:
            place_next(tetrominoes[1]);
            init_tetro(tetris);
            break;
        case MOVING:
            switch (action)
            {
                case Down:
                    move_down(&tetris->tetro);
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
                default:
                    break;
            }
            break;
        case SHIFTING:
            break;
        case ATTACHING:
            break;
        case GAMEOVER:
            break;
        default:
            break;
    }
}

void move_down(Tetromino *tetro) {
    tetro->y -= 1;
}

void move_left(Tetromino *tetro) {
    tetro->x -= 1;
}

void move_right(Tetromino *tetro) {
    tetro->x += 1;
}

void rotation(Tetromino *tetro) {
    (void) tetro;
}