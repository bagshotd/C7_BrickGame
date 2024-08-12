#include "backend.h"

void moving(Game *game, Tmove move) {
  switch (move) {
    case TLEFT:
      moveX(game, LEFT);
      break;
    case TRIGHT:
      moveX(game, RIGHT);
      break;
    case TDROP:
      moveDrop(game);
      break;
    case TDOWN:
      moveDown(game);
      break;
    case TUP:
      moveRotate(game, 1);
      break;
    case TCOUNTER:
      moveRotate(game, -1);
      break;
    default:
      break;
  }
}

void moveX(Game *game, int place) {
  rmTetramino(game, game->falling);
  game->falling->loc.col += place;
  if (!checkMovesTetramino(game, game->falling)) {
    game->falling->loc.col -= place;
  }
  putTetramino(game, game->falling);
}

void moveDown(Game *game) {
  rmTetramino(game, game->falling);
  game->falling->loc.row++;
  if (!checkMovesTetramino(game, game->falling)) {
    game->falling->loc.row--;
  }
  putTetramino(game, game->falling);
  game->gravityTicks = getGravity(game->speed);
}

void moveDrop(Game *game) {
  rmTetramino(game, game->falling);
  while (checkMovesTetramino(game, game->falling)) {
    game->falling->loc.row++;
  }
  game->falling->loc.row--;
  putTetramino(game, game->falling);
  newFalling(game);
}

void moveRotate(Game *game, int orint) {
  rmTetramino(game, game->falling);
  while (1) {
    game->falling->orintation = (game->falling->orintation + orint) % ORI;

    if (checkMovesTetramino(game, game->falling)) break;

    game->falling->loc.col--;
    if (checkMovesTetramino(game, game->falling)) break;

    game->falling->loc.col += 2;
    if (checkMovesTetramino(game, game->falling)) break;

    game->falling->loc.col--;
  }

  putTetramino(game, game->falling);
}

Tmove action() {
  Tmove move = TNONE;
  switch (getch()) {
    case KEY_LEFT:
      move = TLEFT;
      break;
    case KEY_RIGHT:
      move = TRIGHT;
      break;
    case KEY_UP:
      move = TUP;
      break;
    case KEY_DOWN:
      move = TDOWN;
      break;
    case 'q':
      move = TEND;
      break;
    case ' ':
      move = TDROP;
      break;
    case 'p':
      move = TPAUSE;
      break;
    default:
      move = TNONE;
  }

  return move;
}