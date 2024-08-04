#include <check.h>
#include <stdio.h>

#include "fsm.h"

static int tetro[4][4] = {
    {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};

START_TEST(init_game_test) {
  Tetris *tetris = get_tetris();
  ck_assert_int_eq(tetris->info.pause, 0);
  ck_assert_int_eq(tetris->info.level, 1);
  ck_assert_int_eq(tetris->info.score, 0);
}
END_TEST

START_TEST(allocate_field_test) {
  Tetris *tetris = get_tetris();
  ck_assert_int_eq(tetris->info.field[0][0], 0);
}
END_TEST

START_TEST(load_high_score_test) {
  int res = load_high_score("test.txt");
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(game_over_test) {
  Tetris *tetris = get_tetris();
  tetris->state = GAMEOVER;
  int res = game_over();
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(user_input_test) {
  Tetris *tetris = get_tetris();
  userInput(Start, 0);
  ck_assert_int_eq(SPAWN, tetris->state);
}
END_TEST

START_TEST(get_random_tet_test) {
  int res = get_random_tet();
  ck_assert_int_le(res, 7);
  ck_assert_int_ge(res, 0);
}
END_TEST

START_TEST(pause_state_test) {
  Tetris *tetris = get_tetris();
  UserAction_t action = Pause;
  pause_state(&action);
  ck_assert_int_eq(1, tetris->info.pause = 1);
}
END_TEST

START_TEST(attaching_state_test) {
  Tetris *tetris = get_tetris();
  attaching_state();
  ck_assert_int_eq(tetris->state, SPAWN);
}
END_TEST

START_TEST(shifting_state_test) {
  Tetris *tetris = get_tetris();
  int is_attached = 0;
  int is_down = 0;
  shifting_state(&is_attached, &is_down);
  ck_assert_int_eq(tetris->state, MOVING);
  is_attached = 1;
  shifting_state(&is_attached, &is_down);
  ck_assert_int_eq(tetris->state, ATTACHING);
}
END_TEST

START_TEST(spawn_state_test) {
  Tetris *tetris = get_tetris();
  tetris->is_start = 1;
  spawn_state();
  ck_assert_int_eq(tetris->is_start, 0);
}
END_TEST

START_TEST(start_state_test) {
  Tetris *tetris = get_tetris();
  UserAction_t action = Start;
  start_state(&action);
  ck_assert_int_eq(tetris->state, SPAWN);
}
END_TEST

START_TEST(moving_state_test) {
  UserAction_t action = Down;
  int is_down = 0;
  bool hold = 0;
  moving_state(&action, &hold, &is_down);
  ck_assert_int_eq(is_down, 1);
}
END_TEST

START_TEST(get_tetris_test) {
  Tetris *tetris = get_tetris();
  ck_assert_int_eq(tetris->state, START);
}
END_TEST

START_TEST(get_state_test) {
  Tetris *tetris = get_tetris();
  tetris->state = SPAWN;
  GameState state = get_state();
  ck_assert_int_eq(state, SPAWN);
}
END_TEST

START_TEST(check_timeout_test) {
  int result = check_timeout();
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(place_next_test) {
  place_next(tetro);
  Tetris *tetris = get_tetris();
  ck_assert_int_eq(tetris->info.next[1][1], 1);
}
END_TEST

START_TEST(init_tetro_test) {
  Tetris *tetris = get_tetris();
  place_next(tetro);
  init_tetro(tetris);
  ck_assert_int_eq(tetris->tetro.shape[1][1], 1);
}
END_TEST

START_TEST(plant_tetro_test) {
  Tetris *tetris = get_tetris();
  place_next(tetro);
  init_tetro(tetris);
  plant_tetro();
  ck_assert_int_eq(tetris->info.field[1][4], 1);
}
END_TEST

START_TEST(clear_tetro_test) {
  Tetris *tetris = get_tetris();
  place_next(tetro);
  init_tetro(tetris);
  plant_tetro();
  clear_tetro();
  ck_assert_int_eq(tetris->info.field[1][4], 0);
}
END_TEST

START_TEST(clear_current_tetro_test) {
  Tetris *tetris = get_tetris();
  place_next(tetro);
  init_tetro(tetris);
  clear_current_tetro();
  ck_assert_int_eq(tetris->tetro.shape[1][1], 0);
}
END_TEST

START_TEST(move_left_test) {
  Tetris *tetris = get_tetris();
  place_next(tetro);
  init_tetro(tetris);
  move_left(&tetris->tetro);
  ck_assert_int_eq(tetris->tetro.x, 2);
}
END_TEST

Suite *game_suite(void) {
  Suite *s;
  TCase *tc_fsm;
  TCase *tc_game;

  s = suite_create("GameTests");
  tc_fsm = tcase_create("FSM");
  tc_game = tcase_create("GAME");

  tcase_add_test(tc_game, init_game_test);
  tcase_add_test(tc_game, allocate_field_test);
  tcase_add_test(tc_game, load_high_score_test);
  tcase_add_test(tc_game, game_over_test);
  tcase_add_test(tc_game, get_tetris_test);
  tcase_add_test(tc_game, get_state_test);
  tcase_add_test(tc_game, check_timeout_test);
  tcase_add_test(tc_game, place_next_test);
  tcase_add_test(tc_game, init_tetro_test);
  tcase_add_test(tc_game, plant_tetro_test);
  tcase_add_test(tc_game, clear_tetro_test);
  tcase_add_test(tc_game, clear_current_tetro_test);
  tcase_add_test(tc_game, move_left_test);

  tcase_add_test(tc_fsm, user_input_test);
  tcase_add_test(tc_fsm, get_random_tet_test);
  tcase_add_test(tc_fsm, pause_state_test);
  tcase_add_test(tc_fsm, attaching_state_test);
  tcase_add_test(tc_fsm, shifting_state_test);
  tcase_add_test(tc_fsm, spawn_state_test);
  tcase_add_test(tc_fsm, start_state_test);
  tcase_add_test(tc_fsm, moving_state_test);

  suite_add_tcase(s, tc_fsm);
  suite_add_tcase(s, tc_game);
  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = game_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? 0 : 1;
}