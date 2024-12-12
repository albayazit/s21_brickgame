#ifndef SRC_BRICK_GAME_SNAKE_INC_SNAKE_GAME_H
#define SRC_BRICK_GAME_SNAKE_INC_SNAKE_GAME_H

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

#include "snake_fsm.h"

#define ROWS 20
#define COLS 10

namespace s21 {

/**
 * @brief Направления движения змейки.
 */
enum class Direction { UP, DOWN, LEFT, RIGHT };

/**
 * @brief Класс SnakeGame реализует основную логику игры Snake.
 */
class SnakeGame {
 private:
  SnakeFSM fsm_;
  int field_[ROWS + 1][COLS] = {};
  std::vector<std::pair<int, int>> snake_;
  std::pair<int, int> apple_;
  int score_;
  int direction_;
  int isGamePaused_;
  int isDirectionChanged_;
  int highScore_;
  int level_;

 public:
  /**
   * @brief Конструктор SnakeGame. Инициализирует подписки FSM.
   */
  SnakeGame() : score_(0), level_(1), highScore_(loadHighScore()) {
    fsm_.subscribe(SnakeEvent::StartGame, [this]() { startGame(); });
    fsm_.subscribe(SnakeEvent::PauseGame, [this]() { setPause(1); });
    fsm_.subscribe(SnakeEvent::ResumeGame, [this]() { setPause(0); });
  }
  /**
   * @brief Получает текущий уровень игры.
   * @return Текущий уровень.
   */
  int getLevel() { return level_; }
  /**
   * @brief Загружает рекордный счет из файла.
   * @return Рекордный счет.
   */
  int loadHighScore() {
    std::ifstream file("snake_bd.txt");
    if (file.is_open()) {
      int score;
      file >> score;
      file.close();
      return score;
    } else {
      return 0;
    }
  }
  /**
   * @brief Сохраняет рекордный счет в файл.
   */
  void saveHighScore() {
    if (score_ > highScore_) {
      highScore_ = score_;
      std::ofstream file("snake_bd.txt");
      if (file.is_open()) {
        file << highScore_;
        file.close();
      }
    }
  }

  /**
   * @brief Начинает новую игру.
   */
  void startGame() {
    srand(time(nullptr));
    placeApple();
    placeSnake();
    direction_ = static_cast<int>(Direction::RIGHT);
    isDirectionChanged_ = 0;
    isGamePaused_ = 0;
  }

  /**
   * @brief Получает текущий счет.
   * @return Текущий счет.
   */
  int getScore() { return score_; }

  /**
   * @brief Возвращает указатель на игровое поле.
   * @return Указатель на игровое поле.
   */
  const int* const* getField() const {
    static const int* fieldPointers[21];
    for (int i = 0; i < 21; ++i) {
      fieldPointers[i] = field_[i];
    }
    return fieldPointers;
  }

  /**
   * @brief Размещает змейку на игровом поле.
   */
  void placeSnake() {
    snake_ = {{10, 5}, {10, 4}, {10, 3}, {10, 2}};
    for (const auto& segment : snake_) {
      field_[segment.first][segment.second] = 1;
    }
  }

