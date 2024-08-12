#include <stdio.h>

#include "back/backend.h"
#include "front/frontend.h"

void game() {
  start();

  WINDOW *board = NULL, *next = NULL, *score = NULL;
  WINDOWINIT(board, next, score);

  Game game = gameInit();
  Gstatus status = GO;

  while (status != END) {
    if (status == PAUSE) {
      pause();
    }

    status = tick(&game, action());

    PRINT(board, next, score, &game);

    if (status == AGAIN) {
      status = again(&game);
    }
  }
}

int main() {
  CURSINIT();
  game();
  wclear(stdscr);
  endwin();

  return 0;
}