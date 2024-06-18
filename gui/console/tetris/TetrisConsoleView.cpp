#include "TetrisConsoleView.h"

namespace s21 {

TetrisConsoleView::TetrisConsoleView(TetrisController *c) : controller_(c) {
  data_ = &controller_->getModelData();
  action_ = UserAction::NO_ACT;
}

void TetrisConsoleView::Start() {
  nodelay(stdscr, TRUE);
  controller_->setModelToDefault();
  data_ = &controller_->getModelData();

  TetrisMainLoop();

  if (data_->game_state == GameState::GAMEOVER ||
      data_->game_state == GameState::EXIT) {
    renderGameOverMenu(false, data_->lvl, data_->cur_score, data_->best_score);
  }
}

void TetrisConsoleView::TetrisMainLoop() {
  while (data_->game_state != GameState::GAMEOVER &&
         data_->game_state != GameState::EXIT) {
    checkState();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    action_ = getAction();
    updateModel();
  }
}

void TetrisConsoleView::checkState() {
  if (data_->was_modified) {
    if (data_->game_state == GameState::START) {
      renderStartInfo();
    } else if (data_->game_state == GameState::PAUSE) {
      renderPauseInfo(data_->lvl, data_->cur_score, data_->best_score);
    } else {
      renderGame();
    }
  }
}

void TetrisConsoleView::updateModel() {
  controller_->updateModelData(action_);
  data_ = &controller_->getModelData();
  action_ = UserAction::NO_ACT;
}

void TetrisConsoleView::renderGame() {
  clear();
  renderGameField(WidgetChoice::TETRIS, data_->lvl, data_->cur_score,
                  data_->best_score);

  const auto &projection = data_->projection.getCords();
  const auto &cur_figure = data_->cur_figure.getCords();
  const auto &next_figure = data_->next_figure.getCords();
  const auto &game_field = data_->game_field;

  attron(COLOR_PAIR(8));
  for (const auto &item : projection) {
    mvprintw(item.y_, item.x_ + 1, ".");
  }
  attroff(COLOR_PAIR(8));

  attron(COLOR_PAIR((short)data_->cur_figure.getShape()));
  for (const auto &item : cur_figure) {
    mvprintw(item.y_, item.x_ + 1, ".");
  }
  attroff(COLOR_PAIR((short)data_->cur_figure.getShape()));

  attron(COLOR_PAIR((short)data_->next_figure.getShape()));
  for (const auto &item : next_figure) {
    mvprintw(item.y_ + 2, item.x_ + 11, ".");
  }
  attroff(COLOR_PAIR((short)data_->next_figure.getShape()));

  for (int i = 0; i < ConstSizes::field_height; ++i) {
    for (int j = 0; j < ConstSizes::field_width; ++j) {
      attron(COLOR_PAIR(game_field[i][j].second));
      if (game_field[i][j].first) mvprintw(i + 1, j + 1, ".");
      attroff(COLOR_PAIR(game_field[i][j].second));
    }
  }
}
}  // namespace s21