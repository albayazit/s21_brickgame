#include "desktop_view.h"

#include <QDebug>

/**
 * @brief Основная функция, запускающая приложение и отображающее окно игры.
 *
 * @param argc Количество аргументов командной строки.
 * @param argv Массив строк с аргументами командной строки.
 * @return Статус завершения приложения.
 */
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  DesktopView window;
  window.setWindowTitle("Brickgame");
  window.show();
  return app.exec();
}

/**
 * @brief Конструктор класса DesktopView, отображающий главное меню.
 */
DesktopView::DesktopView(QWidget *parent) : QMainWindow(parent) { showMenu(); }

/**
 * @brief Отображает главное меню с кнопками для выбора игры.
 */
void DesktopView::showMenu() {
  if (centralWidget) {
    delete centralWidget;
  }
  centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  QVBoxLayout *menuLayout = new QVBoxLayout(centralWidget);
  QLabel *titleLabel = new QLabel("BrickGame", this);
  titleLabel->setAlignment(Qt::AlignCenter);
  titleLabel->setStyleSheet(
      "font-size: 24px; font-weight: bold; margin-bottom: 20px;");
  menuLayout->addWidget(titleLabel);
  QPushButton *snakeButton = new QPushButton("Snake", this);
  connect(snakeButton, &QPushButton::clicked, this,
          [this]() { startSnakeGame(); });
  menuLayout->addWidget(snakeButton);
  QPushButton *tetrisButton = new QPushButton("Tetris", this);
  connect(tetrisButton, &QPushButton::clicked, this,
          [this]() { startTetrisGame(); });
  menuLayout->addWidget(tetrisButton);
  menuLayout->setAlignment(Qt::AlignTop);
  resize(400, 200);
}

/**
 * @brief Отображает окно с сообщением о завершении игры.
 */
void DesktopView::showGameOver() {
  if (centralWidget) {
    delete centralWidget;
  }
  centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  QVBoxLayout *menuLayout = new QVBoxLayout(centralWidget);
  QLabel *titleLabel = new QLabel("GameOver", this);
  titleLabel->setAlignment(Qt::AlignCenter);
  titleLabel->setStyleSheet(
      "font-size: 24px; font-weight: bold; margin-bottom: 20px;");
  menuLayout->addWidget(titleLabel);
  if (isTetrisGameActive) {
    tetrisControllerFreeMemory();
  }
  QPushButton *snakeButton = new QPushButton("Quit", this);
  connect(snakeButton, &QPushButton::clicked, this,
          [this]() { QApplication::quit(); });
  menuLayout->addWidget(snakeButton);
}

/**
 * @brief Запускает игру Snake.
 */
void DesktopView::startSnakeGame() {
  if (centralWidget) {
    delete centralWidget;
  }
  centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  setupUI();
  SnakeController controller;
  currentState = controller.updateCurrentState();
  updateUI();
  gameTimer = new QTimer(this);
  connect(gameTimer, &QTimer::timeout, this, &DesktopView::updateGame);
  gameTimer->start(100);
}

/**
 * @brief Настроит UI для игры.
 */
void DesktopView::setupUI() {
  centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
  if (isTetrisGameActive) {
    QWidget *nextFieldContainer = setupNextField();
    mainLayout->addWidget(nextFieldContainer, 0, Qt::AlignCenter);
  }

  QLayout *gameGrid = setupGameGrid();
  mainLayout->addLayout(gameGrid);

  QLayout *infoLayout = setupInfoPanel();
  mainLayout->addLayout(infoLayout);
}

/**
 * @brief Настроит и возвращает виджет для отображения следующего тетромино.
 */
QWidget *DesktopView::setupNextField() {
  QGridLayout *nextFieldLayout = new QGridLayout();
  QWidget *nextFieldContainer = new QWidget(this);
  nextFieldContainer->setLayout(nextFieldLayout);
  nextFieldContainer->setFixedSize(100, 100);

  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      nextField[row][col] = new QLabel(this);
      nextField[row][col]->setFixedSize(20, 20);
      nextField[row][col]->setStyleSheet(
          "background-color: black; border: 1px solid gray;");
      nextFieldLayout->addWidget(nextField[row][col], row, col);
    }
  }

  return nextFieldContainer;
}

/**
 * @brief Настроит и возвращает макет для игрового поля.
 */
