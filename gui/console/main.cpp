
#include "ConsoleView.h"

using namespace s21;
int main() {
  using SnakeController = Controller<SnakeModel, UserAction::UP_BTN>;
  using TetrisController = Controller<TetrisModel, UserAction::NO_ACT>;

  SnakeModel snake_model;
  SnakeController snake_controller(&snake_model);

  TetrisModel tetris_model;
  TetrisController tetris_controller(&tetris_model);

  ConsoleView view(&snake_controller, &tetris_controller);
  view.Start();
  return 0;
}