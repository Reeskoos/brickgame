#ifndef BRICK_GAME_SNAKE_MODEL_H_
#define BRICK_GAME_SNAKE_MODEL_H_

#include <cstddef>
#include <functional>
#include <vector>

#include "../base/BaseModel.h"

namespace s21 {

class SnakeModel;

enum class Direction { UP, DOWN, LEFT, RIGHT };

class SnakeModel : public BaseModel {
 public:
  /**
   * @brief Structure to hold the game data including the snake's position,
   * fruit position, current score, etc.
   */
  struct GameData {
    std::vector<Cords>
        snake_coord;       ///< Coordinates of the snake's body segments
    Cords fruit_coord;     ///< Coordinates of the fruit
    Direction direction;   ///< Current direction of the snake's movement
    GameState game_state;  ///< Current state of the game

    size_t cur_score;   ///< Current score of the game
    size_t best_score;  ///< Best score achieved in the game
    size_t lvl;         ///< Current level of the game

    bool win;           ///< Flag indicating if the game is won
    bool was_modified;  ///< Flag for smooth console view

    /**
     * @brief Default constructor initializing game data to default values.
     */
    GameData()
        : snake_coord(),
          fruit_coord(),
          direction(Direction::UP),
          game_state(GameState::START),
          cur_score(0),
          best_score(0),
          lvl(1),
          win(false),
          was_modified(true) {}

    /**
     * @brief Equality operator to compare two GameData objects.
     *
     * @param other The other GameData object to compare with.
     * @return true if the current object is equal to the other; false
     * otherwise.
     */
    bool operator==(const GameData& other) const {
      return cur_score == other.cur_score && game_state == other.game_state &&
             fruit_coord == other.fruit_coord &&
             snake_coord == other.snake_coord;
    }

    /**
     * @brief Inequality operator to compare two GameData objects.
     *
     * @param other The other GameData object to compare with.
     * @return true if the current object is not equal to the other; false
     * otherwise.
     */
    bool operator!=(const GameData& other) const { return !(*this == other); }
  };

  SnakeModel();
  ~SnakeModel();

  /**
   * @brief Updates the game data based on the user's action.
   *
   * @param action The action performed by the user.
   */
  void updateData(UserAction action);

  /**
   * @brief Resets the game data to its default values.
   */
  void setDefault();

  /**
   * @brief Returns the current game data.
   *
   * @return Reference to the current GameData object.
   */
  GameData& getModelData();

 private:
  GameData snake_data_;
  StateActionMatrix<SnakeModel> EventsMatrix;
  long long cur_time_{};
  long long last_move_time_{};
  long long cur_interval_{};

  /**
   * @brief Updates the position of the fruit.
   */
  void updateFruitPos();

  /**
   * @brief Updates the scores in the game data.
   */
  void updateScores();

  /**
   * @brief Checks if the snake has eaten the fruit and updates the game data
   * accordingly.
   */
  void checkEating();

  /**
   * @brief Checks for collisions and updates the game state if a collision is
   * detected.
   */
  void checkCollision();

  /* FSM */

  /**
   * @brief Initializes the event handlers matrix.
   */
  void initEventHandlersMatrix();

  /**
   * @brief Moves the snake in the current direction.
   */
  void moveSnake();

  /**
   * @brief Moves the snake's head in the specified direction.
   *
   * @param direction The direction in which to move the head.
   */
  void MoveHead(Direction direction);

  /**
   * @brief Updates the position of the snake's head based on the direction.
   *
   * @param direction The direction in which to update the head position.
   */
  void UpdateHeadPosition(Direction direction);

  /**
   * @brief Checks if the new direction is opposite to the current direction.
   *
   * @param new_direction The new direction to check.
   * @return true if the new direction is opposite; false otherwise.
   */
  bool IsOppositeDirection(Direction new_direction);

  /**
   * @brief Moves the body of the snake to follow the head.
   */
  void MoveBody();

  /**
   * @brief Moves the head of the snake up.
   */
  void MoveHeadUp();

  /**
   * @brief Moves the head of the snake down.
   */
  void MoveHeadDown();

  /**
   * @brief Moves the head of the snake left.
   */
  void MoveHeadLeft();

  /**
   * @brief Moves the head of the snake right.
   */
  void MoveHeadRight();

  /* STATES UPDATE */

  /**
   * @brief Starts the game by initializing the game state.
   */
  void Start();

  /**
   * @brief Spawns the snake and the fruit at the beginning of the game.
   */
  void Spawn();

  /**
   * @brief Pauses the game.
   */
  void Pause();

  /**
   * @brief Unpauses the game.
   */
  void Unpause();

  /**
   * @brief Ends the game and updates the game state to game over.
   */
  void GameOver();

  /**
   * @brief Exits the game.
   */
  void ExitGame();

  /**
   * @brief Handles collision events and updates the game state accordingly.
   */
  void Collide();
};

};  // namespace s21

#endif  // BRICK_GAME_SNAKE_MODEL_H_
