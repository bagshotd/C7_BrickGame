#include "model_snake.h"

namespace s21 {

Entities::Entities() : x{}, y{} {}
Entities::Entities(int x, int y) : x{x}, y{y} {}
Entities::Entities(const Entities &other) : x{other.x}, y{other.y} {}

Entities Entities::generateRandomApple(std::vector<Entities> &snake_body) {
  std::random_device random_device;
  std::mt19937 random_generator(random_device());
  std::uniform_int_distribution<> x_pos(0, WIDTH - 1);
  std::uniform_int_distribution<> y_pos(0, HEIGTH - 1);

  Entities new_apple;
  bool valid_position = false;

  while (!valid_position) {
    new_apple.x = x_pos(random_generator);
    new_apple.y = y_pos(random_generator);

    valid_position = true;
    for (const auto &segment : snake_body) {
      if (segment.x == new_apple.x && segment.y == new_apple.y) {
        valid_position = false;
        break;
      }
    }
  }

  return new_apple;
}

Entities &Entities::operator=(const Entities &other) {
  if (this != &other) {
    x = other.x;
    y = other.y;
  }
  return *this;
}

GameLogic::GameLogic() {
  game_info.field = new int *[HEIGTH];
  for (int i{}; i < HEIGTH; ++i) {
    game_info.field[i] = new int[WIDTH]{};
  }

  game_state = GameStart;
  current_direction = ToDown;
  next_direction = ToDown;
  game_info.score = game_info.level = 0;
  game_info.high_score = getScore();
  game_info.pause = is_started = false;
  game_info.speed = 600;
  initializeSnake();
  clock_gettime(CLOCK_REALTIME,
                &last_move_time);
}

GameLogic::~GameLogic() {
  for (int i = 0; i < HEIGTH; ++i) {
    delete[] game_info.field[i];
    game_info.field[i] =
        nullptr;
  }

  delete[] game_info.field;
  game_info.field =
      nullptr;
}

int GameLogic::getScore() {
  int high_score = 0;
  FILE *file = fopen("./snake_top.txt", "r");
  if (file != nullptr) {
    if (fscanf(file, "%d", &high_score) != 1) {
      high_score = 0;
    }
    fclose(file);
  }
  return high_score;
}

void GameLogic::setScore(int high_score) {
  FILE *file = fopen("./snake_top.txt", "w");
  if (file != nullptr) {
    fprintf(file, "%d", high_score);
    fclose(file);
  }
}

void GameLogic::generateRandomApple() {
  Entities new_apple = unit.generateRandomApple(snake_body);
  unit.x = new_apple.x;
  unit.y = new_apple.y;
}

void GameLogic::initializeGameInfo(void) {
  clearGameField();
  snake_body.clear();
  initializeSnake();
  generateRandomApple();
  current_direction = ToDown;
  next_direction = ToDown;
  updateFiguresOnField();
};

void GameLogic::initializeSnake() {
  snake_body.clear();
  snake_body.push_back(Entities(WIDTH / 2, HEIGTH / 2 + 1));
  snake_body.push_back(Entities(WIDTH / 2, HEIGTH / 2));
  snake_body.push_back(Entities(WIDTH / 2, HEIGTH / 2 - 1));
  snake_body.push_back(Entities(WIDTH / 2, HEIGTH / 2 - 2));
}

void GameLogic::clearGameField() {
  for (int i = 0; i < HEIGTH; ++i) {
    std::fill(game_info.field[i], game_info.field[i] + WIDTH, EMPTY);
  }
}

void GameLogic::updateSnakeOnField() {
  bool isHead = true;

  for (const auto &segment : snake_body) {
    if (isHead) {
      game_info.field[segment.y][segment.x] = SNAKE_HEAD;
      isHead = false;
    } else {
      game_info.field[segment.y][segment.x] =
          SNAKE_BODY;
    }
  }
}

void GameLogic::updateAppleOnField() {
  game_info.field[unit.y][unit.x] = APPLE;
}

GameInfo_t GameLogic::updateFiguresOnField(void) {
  clearGameField();
  updateSnakeOnField();
  updateAppleOnField();
  return game_info;
}

bool isOutOfField(int x, int y) {
  bool out_of_field = (x < 0 || x >= WIDTH || y < 0 || y >= HEIGTH);
  return out_of_field;
}

bool isSnakeCollision(int x, int y, std::vector<Entities> &snake_body) {
  bool collision = false;

  for (auto it = snake_body.begin() + 1; it != snake_body.end(); ++it) {
    if (it->x == x && it->y == y) {
      collision = true;
      break;
    }
  }

  return collision;
}

bool GameLogic::checkMove() {
  Entities head = snake_body.front();
  bool is_invalid_move = isOutOfField(head.x, head.y) ||
                         isSnakeCollision(head.x, head.y, snake_body);
  return is_invalid_move;
}

void GameLogic::refreshGameInfo() {
  game_info.score++;

  if (game_info.high_score < game_info.score) {
    game_info.high_score = game_info.score;
  }

  if (game_info.level < 10) {
    game_info.level = game_info.score / 5;
  }

  game_info.speed = 500 - 50 * game_info.level;
}

void updateHeadPosition(Entities &head, SnakeDir next_direction) {
  switch (next_direction) {
    case ToUp:
      head.y -= 1;
      break;
    case ToDown:
      head.y += 1;
      break;
    case ToRight:
      head.x += 1;
      break;
    case ToLeft:
      head.x -= 1;
      break;
    default:
      break;
  }
}

void handleAppleEaten(GameLogic *game) {
  game->game_state = Spawn;
  game->refreshGameInfo();
}

void handleCollision(GameLogic *game) {
  game->clearGameField();
  game->snake_body.clear();
  game->game_info.score = game->game_info.level = 0;
  game->game_info.speed = 600;
  game->setScore(game->game_info.high_score);
  game->game_state = GameStart;
  game->is_started = false;
}

void GameLogic::moveSnake() {
  if (game_info.pause || !is_started)
    return;

  Entities new_head{snake_body.front()};

  updateHeadPosition(new_head, next_direction);

  snake_body.insert(snake_body.begin(), new_head);

  if (new_head.x == unit.x && new_head.y == unit.y) {
    handleAppleEaten(this);
  } else if (checkMove()) {
    handleCollision(this);
  } else {
    snake_body.pop_back();
    game_state = Moving;
  }

  current_direction = next_direction;
  clock_gettime(CLOCK_REALTIME,
                &last_move_time);
  updateFiguresOnField();
}
}