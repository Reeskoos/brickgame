#include "TetrisModel.h"
#include <algorithm>
namespace s21 {

TetrisModel::TetrisModel() {
  initEventHandlersMatrix();
  setDefault();
  tetris_data_.best_score = readBestScore("TetrisScore.txt");
}

TetrisModel::~TetrisModel() {
  writeBestScore(tetris_data_.best_score, "TetrisScore.txt");
  tetris_data_.game_field.clear();
}

void TetrisModel::initEventHandlersMatrix() {
  /* START STATE */
  EventsMatrix[GameState::START][UserAction::SPACE_BTN] = &TetrisModel::Start;
  EventsMatrix[GameState::START][UserAction::ENTER_BTN] = &TetrisModel::Start;
  EventsMatrix[GameState::START][UserAction::ESC_BTN] = &TetrisModel::ExitGame;

  /* |SPAWN|COLLIDE|EXIT|GAMEOVER| STATES */
  for (int i = 0; i < USER_ACTIONS_CNT; i++) {
    EventsMatrix[GameState::SPAWN][static_cast<UserAction>(i)] =
        &TetrisModel::SpawnFigure;

    EventsMatrix[GameState::COLLIDE][static_cast<UserAction>(i)] =
        &TetrisModel::Collide;

    EventsMatrix[GameState::EXIT][static_cast<UserAction>(i)] =
        &TetrisModel::ExitGame;

    EventsMatrix[GameState::GAMEOVER][static_cast<UserAction>(i)] =
        &TetrisModel::GameOver;
  }

  /* MOVING STATE */
  EventsMatrix[GameState::MOVING][UserAction::UP_BTN] =
      &TetrisModel::RotateFigure;
  EventsMatrix[GameState::MOVING][UserAction::DOWN_BTN] =
      &TetrisModel::MoveFigureDown;
  EventsMatrix[GameState::MOVING][UserAction::RIGHT_BTN] =
      &TetrisModel::MoveFigureRight;
  EventsMatrix[GameState::MOVING][UserAction::LEFT_BTN] =
      &TetrisModel::MoveFigureLeft;
  EventsMatrix[GameState::MOVING][UserAction::SPACE_BTN] =
      &TetrisModel::DropFigure;
  EventsMatrix[GameState::MOVING][UserAction::TAB_BTN] = &TetrisModel::Pause;
  EventsMatrix[GameState::MOVING][UserAction::ESC_BTN] = &TetrisModel::ExitGame;

  /* PAUSE STATE */
  EventsMatrix[GameState::PAUSE][UserAction::ENTER_BTN] = &TetrisModel::Unpause;
  EventsMatrix[GameState::PAUSE][UserAction::TAB_BTN] = &TetrisModel::Unpause;
  EventsMatrix[GameState::PAUSE][UserAction::ESC_BTN] = &TetrisModel::ExitGame;
}

void TetrisModel::updateData(UserAction action) {
  tetris_data_.was_modified = false;
  GameData tmp = tetris_data_;

  long long cur_time_ = last_move_time_;
  if (tetris_data_.game_state != GameState::PAUSE) cur_time_ = getCurTime();

  using Action = void (TetrisModel::*)();
  Action cur_func = EventsMatrix[tetris_data_.game_state][action];

  if (cur_func) (this->*cur_func)();

  if (tetris_data_.game_state == GameState::MOVING) {
    if (cur_time_ - last_move_time_ >= cur_interval_) {
      last_move_time_ = cur_time_;
      if (checkCollision()) updateField();
      MoveFigureDown();
    }
    updateLvl();
  }

  if (tetris_data_ != tmp) tetris_data_.was_modified = true;
}

void TetrisModel::updateField() {
  placeFigureUp();
  updateScore(checkCompleteLines());
  updateLvl();
  SpawnFigure();
}

void TetrisModel::updateScore(size_t lines) {
  switch (lines) {
    case 1:
      tetris_data_.cur_score += 100;
      break;
    case 2:
      tetris_data_.cur_score += 300;
      break;
    case 3:
      tetris_data_.cur_score += 700;
      break;
    case 4:
      tetris_data_.cur_score += 1500;
      break;
  }

  if (tetris_data_.cur_score >= tetris_data_.best_score) {
    tetris_data_.best_score = tetris_data_.cur_score;
  }
}

void TetrisModel::updateLvl() {
  if (tetris_data_.lvl < 10 && tetris_data_.cur_score >= 600) {
    tetris_data_.lvl = (tetris_data_.cur_score / 600) + 1;
  }
  cur_interval_ = ConstSizes::levels_intervals_ms[tetris_data_.lvl - 1];
}

size_t TetrisModel::checkCompleteLines() {
  size_t cnt = 0;

  for (int i = 0; i < ConstSizes::field_height; ++i) {
    bool completed = true;
    for (int j = 0; j < ConstSizes::field_width; ++j) {
      if (!tetris_data_.game_field[i][j].first) {
        completed = false;
        break;
      }
    }
    if (completed) {
      clearLine(i);
      cnt++;
    }
  }
  return cnt;
}

void TetrisModel::clearLine(int i) {
  tetris_data_.game_field.erase(tetris_data_.game_field.begin() + i);
  tetris_data_.game_field.insert(
      tetris_data_.game_field.begin(),
      std::vector<std::pair<bool, int>>(ConstSizes::field_width, {false, 0}));
}

TetrisModel::GameData& TetrisModel::getModelData() { return tetris_data_; }

void TetrisModel::placeFigureUp() {
  for (const auto& cords : tetris_data_.cur_figure.getCords()) {
    tetris_data_.game_field[cords.y_ - 1][cords.x_].first = true;
    tetris_data_.game_field[cords.y_ - 1][cords.x_].second =
        static_cast<int>(tetris_data_.cur_figure.getShape());
  }
}

void TetrisModel::setDefault() {
  tetris_data_.cur_score = 0;
  tetris_data_.lvl = 1;
  tetris_data_.game_state = GameState::START;
  tetris_data_.cur_figure.setRandomShape();
  tetris_data_.next_figure.setRandomShape();
  tetris_data_.projection = tetris_data_.cur_figure;

  initField();
  initProjection();

  last_move_time_ = getCurTime();
  cur_interval_ = ConstSizes::levels_intervals_ms[0];
}

void TetrisModel::initField() {
  tetris_data_.game_field.resize(
      ConstSizes::field_height,
      std::vector<std::pair<bool, int>>(ConstSizes::field_width, {false, 0}));
  for (int i = 0; i < ConstSizes::field_height; ++i) {
    for (int j = 0; j < ConstSizes::field_width; ++j) {
      tetris_data_.game_field[i][j].first = false;
      tetris_data_.game_field[i][j].second = static_cast<int>(Shape::EMPTY);
    }
  }
}

void TetrisModel::initProjection() {
  tetris_data_.projection = tetris_data_.cur_figure;
  while (FigureCanMove(tetris_data_.projection, UserAction::DOWN_BTN)) {
    tetris_data_.projection.moveDown();
  }
}

bool TetrisModel::FigureCanMove(const Figure& piece, UserAction action) {
  bool res = true;
  for (const auto& cords : piece.getCords()) {
    switch (action) {
      case UserAction::DOWN_BTN:
        if (cords.y_ == ConstSizes::field_height ||
            tetris_data_.game_field[cords.y_][cords.x_].first) {
          res = false;
        }
        break;
      case UserAction::LEFT_BTN:
        if (cords.x_ < 1 ||
            tetris_data_.game_field[cords.y_ - 1][cords.x_ - 1].first) {
          res = false;
        }
        break;
      case UserAction::RIGHT_BTN:
        if (cords.x_ == ConstSizes::field_width - 1 ||
            tetris_data_.game_field[cords.y_ - 1][cords.x_ + 1].first) {
          res = false;
        }
        break;
      default:
        break;
    }
    if (!res) break;
  }
  return res;
}

bool TetrisModel::checkCollision() {
  bool collision = false;
  for (const auto& cords : tetris_data_.cur_figure.getCords()) {
    if (cords.y_ == ConstSizes::field_height) {
      collision = true;
      break;
    }
    if (tetris_data_.game_field[cords.y_][cords.x_].first) {
      collision = true;
      break;
    }
  }
  return collision;
}

void TetrisModel::SpawnFigure() {
  tetris_data_.game_state = GameState::MOVING;
  tetris_data_.cur_figure = tetris_data_.next_figure;
  tetris_data_.next_figure.setRandomShape();
  tetris_data_.projection = tetris_data_.cur_figure;
  initProjection();
  if (checkCollision()) tetris_data_.game_state = GameState::GAMEOVER;
}

void TetrisModel::MoveFigureLeft() {
  if (FigureCanMove(tetris_data_.cur_figure, UserAction::LEFT_BTN)) {
    tetris_data_.cur_figure.moveLeft();
    initProjection();
  }
}

void TetrisModel::MoveFigureRight() {
  if (FigureCanMove(tetris_data_.cur_figure, UserAction::RIGHT_BTN)) {
    tetris_data_.cur_figure.moveRight();
    initProjection();
  }
}

void TetrisModel::MoveFigureDown() {
  if (FigureCanMove(tetris_data_.cur_figure, UserAction::DOWN_BTN)) {
    tetris_data_.cur_figure.moveDown();
    initProjection();
  }
}

void TetrisModel::DropFigure() {
  tetris_data_.cur_figure = tetris_data_.projection;
  updateField();
}

void TetrisModel::RotateFigure() {
  auto temp = tetris_data_.cur_figure;
  tetris_data_.cur_figure.Rotate();

  const auto& cords = tetris_data_.cur_figure.getCords();

  bool rotate_possible =
      !std::any_of(cords.begin(), cords.end(), [this](const auto& cord) {
        return tetris_data_.game_field[cord.y_ - 1][cord.x_].first;
      });

  if (!rotate_possible) {
    tetris_data_.cur_figure = temp;
  }
  initProjection();
}

void TetrisModel::Start() {
  setDefault();
  tetris_data_.game_state = GameState::SPAWN;
}

void TetrisModel::ExitGame() { tetris_data_.game_state = GameState::EXIT; }

void TetrisModel::Pause() { tetris_data_.game_state = GameState::PAUSE; }

void TetrisModel::Unpause() { tetris_data_.game_state = GameState::MOVING; }

void TetrisModel::Collide() {
  tetris_data_.game_state = GameState::SPAWN;
  updateField();
}

void TetrisModel::GameOver() { tetris_data_.game_state = GameState::EXIT; }

}  // namespace s21
