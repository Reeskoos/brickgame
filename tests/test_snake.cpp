
#include <gtest/gtest.h>

#include <thread>

#include "../src/brick_game/snake/SnakeModel.h"

using namespace s21;

class TestSnake : public ::testing::Test {
 protected:
  SnakeModel snake_model;
  SnakeModel::GameData *snake_game_data;
};

TEST_F(TestSnake, MoveSnakeUp) {
  snake_model.setDefault();
  snake_game_data = &snake_model.getModelData();
  EXPECT_EQ(snake_game_data->game_state, GameState::START);
  snake_model.updateData(UserAction::SPACE_BTN);
  EXPECT_EQ(snake_game_data->game_state, GameState::SPAWN);
  snake_model.updateData(UserAction::NO_ACT);
  EXPECT_EQ(snake_game_data->game_state, GameState::MOVING);
  snake_model.updateData(UserAction::UP_BTN);
  EXPECT_EQ(snake_game_data->direction, Direction::UP);
  std::this_thread::sleep_for(std::chrono::milliseconds(510));
  snake_model.updateData(UserAction::NO_ACT);
  snake_model.updateData(UserAction::ESC_BTN);
  EXPECT_EQ(snake_game_data->game_state, GameState::EXIT);
}

TEST_F(TestSnake, MoveSnakeLeft) {
  snake_model.setDefault();
  SnakeModel::GameData *snake_game_data = &snake_model.getModelData();
  EXPECT_EQ(snake_game_data->game_state, GameState::START);
  snake_model.updateData(UserAction::SPACE_BTN);
  EXPECT_EQ(snake_game_data->game_state, GameState::SPAWN);
  snake_model.updateData(UserAction::NO_ACT);
  EXPECT_EQ(snake_game_data->game_state, GameState::MOVING);
  snake_model.updateData(UserAction::LEFT_BTN);
  std::this_thread::sleep_for(std::chrono::milliseconds(510));
  snake_model.updateData(UserAction::NO_ACT);
  snake_model.updateData(UserAction::ESC_BTN);
  EXPECT_EQ(snake_game_data->game_state, GameState::EXIT);
}

TEST_F(TestSnake, MoveSnakeRight) {
  snake_model.setDefault();
  SnakeModel::GameData *snake_game_data = &snake_model.getModelData();
  EXPECT_EQ(snake_game_data->game_state, GameState::START);
  snake_model.updateData(UserAction::SPACE_BTN);
  EXPECT_EQ(snake_game_data->game_state, GameState::SPAWN);
  snake_model.updateData(UserAction::NO_ACT);
  EXPECT_EQ(snake_game_data->game_state, GameState::MOVING);
  snake_model.updateData(UserAction::RIGHT_BTN);
  EXPECT_EQ(snake_game_data->direction, Direction::RIGHT);
  std::this_thread::sleep_for(std::chrono::milliseconds(510));
  snake_model.updateData(UserAction::NO_ACT);
  snake_model.updateData(UserAction::ESC_BTN);
  EXPECT_EQ(snake_game_data->game_state, GameState::EXIT);
}

TEST_F(TestSnake, MoveSnakeAround) {
  snake_model.setDefault();
  SnakeModel::GameData *snake_game_data = &snake_model.getModelData();
  EXPECT_EQ(snake_game_data->game_state, GameState::START);
  snake_model.updateData(UserAction::SPACE_BTN);
  EXPECT_EQ(snake_game_data->game_state, GameState::SPAWN);
  snake_model.updateData(UserAction::NO_ACT);
  EXPECT_EQ(snake_game_data->game_state, GameState::MOVING);
  snake_model.updateData(UserAction::RIGHT_BTN);
  EXPECT_EQ(snake_game_data->direction, Direction::RIGHT);
  snake_model.updateData(UserAction::DOWN_BTN);
  EXPECT_EQ(snake_game_data->direction, Direction::DOWN);
  std::this_thread::sleep_for(std::chrono::milliseconds(510));
  snake_model.updateData(UserAction::NO_ACT);
  EXPECT_EQ(snake_game_data->direction, Direction::DOWN);
  snake_model.updateData(UserAction::LEFT_BTN);
  EXPECT_EQ(snake_game_data->direction, Direction::LEFT);
  snake_model.updateData(UserAction::UP_BTN);
  EXPECT_EQ(snake_game_data->direction, Direction::UP);
  snake_model.updateData(UserAction::ESC_BTN);
  EXPECT_EQ(snake_game_data->game_state, GameState::EXIT);
}

TEST_F(TestSnake, UpdateScoreTest) {
  snake_model.setDefault();
  SnakeModel::GameData *snake_game_data = &snake_model.getModelData();
  snake_game_data->fruit_coord.x_ = snake_game_data->snake_coord[0].x_;
  snake_game_data->fruit_coord.y_ = snake_game_data->snake_coord[0].y_ - 1;
  snake_model.updateData(UserAction::SPACE_BTN);

  while (snake_game_data->game_state != GameState::GAMEOVER) {
    snake_model.updateData(UserAction::UP_BTN);
  }
}

TEST_F(TestSnake, PauseUnpauseTest) {
  snake_model.setDefault();
  SnakeModel::GameData *snake_game_data = &snake_model.getModelData();
  EXPECT_EQ(snake_game_data->game_state, GameState::START);
  snake_model.updateData(UserAction::SPACE_BTN);
  EXPECT_EQ(snake_game_data->game_state, GameState::SPAWN);
  snake_model.updateData(UserAction::NO_ACT);
  EXPECT_EQ(snake_game_data->game_state, GameState::MOVING);
  snake_model.updateData(UserAction::TAB_BTN);
  EXPECT_EQ(snake_game_data->game_state, GameState::PAUSE);
  snake_model.updateData(UserAction::TAB_BTN);
  EXPECT_EQ(snake_game_data->game_state, GameState::MOVING);
  snake_model.updateData(UserAction::ESC_BTN);
  EXPECT_EQ(snake_game_data->game_state, GameState::EXIT);
}

TEST_F(TestSnake, GameOverTest) {
  snake_model.setDefault();
  SnakeModel::GameData *snake_game_data = &snake_model.getModelData();
  EXPECT_EQ(snake_game_data->game_state, GameState::START);
  snake_model.updateData(UserAction::SPACE_BTN);
  EXPECT_EQ(snake_game_data->game_state, GameState::SPAWN);
  snake_model.updateData(UserAction::NO_ACT);
  EXPECT_EQ(snake_game_data->game_state, GameState::MOVING);
  while (snake_game_data->game_state != GameState::GAMEOVER) {
    snake_model.updateData(UserAction::UP_BTN);
  }
  EXPECT_EQ(snake_game_data->game_state, GameState::GAMEOVER);
}
