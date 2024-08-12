#ifndef BACKEND_H
#define BACKEND_H

#define _POSIX_C_SOURCE 199309L

#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ROWS 20
#define COLUMNS 10
#define LEFT -1
#define RIGHT 1
#define TETRAMINOS 7
#define ORI 4
#define CELLS 4
#define STARTSPEED 1
#define STARTSCORE 0
#define STARTLEVEL 1
#define STARTGRAVITY 50
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define TYPETOCELL(x) ((x) + 1)
#define ISEMPTY(x) ((x) == TEMPTY)
#define ISFILLED(x) (!ISEMPTY(x))
typedef enum {
  TLEFT,
  TRIGHT,
  TUP,
  TCOUNTER,
  TDOWN,
  TDROP,
  TCLOCK,
  TPAUSE,
  TEND,
  TNONE
} Tmove;

typedef enum { END, AGAIN, GO, PAUSE } Gstatus;

typedef struct Location {
  int row;
  int col;
} Location;

typedef struct Block {
  int type;
  int orintation;
  Location loc;
} Block;

typedef enum {
  TEMPTY,
  TCELLI,
  TCELLJ,
  TCELLL,
  TCELLO,
  TCELLS,
  TCELLT,
  TCELLZ
} Tcell;

typedef struct Game {
  int rows;
  int cols;
  int *field;
  int score;
  int highScore;
  int level;
  int speed;
  int gravityTicks;
  Block *falling;
  Block *next;
} Game;

Game gameInit();
Location getTetr(int tetramino, int orintation, int cells);
int randomTetr();
void gameDelete(Game *game);
int geBlock(Game *game, int row, int col);
void seBlock(Game *game, int row, int col, int value);
bool checkBlock(const Game *game, int row, int col);
void putTetramino(Game *game, Block *block);
void rmTetramino(Game *game, Block *block);
bool checkMovesTetramino(Game *game, const Block *block);
void newFalling(Game *game);
void moveX(Game *game, int place);
void moveRotate(Game *game, int orint);
void moveDown(Game *game);
bool isFullLines(Game *game, int index);
void shiftLines(Game *game, int index);
int checkLines(Game *game);
bool gameOver(Game *game);
void gravityBySpeed(Game *game);
Gstatus tick(Game *game, Tmove move);
void saveScore(const Game *game);
int getHighScore();
void newGame(Game *game);
Tmove action();
void upper(Game *game, int clearLines);
void moveDrop(Game *game);
void sleepMilli(int milliseconds);
void moving(Game *game, Tmove move);
int getGravity(int index);
bool againGame(Game *game);

#endif