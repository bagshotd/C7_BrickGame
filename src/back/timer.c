#include "backend.h"

void sleepMilli(int milliseconds) {
  struct timespec timer = {0};
  timer.tv_sec = 0;
  timer.tv_nsec = milliseconds * 1000 * 1000;

  nanosleep(&timer, NULL);
}