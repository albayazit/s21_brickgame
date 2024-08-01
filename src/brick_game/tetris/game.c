#include "game.h"

void init_game(Tetris *tetris) {
    tetris->info.field = allocate_field(FIELD_HEIGHT + 2, FIELD_WIDTH);
    tetris->info.next = allocate_field(4, 4);
    tetris->info.score = 0;
    tetris->info.high_score = 0;
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

GameInfo_t updateCurrentState() {
    return get_tetris()->info;
}


int game_over() {
    int flag = 1;
    Tetris *tetris = get_tetris();
    if (tetris->state == GAMEOVER || tetris->state == EXIT_STATE) {
        flag = 0;
    }
    return flag;
}

Tetris* get_tetris() {
    static Tetris tetris;
    if (tetris.info.field == NULL) {
        init_game(&tetris);
        tetris.state = START;
    }
    return &tetris;
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
