#include <gtest/gtest.h>
#include <unistd.h>

#include "../brick_game/snake/controller_snake.h"

TEST(GameInitialization, InitialStateIsStartGame) {
  s21::GameLogic game{};
  s21::Controller controller{&game};
  EXPECT_EQ(game.game_state, s21::GameStart);
  controller.userInput(Start, false);
  controller.updateCurrentState();
  EXPECT_EQ(game.game_state, s21::Moving);
  controller.userInput(Pause, false);
  EXPECT_EQ(game.game_info.pause, true);
  controller.updateCurrentState();
}

TEST(SnakeCollision, SnakeHitsWallAndResets) {
  s21::GameLogic game{};
  s21::Controller controller{&game};
  EXPECT_EQ(game.game_state, s21::GameStart);
  controller.userInput(Start, false);
  controller.updateCurrentState();
  controller.userInput(Left, false);
  for (int i{}; i < 7; ++i) game.moveSnake();
  controller.updateCurrentState();
  EXPECT_EQ(game.game_state, s21::GameStart);
}

TEST(SnakeCollision, SnakeHitsItselfAndResets) {
  s21::GameLogic game{};
  s21::Controller controller{&game};
  EXPECT_EQ(game.game_state, s21::GameStart);
  controller.userInput(Start, false);
  controller.updateCurrentState();
  controller.userInput(Left, false);
  game.moveSnake();
  controller.userInput(Up, false);
  game.moveSnake();
  controller.userInput(Right, false);
  game.moveSnake();
  controller.updateCurrentState();
  EXPECT_EQ(game.game_state, s21::GameStart);
}

TEST(PauseFunctionality, GamePausesAndResumes) {
  s21::GameLogic game{};
  s21::Controller controller{&game};
  EXPECT_EQ(game.game_state, s21::GameStart);
  controller.userInput(Start, false);
  controller.updateCurrentState();
  controller.userInput(Pause, false);
  EXPECT_EQ(game.game_info.pause, true);
  controller.userInput(Pause, false);
  EXPECT_EQ(game.game_info.pause, false);
  controller.updateCurrentState();
  EXPECT_EQ(game.game_state, s21::Moving);
}

TEST(GameTermination, GameEndsOnTerminateInput) {
  s21::GameLogic game{};
  s21::Controller controller{&game};
  EXPECT_EQ(game.game_state, s21::GameStart);
  controller.userInput(Start, false);
  controller.updateCurrentState();
  controller.userInput(Terminate, false);
  controller.updateCurrentState();
  EXPECT_EQ(game.game_state, s21::GameOver);
}

TEST(DirectionChange, SnakeChangesDirectionDown) {
  s21::GameLogic game{};
  s21::Controller controller{&game};
  EXPECT_EQ(game.game_state, s21::GameStart);
  controller.userInput(Start, false);
  controller.updateCurrentState();
  controller.userInput(Down, false);
  controller.updateCurrentState();
  EXPECT_EQ(game.game_state, s21::Moving);
}

TEST(AppleGeneration, AppleSpawnsCorrectly) {
  s21::GameLogic game{};
  s21::Controller controller{&game};
  EXPECT_EQ(game.game_state, s21::GameStart);
  controller.userInput(Start, false);
  game.unit.x = 5;
  game.unit.y = 14;
  game.moveSnake();
  game.moveSnake();
  game.moveSnake();
  EXPECT_EQ(game.game_state, s21::Spawn);
  controller.updateCurrentState();
  EXPECT_EQ(game.game_state, s21::Moving);
}

TEST(GameSpeed, GameSpeedsUpOverTime) {
  s21::GameLogic game{};
  s21::Controller controller{&game};
  EXPECT_EQ(game.game_state, s21::GameStart);
  controller.userInput(Start, false);
  controller.updateCurrentState();
  usleep(1000000);
  controller.updateCurrentState();
  EXPECT_EQ(game.game_state, s21::Rotate);
  controller.updateCurrentState();
  EXPECT_EQ(game.game_state, s21::Moving);
}

