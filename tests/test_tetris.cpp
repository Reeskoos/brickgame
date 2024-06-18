
#include <gtest/gtest.h>

#include <thread>

#include "../brick_game/tetris/TetrisModel.h"

using namespace s21;

class TetrisGameTest : public ::testing::Test {
 protected:
  TetrisModel tetris_model;
  TetrisModel::GameData *game_data;
};

TEST_F(TetrisGameTest, MoveFigure) {
  tetris_model.setDefault();
  game_data = &tetris_model.getModelData();
  EXPECT_EQ(game_data->game_state, GameState::START);
  tetris_model.updateData(UserAction::SPACE_BTN);
  EXPECT_EQ(game_data->game_state, GameState::SPAWN);
  tetris_model.updateData(UserAction::NO_ACT);
  EXPECT_EQ(game_data->game_state, GameState::MOVING);

  auto expected_cords = game_data->cur_figure.getCords();
  tetris_model.updateData(UserAction::LEFT_BTN);
  EXPECT_TRUE(expected_cords != game_data->cur_figure.getCords());
  tetris_model.updateData(UserAction::RIGHT_BTN);
  EXPECT_TRUE(expected_cords == game_data->cur_figure.getCords());

  std::this_thread::sleep_for(std::chrono::milliseconds(510));
  tetris_model.updateData(UserAction::NO_ACT);

  for (size_t i = 0; i < expected_cords.size(); ++i) {
    EXPECT_EQ(expected_cords[i].y_ + 1, game_data->cur_figure.getCords()[i].y_);
  }

  tetris_model.updateData(UserAction::ESC_BTN);
  EXPECT_EQ(game_data->game_state, GameState::EXIT);
}

TEST_F(TetrisGameTest, DropFigure) {
  tetris_model.setDefault();
  game_data = &tetris_model.getModelData();
  EXPECT_EQ(game_data->game_state, GameState::START);
  tetris_model.updateData(UserAction::SPACE_BTN);
  EXPECT_EQ(game_data->game_state, GameState::SPAWN);
  tetris_model.updateData(UserAction::NO_ACT);
  EXPECT_EQ(game_data->game_state, GameState::MOVING);

  tetris_model.updateData(UserAction::SPACE_BTN);

  tetris_model.updateData(UserAction::ESC_BTN);
  EXPECT_EQ(game_data->game_state, GameState::EXIT);
}

TEST_F(TetrisGameTest, RotateFigure) {
  tetris_model.setDefault();
  game_data = &tetris_model.getModelData();
  EXPECT_EQ(game_data->game_state, GameState::START);
  tetris_model.updateData(UserAction::SPACE_BTN);
  EXPECT_EQ(game_data->game_state, GameState::SPAWN);
  tetris_model.updateData(UserAction::NO_ACT);
  EXPECT_EQ(game_data->game_state, GameState::MOVING);

  tetris_model.updateData(UserAction::UP_BTN);
  tetris_model.updateData(UserAction::UP_BTN);
  tetris_model.updateData(UserAction::UP_BTN);

  tetris_model.updateData(UserAction::ESC_BTN);
  EXPECT_EQ(game_data->game_state, GameState::EXIT);
}

TEST_F(TetrisGameTest, PauseUnpause) {
  tetris_model.setDefault();
  game_data = &tetris_model.getModelData();
  EXPECT_EQ(game_data->game_state, GameState::START);
  tetris_model.updateData(UserAction::SPACE_BTN);
  EXPECT_EQ(game_data->game_state, GameState::SPAWN);
  tetris_model.updateData(UserAction::NO_ACT);
  EXPECT_EQ(game_data->game_state, GameState::MOVING);

  tetris_model.updateData(UserAction::TAB_BTN);
  EXPECT_EQ(game_data->game_state, GameState::PAUSE);

  tetris_model.updateData(UserAction::TAB_BTN);
  EXPECT_EQ(game_data->game_state, GameState::MOVING);

  tetris_model.updateData(UserAction::ESC_BTN);
  EXPECT_EQ(game_data->game_state, GameState::EXIT);
}

TEST_F(TetrisGameTest, GameOver) {
  tetris_model.setDefault();
  game_data = &tetris_model.getModelData();
  EXPECT_EQ(game_data->game_state, GameState::START);
  tetris_model.updateData(UserAction::SPACE_BTN);
  EXPECT_EQ(game_data->game_state, GameState::SPAWN);
  tetris_model.updateData(UserAction::NO_ACT);
  EXPECT_EQ(game_data->game_state, GameState::MOVING);

  for (int i = 0; i < 20; i++) {
    tetris_model.updateData(UserAction::DOWN_BTN);
  }

  while (game_data->game_state != GameState::GAMEOVER) {
    tetris_model.updateData(UserAction::SPACE_BTN);
  }

  tetris_model.updateData(UserAction::ESC_BTN);
  EXPECT_EQ(game_data->game_state, GameState::EXIT);
}

TEST_F(TetrisGameTest, ScoreTest) {
  tetris_model.setDefault();
  game_data = &tetris_model.getModelData();
  EXPECT_EQ(game_data->game_state, GameState::START);
  tetris_model.updateData(UserAction::SPACE_BTN);
  EXPECT_EQ(game_data->game_state, GameState::SPAWN);
  tetris_model.updateData(UserAction::NO_ACT);
  EXPECT_EQ(game_data->game_state, GameState::MOVING);

  /* 1 line */
  for (int i = 19; i < 20; ++i) {
    for (int j = 0; j < 10; ++j) {
      game_data->game_field[i][j].first = true;
    }
  }

  tetris_model.updateData(UserAction::SPACE_BTN);
  EXPECT_EQ(game_data->cur_score, 100);

  tetris_model.setDefault();
  tetris_model.updateData(UserAction::SPACE_BTN);
  tetris_model.updateData(UserAction::NO_ACT);

  /* 4 lines */
  for (int i = 16; i < 20; ++i) {
    for (int j = 0; j < 10; ++j) {
      game_data->game_field[i][j].first = true;
    }
  }

  tetris_model.updateData(UserAction::SPACE_BTN);
  EXPECT_EQ(game_data->cur_score, 1500);

  tetris_model.updateData(UserAction::ESC_BTN);
  EXPECT_EQ(game_data->game_state, GameState::EXIT);
}
