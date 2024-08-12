#include "backend.h"

bool isFullLines(Game *game, int index) {
  bool status = true;
  for (int i = 0; i < game->cols; i++) {
    if (geBlock(game, index, i) == 0) {
      status = false;
      break;
    }
  }

  return status;
}

void shiftLines(Game *game, int index) {
  for (int i = index - 1; i >= 0; i--) {
    for (int j = 0; j < game->cols; j++) {
      seBlock(game, i + 1, j, geBlock(game, i, j));
      seBlock(game, i, j, 0);
    }
  }
}

int checkLines(Game *game) {
  int lines = 0;
  rmTetramino(game, game->falling);
  for (int i = game->rows - 1; i >= 0; i--) {
    if (isFullLines(game, i)) {
      shiftLines(game, i);
      i++;
      lines++;
    }
  }
  putTetramino(game, game->falling);

  return lines;
}