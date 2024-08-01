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
            if (tetris->is_start) {
                place_next(tetrominoes[get_random_tet()]);
                tetris->is_start = 0;
            }
            init_tetro(tetris);
            place_next(tetrominoes[get_random_tet()]);
            plant_tetro();
            tetris->state = MOVING;
            break;
        case MOVING:
            switch (action)
            {
                case Down:
                    tetris->state = SHIFTING;
                    is_down = 1;
                    break;
                case Right:
                    move_right(&tetris->tetro);
                    break;
                case Left:
                    clear_tetro();
                    move_left(&tetris->tetro);
                    plant_tetro();
                    break;
                case Action:
                    clear_tetro();
                    rotation(&tetris->tetro);
                    plant_tetro();
                    break;
                case Terminate:
                    tetris->state = EXIT_STATE;
                    break;
                default:
                    break;
            }
            if (check_timeout()) {
                tetris->state = SHIFTING;
            }
            break;
        case SHIFTING:
            if (is_attached) {
                tetris->state = ATTACHING;
                is_attached = 0;
            } else if (is_down) {
                clear_tetro();
                while (is_y_collision(tetris->tetro.shape, tetris->tetro.y) != 1) {
                    tetris->tetro.y += 1;
                }
                is_down = 0;
                tetris->state = ATTACHING;
                plant_tetro();
            } else {
                clear_tetro();
                if (is_y_collision(tetris->tetro.shape, tetris->tetro.y)) {
                    tetris->state = ATTACHING;
                    plant_tetro();
                    break;
                } else {
                    tetris->tetro.y += 1;
                }
                plant_tetro();
                tetris->state = MOVING;
            }
            break;
        case ATTACHING:
            place_tetro();
            clear_current_tetro();
            check_line_fill();
            if (is_game_over()) {
                tetris->state = GAMEOVER;
            } else {
                tetris->state = SPAWN;
            }
            break;
        case GAMEOVER:
            exit(0);
            break;
        default:
            break;
    }
}

int is_game_over() {
    int result = 0;
    Tetris *tetris = get_tetris();
    for (int i = 0; i < FIELD_WIDTH; i++) {
        if (tetris->info.field[1][i] == 1) {
            result = 1;
            break;
        }
    }
    return result;
}

void check_line_fill() {
    int count = 0;
    Tetris *tetris = get_tetris();
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (tetris->info.field[i][j] == 1) {
                count++;
            }
        }
        if (count == 10) {
            for (int j = 0; j < FIELD_WIDTH; j++) {
                tetris->info.field[i][j] = 0;
            }
            move_lines(i);
        }
        count = 0;
    }
}

void move_lines(int height) {
    Tetris *tetris = get_tetris();
    for (int i = height; i > 1; i--) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (tetris->info.field[i - 1][j] == 1) {
                tetris->info.field[i][j] = 1;
                tetris->info.field[i - 1][j] = 0;
            }
        }
    }
}

void clear_current_tetro() {
    Tetris *tetris = get_tetris();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tetris->tetro.shape[i][j] = 0;
        }
    }
}

void place_tetro() {
    Tetris *tetris = get_tetris();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetris->tetro.shape[i][j] == 1) {
                tetris->info.field[i + tetris->tetro.y][j + tetris->tetro.x] = 1;
            }
        }
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

void move_left(Tetromino *tetro) {
    clear_tetro();
    if (!is_x_collision(tetro->shape, tetro->x - 1)) {
        tetro->x -= 1;
    }
    plant_tetro();
}

void move_right(Tetromino *tetro) {
    clear_tetro();
    if (!is_x_collision(tetro->shape, tetro->x + 1)) {
        tetro->x += 1;
    }
    plant_tetro();
}

void rotation(Tetromino *tetro) {
    int temp[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[j][4 - 1 - i] = tetro->shape[i][j];
        }
    }
    if (check_rotate_collision(temp, tetro->x, tetro->y)) {
        // Копирование временной матрицы обратно в исходную
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                tetro->shape[i][j] = temp[i][j];
            }
        }
    }
}

int check_rotate_collision(int temp[4][4], int x, int y) {
    int result = 1;
    Tetris *tetris = get_tetris();
    if (is_x_collision(temp, x)) {
        result = 0;
    } else if (is_y_collision(temp, y)) {
        result = 0;
    } else {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (temp[i][j] == 1 && tetris->info.field[i + y][j + x] == 1) {
                    result = 0;
                    break;
                }
            }
        }
    }
    return result;
}

int is_y_collision(int shape[4][4], int y) {
    int result = 0;
    Tetris *tetris = get_tetris();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shape[i][j] == 1 && (y + i >= 20 || tetris->info.field[y + i + 1][tetris->tetro.x + j])) {
                result = 1;
                break;
            }
        }
    }
    return result;
}

int is_x_collision(int shape[4][4], int x) {
    int result = 0;
    Tetris *tetris = get_tetris();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shape[i][j] == 1 && (j + x > 9 || j + x < 0 || tetris->info.field[i + tetris->tetro.y][j + x] == 1)) {
                    result = 1;
                    break;
            }
        }
    }
    return result;
}