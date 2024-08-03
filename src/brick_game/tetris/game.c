#include "game.h"

void init_game(Tetris *tetris) {
    tetris->info.field = allocate_field(FIELD_HEIGHT + 3, FIELD_WIDTH);
    tetris->info.next = allocate_field(4, 4);
    tetris->info.score = 0;
    tetris->info.high_score = load_high_score("db.txt");
    tetris->info.level = 1;
    tetris->info.pause = 0;
    tetris->info.speed = 1;

    tetris->is_start = 1;
}

void init_tetro(Tetris *tetris) {
    tetris->tetro.x = 3;
    tetris->tetro.y = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tetris->tetro.shape[i][j] = tetris->info.next[i][j];
        }
    }
}

void place_next(int shape[4][4]) {
    Tetris* tetris = get_tetris();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tetris->info.next[i][j] = 0;
            if (shape[i][j] == 1) {
                tetris->info.next[i][j] = 1;
            }
        }
    }
}

int** allocate_field(int height, int width) {
    int** field = malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++) {
        field[i] = calloc(width, sizeof(int));
    }
    return field;
}

void plant_tetro() {
    Tetris *tetris = get_tetris();

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetris->tetro.shape[i][j] == 1) {
                tetris->info.field[i + tetris->tetro.y][ j + tetris->tetro.x] = 1;
            }
        }
    }
}

void clear_tetro() {
    Tetris *tetris = get_tetris();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetris->info.field[i + tetris->tetro.y][j + tetris->tetro.x] == 1 && tetris->tetro.shape[i][j] == 1) {
                tetris->info.field[i + tetris->tetro.y][j + tetris->tetro.x] = 0;
            }
        }
    }
}

void check_line_fill() {
    int count = 0;
    Tetris *tetris = get_tetris();
    int lines = 0;
    for (int i = 0; i < FIELD_HEIGHT + 1; i++) {
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
            lines++;
        }
        count = 0;
    }
    if (lines == 1)
        tetris->info.score += 100;
    else if (lines == 2)
        tetris->info.score += 300;
    else if (lines == 3)
        tetris->info.score += 700;
    else if (lines == 4) 
        tetris->info.score += 1500;

    if (tetris->info.score > tetris->info.high_score) {
        tetris->info.high_score = tetris->info.score;
        save_high_score("db.txt", tetris->info.high_score);
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
    int result = 0;
    static struct timeval last_time;
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    if (last_time.tv_sec == 0 && last_time.tv_usec == 0) {
        last_time = current_time;
    }
    long elapsed_time = (current_time.tv_sec - last_time.tv_sec) * 1000000 + 
                        (current_time.tv_usec - last_time.tv_usec);

    if (elapsed_time >= 1000000) {
        last_time = current_time;
        result = 1;
    }

    return result; // Время еще не прошло
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
    clear_tetro();
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
    plant_tetro();
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
                if ((temp[i][j] == 1 && tetris->info.field[i + y][j + x] == 1) || tetris->tetro.y == 0) {
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

GameInfo_t updateCurrentState() {
    return get_tetris()->info;
}

GameState get_state() {
    Tetris *tetris = get_tetris();
    return tetris->state;
}

Tetris* get_tetris() {
    static Tetris tetris;
    if (tetris.info.field == NULL) {
        init_game(&tetris);
        tetris.state = START;
    }
    return &tetris;
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

int game_over() {
    int flag = 1;
    Tetris *tetris = get_tetris();
    if (tetris->state == GAMEOVER || tetris->state == EXIT_STATE) {
        flag = 0;
    }
    return flag;
}

void save_high_score(const char *filename, int high_score) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Unable to open file for writing");
        return;
    }
    fprintf(file, "%d\n", high_score);
    fclose(file);
}

int load_high_score(const char *filename) {
    int high_score = 0;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file for reading");
        high_score = 0; // Если файл не найден, возвращаем 0
    } else {
        fscanf(file, "%d", &high_score);
    }
    fclose(file);
    return high_score;
}