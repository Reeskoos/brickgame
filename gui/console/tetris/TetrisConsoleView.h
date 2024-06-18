#ifndef BRICKGAME_TETRIS_CONSOLE_VIEW_H_
#define BRICKGAME_TETRIS_CONSOLE_VIEW_H_

#include "../base/BaseConsoleView.h"

namespace s21 {

/**
 * @brief Class for rendering the console view of the Tetris game.
 */
class TetrisConsoleView : public BaseConsoleView {
 public:
  using TetrisController = Controller<TetrisModel, UserAction::NO_ACT>;

  /**
   * @brief Constructs a new TetrisConsoleView object.
   *
   * @param c Pointer to the TetrisController object that this view will use.
   * Defaults to nullptr.
   */
  explicit TetrisConsoleView(TetrisController *c = nullptr);

  /**
   * @brief Starts the Tetris game console view.
   *
   * This method overrides the pure virtual function in BaseConsoleView.
   */
  void Start() override;

 private:
  UserAction action_;  ///< The current action performed by the user.

  TetrisModel::GameData
      *data_;  ///< Pointer to the game data of the Tetris model.

  TetrisController *controller_;  ///< Pointer to the TetrisController object.

  /**
   * @brief Main loop for running the Tetris game.
   */
  void TetrisMainLoop();

  /**
   * @brief Updates the model based on the current action.
   */
  void updateModel();

  /**
   * @brief Renders the Tetris game on the console.
   */
  void renderGame();

  /**
   * @brief Checks the current state of the game.
   */
  void checkState();
};

}  // namespace s21

#endif  // BRICKGAME_TETRIS_CONSOLE_VIEW_H_
