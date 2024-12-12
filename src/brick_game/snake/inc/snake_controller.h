#ifndef SRC_BRICK_GAME_SNAKE_INC_SNAKE_CONTROLLER_H
#define SRC_BRICK_GAME_SNAKE_INC_SNAKE_CONTROLLER_H

#include <chrono>

#include "brickgame_api.h"
#include "snake_game.h"

namespace s21 {

/**
 * @brief Конструктор SnakeController. Инициализирует игру и задаёт начальный
 * интервал кадров.
 */
class SnakeController {
 private:
  SnakeGame game_;
  std::chrono::steady_clock::time_point lastUpdateTime_;
  double frameInterval_;

 public:
  SnakeController()
      : game_(),
        frameInterval_(0.3),
        lastUpdateTime_(std::chrono::steady_clock::now()) {}

  /**
   * @brief Обрабатывает пользовательский ввод и выполняет соответствующее
   * действие.
   * @param action Действие пользователя.
   * @param hold Удерживается ли действие.
   */
  void userInput(UserAction_t action, bool hold) {
    switch (action) {
      case Start:
        if (game_.getCurrentState() != GameState::PLAYING) {
          game_.triggerEvent(SnakeEvent::StartGame);
        }
        break;
      case Pause:
        if (game_.isGamePaused()) {
          game_.triggerEvent(SnakeEvent::ResumeGame);
        } else {
          game_.triggerEvent(SnakeEvent::PauseGame);
        }
        break;
      case Up:
        if ((game_.getCurrentState() == GameState::PLAYING)) {
          game_.moveUp();
        }
        break;
      case Down:
        if ((game_.getCurrentState() == GameState::PLAYING)) {
          game_.moveDown();
        }
        break;
      case Left:
        if ((game_.getCurrentState() == GameState::PLAYING)) {
          game_.moveLeft();
        }
        break;
      case Right:
        if ((game_.getCurrentState() == GameState::PLAYING)) {
          game_.moveRight();
        }
        break;
      case Action:
        if ((game_.getCurrentState() == GameState::PLAYING)) {
          game_.update();
        }
        break;
      default:
        break;
    }
  }
  /**
   * @brief Обновляет текущее состояние игры.
   * @return Информация о состоянии игры, включая поле, счёт, уровень и статус.
   */
  GameInfo_t updateCurrentState() {
    GameInfo_t info;
    auto now = std::chrono::steady_clock::now();
    double elapsedTime =
        std::chrono::duration_cast<std::chrono::duration<double>>(
            now - lastUpdateTime_)
            .count();
    updateFrameInterval();
    if (elapsedTime >= frameInterval_) {
      lastUpdateTime_ = now;
      if ((game_.getCurrentState() == GameState::PLAYING)) {
        game_.update();
      }
    }
    info.field = const_cast<int**>(
        reinterpret_cast<const int* const*>(game_.getField()));
    info.score = game_.getScore();
    info.level = game_.getLevel();
    info.pause = game_.isGamePaused();
    info.speed = game_.getLevel();
    info.high_score = game_.getHighScore();
    return info;
  }
  /**
   * @brief Обновляет интервал кадров в зависимости от уровня игры.
   */
  void updateFrameInterval() {
    double minInterval = 0.1;
    double maxInterval = 0.3;
    frameInterval_ = maxInterval - ((game_.getLevel() - 1) / 9.0) *
                                       (maxInterval - minInterval);
  }
  /**
   * @brief Проверяет, окончена ли игра.
   * @return 1, если игра окончена, иначе 0.
   */
  int isGameOver() const {
    return game_.getCurrentState() == GameState::GAMEOVER ? 1 : 0;
  }
  /**
   * @brief Проверяет, какой сейчас статус у игры.
   * @return 1, если игра идет, иначе 0.
   */
  int isPlaying() const {
    return game_.getCurrentState() == GameState::PLAYING ? 1 : 0;
  }
  /**
   * @brief Проверяет, выиграл ли игрок.
   * @return 1, если игрок выиграл, иначе 0.
   */
  int isWinner() const {
    return game_.getCurrentState() == GameState::WINNER ? 1 : 0;
  }
};

}  // namespace s21
#endif