QLayout *DesktopView::setupGameGrid() {
  QGridLayout *gameGrid = new QGridLayout();
  gameGrid->setSpacing(1);

  for (int row = 0; row < FIELD_HEIGHT + 1; ++row) {
    for (int col = 0; col < FIELD_WIDTH; ++col) {
      gameField[row][col] = new QLabel(centralWidget);
      if (row != 0) {
        gameField[row][col]->setFixedSize(20, 20);
        gameField[row][col]->setStyleSheet(
            "background-color: black; border: 0px solid gray;");
        gameGrid->addWidget(gameField[row][col], row, col);
      }
    }
  }

  return gameGrid;
}

/**
 * @brief Настроит и возвращает макет панели информации.
 */
QLayout *DesktopView::setupInfoPanel() {
  QVBoxLayout *infoLayout = new QVBoxLayout();
  scoreLabel = new QLabel("Score: 0", this);
  levelLabel = new QLabel("Level: 1", this);
  speedLabel = new QLabel("Speed: 1", this);
  recordLabel = new QLabel("Record: 0", this);
  infoLayout->addWidget(scoreLabel);
  infoLayout->addWidget(levelLabel);
  infoLayout->addWidget(speedLabel);
  infoLayout->addWidget(recordLabel);
  infoLayout->addStretch();
  return infoLayout;
}

/**
 * @brief Обновляет состояние игры каждое обновление таймера.
 */
void DesktopView::updateGame() {
  currentState = controller.updateCurrentState();
  if (controller.isGameOver()) {
    gameTimer->stop();
    showGameOver();
    return;
  }
  if (controller.isWinner()) {
    gameTimer->stop();
    QMessageBox::information(this, "Winner!", "You won!");
    return;
  }
  updateUI();
}

/**
 * @brief Обновляет отображение UI, включая игровое поле и информацию.
 */
void DesktopView::updateUI() {
  for (int row = 1; row < FIELD_HEIGHT + 1; ++row) {
    for (int col = 0; col < FIELD_WIDTH; ++col) {
      int cellValue = currentState.field[row][col];
      QString color;
      if (cellValue == 0) {
        color = "black";
      } else if (cellValue == 1) {
        color = "green";
      } else {
        color = "red";
      }
      gameField[row][col]->setStyleSheet(
          QString("background-color: %1; border: 1px solid gray;").arg(color));
    }
  }

  if (isTetrisGameActive) {
    updateNextField();
  }

  scoreLabel->setText(QString("Score: %1").arg(currentState.score));
  levelLabel->setText(QString("Level: %1").arg(currentState.level));
  speedLabel->setText(QString("Speed: %1").arg(currentState.speed));
  recordLabel->setText(QString("Record: %1").arg(currentState.high_score));
}

/**
 * @brief Обновляет отображение следующего тетромино.
 */
void DesktopView::updateNextField() {
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      nextField[row][col]->setStyleSheet(
          "background-color: black; border: 1px solid gray;");
    }
  }

  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      if (currentState.next[row][col] == 1) {
        QString color = "green";
        nextField[row][col]->setStyleSheet(
            QString("background-color: %1; border: 1px solid gray;")
                .arg(color));
      }
    }
  }
}

/**
 * @brief Обработчик нажатия клавиш для управления игрой.
 *
 * @param event Событие нажатия клавиши.
 */
void DesktopView::keyPressEvent(QKeyEvent *event) {
  UserAction_t action;
  switch (event->key()) {
    case Qt::Key_Up:
      action = Up;
      break;
    case Qt::Key_Down:
      action = Down;
      break;
    case Qt::Key_Left:
      action = Left;
      break;
    case Qt::Key_Right:
      action = Right;
      break;
    case Qt::Key_Space:
      action = Action;
      break;
    case Qt::Key_P:
      action = Pause;
      break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
      action = Start;
      break;
    case Qt::Key_Q:
      action = Terminate;
      break;
    default:
      return;
  }
  if (action == Terminate) {
    gameTimer->stop();
    showGameOver();
  } else if (isTetrisGameActive) {
    tetrisControllerUserInput(&action, false);
  } else {
    controller.userInput(action, false);
  }
}

/**
 * @brief Запускает игру Tetris.
 */
void DesktopView::startTetrisGame() {
  if (centralWidget) {
    delete centralWidget;
  }
  centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  isTetrisGameActive = true;
  setupUI();
  currentState = tetrisControllerUpdateGame();
  updateUI();
  gameTimer = new QTimer(this);
  connect(gameTimer, &QTimer::timeout, this, &DesktopView::updateTetrisGame);
  gameTimer->start(20);
}

/**
 * @brief Обновляет состояние игры Tetris.
 */
void DesktopView::updateTetrisGame() {
  currentState = tetrisControllerUpdateGame();
  if (!tetrisControllerIsGameOver()) {
    gameTimer->stop();
    showGameOver();
    return;
  }
  updateUI();
}