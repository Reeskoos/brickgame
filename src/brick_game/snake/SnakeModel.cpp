#include "SnakeModel.h"

namespace s21 {

SnakeModel::SnakeModel() : snake_data_() {
  initEventHandlersMatrix();
  snake_data_.best_score = readBestScore("SnakeScore.txt");
};

SnakeModel::~SnakeModel() {
  writeBestScore(snake_data_.best_score, "SnakeScore.txt");
  snake_data_.snake_coord.clear();
};

void SnakeModel::initEventHandlersMatrix() {
  /* START STATE */
  EventsMatrix[GameState::START][UserAction::SPACE_BTN] = &SnakeModel::Start;
  EventsMatrix[GameState::START][UserAction::ENTER_BTN] = &SnakeModel::Start;
  EventsMatrix[GameState::START][UserAction::ESC_BTN] = &SnakeModel::ExitGame;

  /* |SPAWN|COLLIDE|EXIT|GAMEOVER| STATES */
  for (int i = 0; i < USER_ACTIONS_CNT; i++) {
    EventsMatrix[GameState::SPAWN][static_cast<UserAction>(i)] =
        &SnakeModel::Spawn;

    EventsMatrix[GameState::COLLIDE][static_cast<UserAction>(i)] =
        &SnakeModel::Collide;

    EventsMatrix[GameState::EXIT][static_cast<UserAction>(i)] =
        &SnakeModel::ExitGame;

    EventsMatrix[GameState::GAMEOVER][static_cast<UserAction>(i)] =
        &SnakeModel::GameOver;
  }

  /* MOVING STATE */
  EventsMatrix[GameState::MOVING][UserAction::UP_BTN] = &SnakeModel::MoveHeadUp;
  EventsMatrix[GameState::MOVING][UserAction::DOWN_BTN] =
      &SnakeModel::MoveHeadDown;
  EventsMatrix[GameState::MOVING][UserAction::RIGHT_BTN] =
      &SnakeModel::MoveHeadRight;
  EventsMatrix[GameState::MOVING][UserAction::LEFT_BTN] =
      &SnakeModel::MoveHeadLeft;
  EventsMatrix[GameState::MOVING][UserAction::SPACE_BTN] = &SnakeModel::Pause;
  EventsMatrix[GameState::MOVING][UserAction::TAB_BTN] = &SnakeModel::Pause;
  EventsMatrix[GameState::MOVING][UserAction::ESC_BTN] = &SnakeModel::ExitGame;

  /* PAUSE STATE */
  EventsMatrix[GameState::PAUSE][UserAction::ENTER_BTN] = &SnakeModel::Unpause;
  EventsMatrix[GameState::PAUSE][UserAction::TAB_BTN] = &SnakeModel::Unpause;
  EventsMatrix[GameState::PAUSE][UserAction::ESC_BTN] = &SnakeModel::ExitGame;
}

SnakeModel::GameData& SnakeModel::getModelData() { return snake_data_; };

void SnakeModel::updateData(UserAction action) {
  snake_data_.was_modified = false;
  GameData tmp = snake_data_;

  cur_time_ = last_move_time_;
  if (snake_data_.game_state != GameState::PAUSE) cur_time_ = getCurTime();

  using Action = void (SnakeModel::*)();
  Action cur_func = EventsMatrix[snake_data_.game_state][action];

  if (cur_func) (this->*cur_func)();

  if (snake_data_.game_state == GameState::MOVING) {
    if (cur_time_ - last_move_time_ >= cur_interval_) moveSnake();
  }

  if (snake_data_ != tmp) snake_data_.was_modified = true;
}

void SnakeModel::setDefault() {
  cur_interval_ = ConstSizes::levels_intervals_ms[0];
  cur_time_ = getCurTime();
  last_move_time_ = cur_time_;
  snake_data_.cur_score = 0;
  snake_data_.lvl = 1;
  snake_data_.direction = Direction::UP;
  snake_data_.game_state = GameState::START;
  snake_data_.snake_coord.clear();
  snake_data_.snake_coord.reserve(200);

  snake_data_.snake_coord.push_back({5, ConstSizes::field_height / 2});
  snake_data_.snake_coord.push_back({5, ConstSizes::field_height / 2 + 1});
  snake_data_.snake_coord.push_back({5, ConstSizes::field_height / 2 + 2});
  snake_data_.snake_coord.push_back({5, ConstSizes::field_height / 2 + 3});

  updateFruitPos();
}

void SnakeModel::updateFruitPos() {
  snake_data_.fruit_coord.randomCords();
  for (const auto& i : snake_data_.snake_coord) {
    if (snake_data_.fruit_coord == i) {
      updateFruitPos();
    }
  }
}

void SnakeModel::updateScores() {
  snake_data_.cur_score++;

  if (snake_data_.cur_score > snake_data_.best_score) {
    snake_data_.best_score = snake_data_.cur_score;
  }
  if (snake_data_.cur_score % 5 == 0 && snake_data_.lvl < 10) {
    snake_data_.lvl++;
    cur_interval_ = ConstSizes::levels_intervals_ms[snake_data_.lvl - 1];
  }

  if (snake_data_.cur_score == 200) {
    snake_data_.win = 1;
    snake_data_.game_state = GameState::GAMEOVER;
  }
}

void SnakeModel::checkEating() {
  if (snake_data_.snake_coord[0] == snake_data_.fruit_coord) {
    snake_data_.snake_coord.push_back(snake_data_.snake_coord.back());
    updateScores();
    updateFruitPos();
  }
}

void SnakeModel::checkCollision() {
  for (size_t i = 1; i < snake_data_.snake_coord.size(); ++i) {
    if (snake_data_.snake_coord[0] == snake_data_.snake_coord[i]) {
      snake_data_.game_state = GameState::COLLIDE;
    }
  }
  if (snake_data_.snake_coord[0].x_ < 0 ||
      snake_data_.snake_coord[0].x_ >= ConstSizes::field_width) {
    snake_data_.game_state = GameState::COLLIDE;
  }

  if (snake_data_.snake_coord[0].y_ < 0 ||
      snake_data_.snake_coord[0].y_ >= ConstSizes::field_height) {
    snake_data_.game_state = GameState::COLLIDE;
  }
}

void SnakeModel::moveSnake() {
  switch (snake_data_.direction) {
    case Direction::UP:
      MoveHeadUp();
      break;
    case Direction::DOWN:
      MoveHeadDown();
      break;
    case Direction::RIGHT:
      MoveHeadRight();
      break;
    case Direction::LEFT:
      MoveHeadLeft();
      break;
  }
}

void SnakeModel::MoveBody() {
  for (auto i = snake_data_.snake_coord.size(); i > 0; --i) {
    snake_data_.snake_coord[i] = snake_data_.snake_coord[i - 1];
  }
}

void SnakeModel::MoveHead(Direction direction) {
  if (IsOppositeDirection(direction)) return;
  snake_data_.direction = direction;
  MoveBody();
  UpdateHeadPosition(direction);
  checkEating();
  checkCollision();
  last_move_time_ = cur_time_;
}

void SnakeModel::UpdateHeadPosition(Direction direction) {
  switch (direction) {
    case Direction::UP:
      snake_data_.snake_coord[0].y_ -= 1;
      break;
    case Direction::DOWN:
      snake_data_.snake_coord[0].y_ += 1;
      break;
    case Direction::LEFT:
      snake_data_.snake_coord[0].x_ -= 1;
      break;
    case Direction::RIGHT:
      snake_data_.snake_coord[0].x_ += 1;
      break;
  }
}

void SnakeModel::MoveHeadUp() { MoveHead(Direction::UP); }

void SnakeModel::MoveHeadDown() { MoveHead(Direction::DOWN); }

void SnakeModel::MoveHeadLeft() { MoveHead(Direction::LEFT); }

void SnakeModel::MoveHeadRight() { MoveHead(Direction::RIGHT); }

bool SnakeModel::IsOppositeDirection(Direction new_direction) {
  return (snake_data_.direction == Direction::UP &&
          new_direction == Direction::DOWN) ||
         (snake_data_.direction == Direction::DOWN &&
          new_direction == Direction::UP) ||
         (snake_data_.direction == Direction::LEFT &&
          new_direction == Direction::RIGHT) ||
         (snake_data_.direction == Direction::RIGHT &&
          new_direction == Direction::LEFT);
}

void SnakeModel::Start() { snake_data_.game_state = GameState::SPAWN; }

void SnakeModel::Spawn() { snake_data_.game_state = GameState::MOVING; }

void SnakeModel::Pause() { snake_data_.game_state = GameState::PAUSE; }

void SnakeModel::Unpause() { snake_data_.game_state = GameState::MOVING; }

void SnakeModel::GameOver() { snake_data_.game_state = GameState::GAMEOVER; }

void SnakeModel::ExitGame() { snake_data_.game_state = GameState::EXIT; }

void SnakeModel::Collide() { snake_data_.game_state = GameState::GAMEOVER; }

}  // namespace s21