#ifndef BRICKGAME_SNAKE_CONSOLE_VIEW_H_
#define BRICKGAME_SNAKE_CONSOLE_VIEW_H_

#include "../base/BaseConsoleView.h"

namespace s21 {

/**
 * @brief Class for rendering the console view of the Snake game.
 */
class SnakeConsoleView : public BaseConsoleView {
 public:
  using SnakeController = Controller<SnakeModel, UserAction::UP_BTN>;

  /**
   * @brief Constructs a new SnakeConsoleView object.
   *
   * @param s_c Pointer to the SnakeController object that this view will use.
   */
  explicit SnakeConsoleView(SnakeController* s_c);

  /**
   * @brief Starts the Snake game console view.
   *
   * This method overrides the pure virtual function in BaseConsoleView.
   */
  void Start() override;

 private:
  /**
   * @brief Renders the Snake game on the console.
   */
  void renderGame();

  /**
   * @brief Main loop for running the Snake game.
   */
  void SnakeMainLoop();

  /**
   * @brief Updates the model based on the current action.
   */
  void updateModel();

  /**
   * @brief Checks the current state of the game.
   */
  void checkState();

  UserAction action_{};  ///< The current action performed by the user.
  SnakeModel::GameData*
      data_;  ///< Pointer to the game data of the Snake model.
  SnakeController* controller_;  ///< Pointer to the SnakeController object.
};

}  // namespace s21

#endif  // BRICKGAME_SNAKE_CONSOLE_VIEW_H_
