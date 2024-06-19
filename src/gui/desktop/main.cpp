#include <QApplication>

#include "../../brick_game/snake/SnakeModel.h"
#include "../../controller/Controller.h"
#include "mainwindow.h"
// using namespace s21;
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  s21::SnakeModel snake_model;
  s21::TetrisModel tetris_model;

  s21::Controller<s21::SnakeModel, s21::UserAction::UP_BTN> snake_controller(
      &snake_model);
  s21::Controller<s21::TetrisModel, s21::UserAction::NO_ACT> tetris_controller(
      &tetris_model);

  s21::MainWindow w(&snake_controller, &tetris_controller);
  w.show();
  return a.exec();
}
