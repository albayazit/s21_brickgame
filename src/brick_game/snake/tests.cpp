#include <gtest/gtest.h>

#include "snake_game.h"

using namespace s21;

// Тест загрузки рекордного счета (файл отсутствует)
TEST(SnakeGameTest, LoadHighScore_NoFile) {
  SnakeGame game;
  EXPECT_EQ(game.getHighScore(),
            0);  // Если файл отсутствует, score должен быть 0
}

// Тест размещения змейки
TEST(SnakeGameTest, PlaceSnake) {
  SnakeGame game;
  game.placeSnake();
  const int* const* field = game.getField();

  EXPECT_EQ(field[10][5], 1);  // Голова змейки
  EXPECT_EQ(field[10][4], 1);  // Тело змейки
  EXPECT_EQ(field[10][3], 1);
  EXPECT_EQ(field[10][2], 1);
}

// Тест размещения яблока
TEST(SnakeGameTest, PlaceApple) {
  SnakeGame game;
  game.startGame();
  const int* const* field = game.getField();

  bool appleFound = false;
  for (int i = 1; i <= ROWS; ++i) {
    for (int j = 0; j < COLS; ++j) {
      if (field[i][j] == 2) {  // Яблоко помечено как 2
        appleFound = true;
        break;
      }
    }
  }
  EXPECT_TRUE(appleFound);
}

// Тест столкновения змейки
TEST(SnakeGameTest, CheckCollision) {
  SnakeGame game;
  game.startGame();

  int newHeadX, newHeadY;
  game.calculateNextHeadPosition(newHeadX, newHeadY);
  EXPECT_FALSE(game.checkCollision(newHeadX, newHeadY));  // Нет столкновения

  game.setDirection(Direction::UP);
  for (int i = 0; i < 10; ++i) {
    game.update();  // Двигаем змейку к стене
  }
  game.calculateNextHeadPosition(newHeadX, newHeadY);
  EXPECT_TRUE(game.checkCollision(
      newHeadX, newHeadY));  // Столкновение с верхней границей
}

// Тест FSM (изначальное состояние)
TEST(SnakeFSMTest, InitialState) {
  SnakeFSM fsm;
  EXPECT_EQ(fsm.getCurrentState(), GameState::START);
}

// Тест перехода FSM: StartGame -> PLAYING
TEST(SnakeFSMTest, FSMStartGame) {
  SnakeFSM fsm;
  fsm.triggerEvent(SnakeEvent::StartGame);
  EXPECT_EQ(fsm.getCurrentState(), GameState::PLAYING);
}

// Тест перехода FSM: PLAYING -> PAUSE -> RESUME
TEST(SnakeFSMTest, FSMPauseResume) {
  SnakeFSM fsm;
  fsm.triggerEvent(SnakeEvent::StartGame);
  EXPECT_EQ(fsm.getCurrentState(), GameState::PLAYING);

  fsm.triggerEvent(SnakeEvent::PauseGame);
  EXPECT_EQ(fsm.getCurrentState(), GameState::PAUSE);

  fsm.triggerEvent(SnakeEvent::ResumeGame);
  EXPECT_EQ(fsm.getCurrentState(), GameState::PLAYING);
}