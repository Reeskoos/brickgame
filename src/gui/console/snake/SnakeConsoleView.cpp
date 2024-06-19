#include "SnakeConsoleView.h"

namespace s21 {

SnakeConsoleView::SnakeConsoleView(SnakeController* s_c)
    : action_(UserAction::NO_ACT), data_(), controller_(s_c) {}

void SnakeConsoleView::Start() {
  nodelay(stdscr, TRUE);
  controller_->setModelToDefault();
  data_ = &controller_->getModelData();
  SnakeMainLoop();

  if (data_->game_state == GameState::GAMEOVER ||
      data_->game_state == GameState::EXIT) {
    renderGameOverMenu(data_->win, data_->lvl, data_->cur_score,
                       data_->best_score);
  }
}

void SnakeConsoleView::checkState() {
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

void SnakeConsoleView::SnakeMainLoop() {
  while (data_->game_state != GameState::GAMEOVER &&
         data_->game_state != GameState::EXIT) {
    checkState();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    action_ = getAction();
    updateModel();
  }
}

void SnakeConsoleView::updateModel() {
  controller_->updateModelData(action_);
  data_ = &controller_->getModelData();
  action_ = UserAction::NO_ACT;
}
void SnakeConsoleView::renderGame() {
  clear();

  /* Food */
  attron(COLOR_PAIR(1));
  mvprintw(data_->fruit_coord.y_ + 1, data_->fruit_coord.x_ + 1, "%c", '1');
  attroff(COLOR_PAIR(1));

  /* Snake */
  attron(COLOR_PAIR(2));

  std::string scoreStr = std::to_string(data_->cur_score);
  size_t scoreLen = scoreStr.length();
  size_t snakeLen = data_->snake_coord.size();

  for (size_t i = 0; i < snakeLen; ++i) {
    char ch = (i < scoreLen) ? scoreStr[i] : '0';
    mvprintw(data_->snake_coord[i].y_ + 1, data_->snake_coord[i].x_ + 1, "%c",
             ch);
  }
  attroff(COLOR_PAIR(2));

  renderGameField(WidgetChoice::SNAKE, data_->lvl, data_->cur_score,
                  data_->best_score);
  refresh();
}

}  // namespace s21