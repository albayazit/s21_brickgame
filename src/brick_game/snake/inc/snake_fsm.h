#ifndef SRC_BRICK_GAME_SNAKE_INC_SNAKE_FSM_H
#define SRC_BRICK_GAME_SNAKE_INC_SNAKE_FSM_H

#include <functional>
#include <map>
#include <vector>

namespace s21 {

/**
 * @brief Возможные события FSM для игры Snake.
 */
enum class SnakeEvent {
  StartGame,
  PauseGame,
  ResumeGame,
  MoveUp,
  MoveDown,
  MoveLeft,
  MoveRight,
  GameOver,
  EndGame,
  WinGame
};

/**
 * @brief Возможные состояния FSM для игры Snake.
 */
enum class GameState { START, PLAYING, GAMEOVER, PAUSE, WINNER };

/**
 * @brief Класс SnakeFSM реализует конечный автомат состояний для управления
 * игрой Snake.
 */
class SnakeFSM {
 private:
  GameState state_;
  std::map<SnakeEvent, std::vector<std::function<void()>>> subscribers;

 public:
  /**
   * @brief Конструктор SnakeFSM. Инициализирует начальное состояние.
   */
  SnakeFSM() : state_(GameState::START) {}

  /**
   * @brief Подписывает обработчик на указанное событие.
   * @param event Событие, на которое подписываемся.
   * @param callback Функция-обработчик события.
   */
  void subscribe(SnakeEvent event, std::function<void()> callback) {
    subscribers[event].push_back(callback);
  }
  /**
   * @brief Триггерит событие и вызывает соответствующих подписчиков.
   * @param event Событие, которое нужно обработать.
   */
  void triggerEvent(SnakeEvent event) {
    if (subscribers.find(event) != subscribers.end()) {
      for (const auto& callback : subscribers[event]) {
        callback();
      }
    }
    handleEvent(event);
  }
  /**
   * @brief Возвращает текущее состояние FSM.
   * @return Текущее состояние FSM.
   */
  GameState getCurrentState() const { return state_; }

 private:
  /**
   * @brief Обрабатывает изменение состояния в ответ на событие.
   * @param event Событие, которое вызвало изменение состояния.
   */
  void handleEvent(SnakeEvent event) {
    switch (state_) {
      case GameState::START:
        if (event == SnakeEvent::StartGame) {
          state_ = GameState::PLAYING;
        }
        break;
      case GameState::PLAYING:
        if (event == SnakeEvent::PauseGame) {
          state_ = GameState::PAUSE;
        } else if (event == SnakeEvent::GameOver) {
          state_ = GameState::GAMEOVER;
        } else if (event == SnakeEvent::WinGame) {
          state_ = GameState::WINNER;
        }
      case GameState::PAUSE:
        if (event == SnakeEvent::ResumeGame) {
          state_ = GameState::PLAYING;
        }
      default:
        break;
    }
  }
};
}  // namespace s21

#endif