  /**
   * @brief Обновляет состояние игры.
   */
  void update() {
    isDirectionChanged_ = 0;
    moveSnake();
  }
  /**
   * @brief Получает рекордный счет.
   * @return Рекордный счет.
   */
  int getHighScore() const { return highScore_; }
  /**
   * @brief Вычисляет координаты следующей головы змейки.
   * @param newHeadX Координата X головы.
   * @param newHeadY Координата Y головы.
   */
  void calculateNextHeadPosition(int& newHeadX, int& newHeadY) {
    int dx = 0, dy = 0;
    switch (static_cast<Direction>(direction_)) {
      case Direction::UP:
        dx = -1;
        break;
      case Direction::DOWN:
        dx = 1;
        break;
      case Direction::LEFT:
        dy = -1;
        break;
      case Direction::RIGHT:
        dy = 1;
        break;
    }
    auto head = snake_.front();
    newHeadX = head.first + dx;
    newHeadY = head.second + dy;
  }
  /**
   * @brief Проверяет, есть ли столкновение змейки.
   * @param newHeadX Координата X новой головы.
   * @param newHeadY Координата Y новой головы.
   * @return true, если столкновение произошло.
   */
  bool checkCollision(int newHeadX, int newHeadY) {
    if (newHeadX < 1 || newHeadX > ROWS || newHeadY < 0 ||
        newHeadY > COLS - 1) {
      return true;
    }
    for (const auto& segment : snake_) {
      if (segment.first == newHeadX && segment.second == newHeadY) {
        return true;
      }
    }
    return false;
  }
  /**
   * @brief Проверяет, съела ли змейка яблоко.
   * @param newHeadX Координата X новой головы.
   * @param newHeadY Координата Y новой головы.
   * @return true, если яблоко съедено.
   */
  bool isAppleEaten(int newHeadX, int newHeadY) {
    return (newHeadX == apple_.first && newHeadY == apple_.second);
  }
  /**
   * @brief Обновляет тело змейки.
   * @param newHeadX Координата X новой головы.
   * @param newHeadY Координата Y новой головы.
   * @param ateApple Флаг съеденного яблока.
   */
  void updateSnakeBody(int newHeadX, int newHeadY, bool ateApple) {
    snake_.insert(snake_.begin(), {newHeadX, newHeadY});
    field_[newHeadX][newHeadY] = 1;

    if (snake_.size() >= 200) {
      fsm_.triggerEvent(SnakeEvent::WinGame);
      return;
    }

    if (!ateApple) {
      auto tail = snake_.back();
      field_[tail.first][tail.second] = 0;
      snake_.pop_back();
    } else {
      score_++;
      if (score_ / 5 > level_ - 1 && level_ < 10) {
        level_++;
      }
      saveHighScore();
      placeApple();
    }
  }
  /**
   * @brief Перемещает змейку.
   */
  void moveSnake() {
    int newHeadX, newHeadY;
    calculateNextHeadPosition(newHeadX, newHeadY);

    if (checkCollision(newHeadX, newHeadY)) {
      triggerEvent(SnakeEvent::GameOver);
      return;
    }

    bool ateApple = isAppleEaten(newHeadX, newHeadY);
    updateSnakeBody(newHeadX, newHeadY, ateApple);
  }
  /**
   * @brief Размещает яблоко на игровом поле.
   */
  void placeApple() {
    do {
      apple_.first = (rand() % ROWS) + 1;
      apple_.second = rand() % COLS;
    } while (field_[apple_.first][apple_.second] != 0);
    field_[apple_.first][apple_.second] = 2;
  }
  /**
   * @brief Проверяет, находится ли игра на паузе.
   * @return 1, если игра на паузе.
   */
  int isGamePaused() { return isGamePaused_; }
  /**
   * @brief Устанавливает состояние паузы игры.
   * @param state Новое состояние паузы.
   */
  void setPause(int state) { isGamePaused_ = state; }
  /**
   * @brief Устанавливает новое направление движения змейки.
   * @param dir Новое направление.
   */
  void setDirection(Direction dir) {
    if (!isDirectionChanged_) {
      if (dir == Direction::UP && getDirection() == Direction::DOWN ||
          dir == Direction::DOWN && getDirection() == Direction::UP ||
          dir == Direction::RIGHT && getDirection() == Direction::LEFT ||
          dir == Direction::LEFT && getDirection() == Direction::RIGHT)
        return;
      direction_ = static_cast<int>(dir);
      isDirectionChanged_ = 1;
    }
  }
  /**
   * @brief Получает текущее направление движения змейки.
   * @return Текущее направление.
   */
  Direction getDirection() { return static_cast<Direction>(direction_); }
  /**
   * @brief Изменяет направление движения.
   */
  void moveUp() {
    if (getDirection() != Direction::DOWN) {
      setDirection(Direction::UP);
    }
  }
  /**
   * @brief Изменяет направление движения.
   */
  void moveDown() {
    if (getDirection() != Direction::UP) {
      setDirection(Direction::DOWN);
    }
  }
  /**
   * @brief Изменяет направление движения.
   */
  void moveLeft() {
    if (getDirection() != Direction::RIGHT) {
      setDirection(Direction::LEFT);
    }
  }
  /**
   * @brief Изменяет направление движения.
   */
  void moveRight() {
    if (getDirection() != Direction::LEFT) {
      setDirection(Direction::RIGHT);
    }
  }
  /**
   * @brief Триггерит событие FSM.
   * @param event Событие для обработки.
   */
  void triggerEvent(SnakeEvent event) { fsm_.triggerEvent(event); }
  /**
   * @brief Получает текущее состояние FSM.
   * @return Текущее состояние FSM.
   */
  GameState getCurrentState() const { return fsm_.getCurrentState(); }
};
}  // namespace s21

#endif