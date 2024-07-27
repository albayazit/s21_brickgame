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
            place_next(tetrominoes[get_random_tet()]);
            init_tetro(tetris);
            tetris->state = MOVING;
            break;
        case MOVING:
            if (check_timeout()) {
                tetris->state = SHIFTING;
            }

            // switch (action)
            // {
            //     case Down:
            //         move_down(&tetris->tetro);
            //         break;
            //     case Right:
            //         move_right(&tetris->tetro);
            //         break;
            //     case Left:
            //         move_left(&tetris->tetro);
            //         break;
            //     case Action:
            //         rotation(&tetris->tetro);
            //         break;
            //     case Terminate:
            //         tetris->state = EXIT_STATE;
            //         break;
            //     default:
            //         break;
            // }
            break;
        case SHIFTING:
            plant_tetro();
            tetris->tetro.y += 1;
            tetris->state = MOVING;
            break;
        case ATTACHING:
            break;
        case GAMEOVER:
            break;
        default:
            break;
    }
}

int check_timeout() {
    static struct timeval last_time;
    struct timeval current_time;

    // Получаем текущее время
    gettimeofday(&current_time, NULL);

    // Если last_time не инициализирована, инициализируем ее
    if (last_time.tv_sec == 0 && last_time.tv_usec == 0) {
        last_time = current_time; // Инициализация last_time
        return 0; // Время еще не прошло
    }

    // Вычисляем разницу во времени
    long elapsed_time = (current_time.tv_sec - last_time.tv_sec) * 1000000 + 
                        (current_time.tv_usec - last_time.tv_usec);

    // Если прошло больше 1 секунды, обновляем last_time и возвращаем 1
    if (elapsed_time >= 1000000) {
        last_time = current_time; // Обновляем last_time
        return 1; // Прошла 1 секунда
    }

    return 0; // Время еще не прошло
}

void move_down(Tetromino *tetro) {
    tetro->y -= 1;
    plant_tetro();
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