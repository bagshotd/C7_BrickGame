#ifndef FRONTEND_H
#define FRONTEND_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../back/backend.h"

#define COLSPERCELL 2
#define ADDBLOCK(w, x)                          \
  waddch((w), ' ' | A_REVERSE | COLOR_PAIR(x)); \
  waddch((w), ' ' | A_REVERSE | COLOR_PAIR(x))
#define ADDEMPTY(w) \
  waddch((w), ' '); \
  waddch((w), ' ')
#define CURSINIT()        \
  {                       \
    initscr();            \
    cbreak();             \
    noecho();             \
    keypad(stdscr, TRUE); \
    timeout(0);           \
    curs_set(0);          \
    initColors();         \
  }
#define WINDOWINIT(x, y, z)                                   \
  {                                                           \
    int row = 0, col = 0;                                     \
    getmaxyx(stdscr, row, col);                               \
    x = newwin(ROWS + 2, 2 * COLUMNS + 2, row / 10, col / 3); \
    y = newwin(4, 11, row / 7, col / 1.4);                    \
    z = newwin(5, 20, 14, col / 1.4);                         \
  }
#define PRINT(wB, wN, wS, game) \
  {                             \
    printField(board, game);    \
    printNext(next, game);      \
    printStat(score, game);     \
    doupdate();                 \
    sleepMilli(10);             \
  }

void printField(WINDOW *w, Game *game);
void printNext(WINDOW *w, Game *game);
void printStat(WINDOW *w, Game *game);
void initColors();
void start();
void pause();
Gstatus again(Game *game);

#endif