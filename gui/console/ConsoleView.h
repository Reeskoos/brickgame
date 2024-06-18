#ifndef BRICK_GAME_CONSOLE_VIEW_H_
#define BRICK_GAME_CONSOLE_VIEW_H_

#include <ncurses.h>

#include "base/BaseConsoleView.h"
#include "snake/SnakeConsoleView.h"
#include "tetris/TetrisConsoleView.h"

namespace s21 {

/**
 * @brief Class for managing and rendering the console view for both Snake and
 * Tetris games.
 */
class ConsoleView : public BaseConsoleView {
 public:
  using SnakeController = Controller<SnakeModel, UserAction::UP_BTN>;
  using TetrisController = Controller<TetrisModel, UserAction::NO_ACT>;

  /**
   * @brief Constructs a new ConsoleView object.
   *
   * @param s_c Pointer to the SnakeController object. Defaults to nullptr.
   * @param t_c Pointer to the TetrisController object. Defaults to nullptr.
   */
  explicit ConsoleView(SnakeController *s_c = nullptr,
                       TetrisController *t_c = nullptr);

  /**
   * @brief Default destructor.
   */
  ~ConsoleView() = default;

  /**
   * @brief Starts the console view.
   *
   * This method overrides the pure virtual function in BaseConsoleView.
   */
  void Start() override;

 protected:
  /**
   * @brief Initializes the ncurses library for console rendering.
   */
  void initNcurses();

 private:
  /**
   * @brief Main loop for managing the console view and game interaction.
   */
  void mainLoop();

  SnakeConsoleView snake_view_;    ///< Console view for the Snake game.
  TetrisConsoleView tetris_view_;  ///< Console view for the Tetris game.

  /**
   * @brief Initializes color pairs for ncurses.
   */
  static void initColors();
};

}  // namespace s21

#endif  // BRICK_GAME_CONSOLE_VIEW_H_
