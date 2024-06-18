#ifndef BRICKGAME_BASE_CONSOLE_VIEW_H_
#define BRICKGAME_BASE_CONSOLE_VIEW_H_

#include <ncurses.h>

#include <chrono>
#include <cmath>
#include <string>
#include <thread>

#include "../../../brick_game/snake/SnakeModel.h"
#include "../../../brick_game/tetris/TetrisModel.h"
#include "../../../controller/Controller.h"

namespace s21 {

enum class WidgetChoice { MENU, EXIT, TETRIS, SNAKE };

/**
 * @brief Abstract base class for rendering the console view of the game.
 */
class BaseConsoleView {
 public:
  /**
   * @brief Pure virtual function to start the console view.
   *
   * This function must be implemented by derived classes.
   */
  virtual void Start() = 0;

  /**
   * @brief Draws a window on the console.
   *
   * @param topLeft Coordinates of the top-left corner of the window.
   * @param bottomRight Coordinates of the bottom-right corner of the window.
   */
  void drawWindow(const Cords& topLeft, const Cords& bottomRight);

  /**
   * @brief Renders the game field for the specified widget with the given level
   * and scores.
   *
   * @param widget The widget choice representing the current game (e.g., Tetris
   * or Snake).
   * @param lvl The current level of the game.
   * @param score The current score of the game.
   * @param best_score The best score achieved in the game.
   */
  void renderGameField(WidgetChoice widget, int lvl, int score, int best_score);

  /**
   * @brief Renders the game over menu with the given level and scores.
   *
   * @param win Indicates if the player has won.
   * @param lvl The level reached by the player.
   * @param score The final score of the player.
   * @param best_score The best score achieved in the game.
   */
  void renderGameOverMenu(bool win, int lvl, int score, int best_score);

  /**
   * @brief Renders the pause information with the given level and scores.
   *
   * @param lvl The current level of the game.
   * @param score The current score of the game.
   * @param best_score The best score achieved in the game.
   */
  void renderPauseInfo(int lvl, int score, int best_score);

  /**
   * @brief Renders the start information screen.
   */
  void renderStartInfo();

  /**
   * @brief Gets the user action from the console input.
   *
   * @return The user action.
   */
  UserAction getAction();

  /**
   * @brief Initializes and displays the main menu.
   *
   * @return The widget choice selected by the user.
   */
  WidgetChoice initMenu();

  /**
   * @brief Renders the main menu with the selected item highlighted.
   *
   * @param selectedItem The index of the currently selected menu item.
   */
  void renderMenu(size_t& selectedItem);
};

}  // namespace s21

#endif  // BRICKGAME_BASE_CONSOLE_VIEW_H_
