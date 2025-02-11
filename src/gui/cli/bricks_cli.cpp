#include "bricks_cli.h"

int main(void) {
  init_ncurses();
  int key = getch();
  while (key != 'q' && key != 'Q') {
    erase();
    mvprintw(1, 1, "==================================");
    mvprintw(2, 5, "BRICK'S GAME Version 2.0");
    mvprintw(3, 1, "==================================");

    mvprintw(5, 5, "Press 'T' to play TETRIS");
    mvprintw(6, 5, "Press 'S' to play SNAKE");
    mvprintw(7, 5, "Press 'Q' to EXIT");

    if (key == 't' || key == 'T') {
      clear();
      runTetrisGame();
    } else if (key == 's' || key == 'S') {
      clear();
      s21::GameLogic model{};
      s21::Controller controller{&model};
      s21::View view{&controller};
      view.runSnakeGame();
    }

    key = getch();
  }
  end_ncurses();
  return 0;
}

void init_ncurses(void) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  timeout(50);
  curs_set(0);
}

void end_ncurses(void) { endwin(); };