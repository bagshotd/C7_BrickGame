#include <check.h>
#include <stdbool.h>
#include <stdio.h>

#include "../back/backend.h"

START_TEST(test_serv) {
  Game game = gameInit();
  Tmove move = TNONE;
  game.score = 5500;
  Gstatus status = tick(&game, move);
  ck_assert_int_eq(status, GO);
  ck_assert_int_eq(game.highScore, 5500);
  status = tick(&game, move);
  newGame(&game);
  ck_assert_int_eq(game.highScore, 5500);

  while (status != AGAIN) {
    status = tick(&game, move);
  }
  ck_assert_int_eq(status, AGAIN);
  gameDelete(&game);
}
END_TEST

START_TEST(test_block) {
  Game game = gameInit();
  Tmove move = TUP;
  Gstatus status = GO;
  while (status != AGAIN) {
    ck_assert_int_eq(status, 2);
    status = tick(&game, move);
  }

  ck_assert_int_eq(status, 1);
  newGame(&game);
  move = TLEFT;
  status = tick(&game, move);
  while (status != AGAIN) {
    ck_assert_int_eq(status, 2);
    status = tick(&game, move);
  }

  ck_assert_int_eq(status, 1);
  newGame(&game);
  move = TRIGHT;
  status = tick(&game, move);
  while (status != AGAIN) {
    ck_assert_int_eq(status, 2);
    status = tick(&game, move);
  }

  ck_assert_int_eq(status, 1);
  newGame(&game);
  move = TCOUNTER;
  status = tick(&game, move);
  while (status != AGAIN) {
    ck_assert_int_eq(status, 2);
    status = tick(&game, move);
  }

  ck_assert_int_eq(status, 1);
  newGame(&game);
  move = TDROP;
  status = tick(&game, move);
  while (status != AGAIN) {
    ck_assert_int_eq(status, 2);
    status = tick(&game, move);
  }

  ck_assert_int_eq(status, 1);
  newGame(&game);
  move = TDOWN;
  tick(&game, move);
  ck_assert_int_eq(game.gravityTicks, 70);
  move = TDOWN;
  status = tick(&game, move);
  while (status != AGAIN) {
    ck_assert_int_eq(status, 2);
    for (int i = 0; i < 2; i++) {
      status = tick(&game, move);
      game.gravityTicks = 0;
    }
  }

  move = TPAUSE;
  status = tick(&game, move);
  ck_assert_int_eq(status, 1);
  gameDelete(&game);
}
END_TEST

START_TEST(test_lines) {
  Game game = gameInit();
  Tmove move = TNONE;
  tick(&game, move);
  for (int i = 189; i < 200; i++) {
    game.field[i] = 1;
  }
  tick(&game, move);
  ck_assert_int_eq(game.score, 100);
  gameDelete(&game);
}
END_TEST

START_TEST(test_action) {
  Game game = gameInit();
  Tmove move = TNONE;
  tick(&game, move);
  move = action();
  ck_assert_int_eq(move, TNONE);
  gameDelete(&game);
}
END_TEST

Suite *back() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("back func");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_serv);
  tcase_add_test(tc_core, test_block);
  tcase_add_test(tc_core, test_lines);
  tcase_add_test(tc_core, test_action);
  suite_add_tcase(s, tc_core);

  return s;
}

int test_backend() {
  int no_failed = 0;

  Suite *s;
  SRunner *runner;

  s = back();
  runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return no_failed;
}

int main() {
  int no_failed = 0;

  no_failed |= test_backend();

  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
