#ifndef BRICKGAME_TETRIS_MODEL_H
#define BRICKGAME_TETRIS_MODEL_H

#include <algorithm>

#include "../base/BaseModel.h"
#include "Figure.h"


namespace s21 {

class TetrisModel : public BaseModel {
 public:
  /**
   * @brief Structure to hold the game data for Tetris including the current
   * score, level, game state, and figures.
   */
  struct GameData {
    size_t cur_score;      ///< Current score of the game
    size_t best_score;     ///< Best score achieved in the game
    size_t lvl;            ///< Current level of the game
    GameState game_state;  ///< Current state of the game

    Figure cur_figure;   ///< Current figure in play
    Figure next_figure;  ///< Next figure to be played
    Figure projection;   ///< Projection of the current figure
    std::vector<std::vector<std::pair<bool, int>>>
        game_field;  ///< The game field, with boolean for occupation and int
                     ///< for the type

    bool was_modified;  ///< Flag for smooth console view

    /**
     * @brief Default constructor initializing game data to default values.
     */
    GameData()
        : cur_score(0),
          best_score(0),
          lvl(1),
          game_state(GameState::START),
          cur_figure(),
          next_figure(),
          projection(),
          game_field(),
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
             cur_figure == other.cur_figure &&
             next_figure == other.next_figure &&
             projection == other.projection && game_field == other.game_field;
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

  /**
   * @brief Constructs a new TetrisModel object.
   */
  TetrisModel();

  /**
   * @brief Destructor for TetrisModel.
   */
  ~TetrisModel();

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
  GameData tetris_data_;        ///< The current game data
  long long last_move_time_{};  ///< Time of the last move
  long long cur_interval_{};    ///< Current time interval between moves
  StateActionMatrix<TetrisModel>
      EventsMatrix;  ///< Matrix to handle state actions

  /**
   * @brief Initializes the event handlers matrix.
   */
  void initEventHandlersMatrix();

  /**
   * @brief Initializes the game field.
   */
  void initField();

  /**
   * @brief Initializes the projection of the current figure.
   */
  void initProjection();

  /**
   * @brief Updates the game field based on the current state.
   */
  void updateField();

  /**
   * @brief Updates the level based on the current score.
   */
  void updateLvl();

  /**
   * @brief Updates the score based on the number of lines completed.
   *
   * @param lines The number of lines completed.
   */
  void updateScore(size_t lines);

  /**
   * @brief Checks for collisions in the game field.
   *
   * @return true if a collision is detected; false otherwise.
   */
  bool checkCollision();

  /**
   * @brief Checks for and returns the number of complete lines in the game
   * field.
   *
   * @return The number of complete lines.
   */
  size_t checkCompleteLines();

  /**
   * @brief Clears a line in the game field.
   *
   * @param i The index of the line to clear.
   */
  void clearLine(int i);

  /* FIGURES */

  /**
   * @brief Spawns a new figure.
   */
  void SpawnFigure();

  /**
   * @brief Places the figure at the top of the game field.
   */
  void placeFigureUp();

  /**
   * @brief Checks if the figure can move based on the user's action.
   *
   * @param piece The figure to check.
   * @param action The user's action.
   * @return true if the figure can move; false otherwise.
   */
  bool FigureCanMove(const Figure& piece, UserAction action);

  /**
   * @brief Moves the figure left by one unit.
   */
  void MoveFigureLeft();

  /**
   * @brief Moves the figure right by one unit.
   */
  void MoveFigureRight();

  /**
   * @brief Moves the figure down by one unit.
   */
  void MoveFigureDown();

  /**
   * @brief Rotates the figure.
   */
  void RotateFigure();

  /**
   * @brief Drops the figure to the bottom of the game field.
   */
  void DropFigure();

  /* STATES */

  /**
   * @brief Starts the game by initializing the game state.
   */
  void Start();

  /**
   * @brief Exits the game.
   */
  void ExitGame();

  /**
   * @brief Pauses the game.
   */
  void Pause();

  /**
   * @brief Unpauses the game.
   */
  void Unpause();

  /**
   * @brief Handles collision events and updates the game state accordingly.
   */
  void Collide();

  /**
   * @brief Ends the game and updates the game state to game over.
   */
  void GameOver();
};

}  // namespace s21

#endif  // BRICKGAME_TETRIS_MODEL_H