TEST(ScoreUpdate, ScoreAndLevelUpdateCorrectly) {
  s21::GameLogic game{};
  s21::Controller controller{&game};
  EXPECT_EQ(game.game_state, s21::GameStart);
  EXPECT_EQ(game.game_info.level, 0);
  controller.userInput(Start, false);
  game.game_info.score = 9;
  game.unit.x = 5;
  game.unit.y = 14;
  game.moveSnake();
  game.moveSnake();
  game.moveSnake();
  EXPECT_EQ(game.game_info.level, 2);
  EXPECT_EQ(game.game_info.score, 10);
  controller.updateCurrentState();
  EXPECT_EQ(game.game_state, s21::Moving);
}

TEST(LevelUpdate, LevelRemainsSameIfNotEnoughPoints) {
  s21::GameLogic game{};
  s21::Controller controller{&game};
  EXPECT_EQ(game.game_state, s21::GameStart);
  EXPECT_EQ(game.game_info.level, 0);
  controller.userInput(Start, false);
  game.game_info.level = 10;
  game.game_info.score = 60;
  game.unit.x = 5;
  game.unit.y = 14;
  game.moveSnake();
  game.moveSnake();
  game.moveSnake();
  EXPECT_EQ(game.game_info.level, 10);
  EXPECT_EQ(game.game_info.score, 61);
  controller.updateCurrentState();
  EXPECT_EQ(game.game_state, s21::Moving);
}

TEST(ScoreIncrement, ScoreIncreasesWhenAppleEaten) {
  s21::GameLogic game{};
  s21::Controller controller{&game};
  EXPECT_EQ(game.game_state, s21::GameStart);
  EXPECT_EQ(game.game_info.level, 0);
  controller.userInput(Start, false);
  game.unit.x = 5;
  game.unit.y = 14;
  game.moveSnake();
  game.moveSnake();
  game.moveSnake();
  EXPECT_EQ(game.game_info.level, 0);
  EXPECT_EQ(game.game_info.score, 1);
  controller.updateCurrentState();
  EXPECT_EQ(game.game_state, s21::Moving);
}

TEST(DirectionChange, SnakeMovesLeftAndEatsApple) {
  s21::GameLogic game{};
  s21::Controller controller{&game};
  EXPECT_EQ(game.game_state, s21::GameStart);
  EXPECT_EQ(game.game_info.level, 0);
  controller.userInput(Start, false);
  game.moveSnake();
  controller.userInput(Left, false);
  game.unit.x = 3;
  game.unit.y = 12;
  game.moveSnake();
  game.moveSnake();
  EXPECT_EQ(game.game_info.level, 0);
  EXPECT_EQ(game.game_info.score, 1);
  controller.updateCurrentState();
  EXPECT_EQ(game.game_state, s21::Moving);
}

TEST(DirectionChange, SnakeMovesRightAndEatsApple) {
  s21::GameLogic game{};
  s21::Controller controller{&game};
  EXPECT_EQ(game.game_state, s21::GameStart);
  EXPECT_EQ(game.game_info.level, 0);
  controller.userInput(Start, false);
  game.moveSnake();
  controller.userInput(Right, false);
  game.unit.x = 8;
  game.unit.y = 12;
  game.moveSnake();
  game.moveSnake();
  game.moveSnake();
  EXPECT_EQ(game.game_info.level, 0);
  EXPECT_EQ(game.game_info.score, 1);
  controller.updateCurrentState();
  EXPECT_EQ(game.game_state, s21::Moving);
}

TEST(DirectionChange, SnakeMovesRightThenUpAndEatsApple) {
  s21::GameLogic game{};
  s21::Controller controller{&game};
  EXPECT_EQ(game.game_state, s21::GameStart);
  EXPECT_EQ(game.game_info.level, 0);
  controller.userInput(Start, false);
  game.unit.x = 6;
  game.unit.y = 11;
  game.moveSnake();
  controller.userInput(Right, false);
  game.moveSnake();
  controller.userInput(Up, false);
  game.moveSnake();
  EXPECT_EQ(game.game_info.level, 0);
  EXPECT_EQ(game.game_info.score, 1);
  controller.updateCurrentState();
  EXPECT_EQ(game.game_state, s21::Moving);
}