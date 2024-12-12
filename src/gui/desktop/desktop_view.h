#ifndef SRC_GUI_DESKTOP_DESKTOP_VIEW_H
#define SRC_GUI_DESKTOP_DESKTOP_VIEW_H

#include <QApplication>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>

#include "brickgame_api.h"
#include "snake_controller.h"
extern "C" {
#include "tetris_controller.h"
}

using namespace s21;

class DesktopView : public QMainWindow {
  Q_OBJECT

 private:
  QWidget *centralWidget;
  QGridLayout *gridLayout;
  QLabel *gameField[FIELD_HEIGHT + 2][FIELD_WIDTH];
  QLabel *nextField[4][4];
  QLabel *scoreLabel;
  QLabel *levelLabel;
  QLabel *speedLabel;
  QLabel *recordLabel;
  QTimer *gameTimer;
  SnakeController controller;
  GameInfo_t currentState;
  bool isTetrisGameActive = false;

 public:
  DesktopView(QWidget *parent = nullptr);

 protected:
  void keyPressEvent(QKeyEvent *event) override;

 private slots:
  void updateGame();

 private:
  void showMenu();
  void showGameOver();
  void startSnakeGame();
  void setupUI();
  void updateUI();
  QWidget *setupNextField();
  QLayout *setupGameGrid();
  QLayout *setupInfoPanel();
  void startTetrisGame();
  void updateTetrisGame();
  void updateNextField();
};

#endif
