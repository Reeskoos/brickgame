#ifndef BRICKGAME_BASE_CONSTANTS_H_
#define BRICKGAME_BASE_CONSTANTS_H_

#include <random>

namespace s21 {

/**
 * \namespace ConstSizes
 * \brief Constants used in the application
 *
 * Contains all the constants used in the application, like field sizes, console
 * sizes and other.
 */
namespace ConstSizes {

/// The width of the game field
constexpr int field_width = 10;

/// The height of the game field
constexpr int field_height = 20;

/// The size of a pixel for view
constexpr int pixel_size = 25;

/// The height of the console window
constexpr int console_window_h = 21;

/// The width of the console window
constexpr int console_window_w = 22;

/// The height of the information panel
constexpr int console_info_panel_h = 7;

/// @brief The intervals in milliseconds between levels
static constexpr int levels_intervals_ms[] = {500, 420, 340, 260, 230,
                                              200, 180, 160, 140, 125};
}  // namespace ConstSizes

/**
 * \struct Cords
 * \brief A struct that represents coordinates in 2D space.
 */
struct Cords {
  Cords() : x_(0), y_(0) {}

  explicit Cords(int x, int y) : x_(x), y_(y) {}

  Cords(std::initializer_list<int> list) {
    auto it = list.begin();
    x_ = *it++;
    y_ = *it;
  }

  bool operator==(const Cords& other) const {
    return (x_ == other.x_ && y_ == other.y_);
  }

  bool operator!=(const Cords& other) const { return !(*this == other); }

  void randomCords() {
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_int_distribution<int> width(0, ConstSizes::field_width - 1);
    std::uniform_int_distribution<int> height(0, ConstSizes::field_height - 1);

    x_ = width(gen);
    y_ = height(gen);
  }

  int x_;
  int y_;
};

enum class GameState { START, SPAWN, MOVING, COLLIDE, PAUSE, EXIT, GAMEOVER };

enum class UserAction {
  NO_ACT = 0,
  UP_BTN,
  DOWN_BTN,
  LEFT_BTN,
  RIGHT_BTN,
  SPACE_BTN,
  ENTER_BTN,
  ESC_BTN,
  TAB_BTN
};

}  // namespace s21

#endif
