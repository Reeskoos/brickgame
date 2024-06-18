#ifndef BRICKGAME_TETRIS_FIGURE_H_
#define BRICKGAME_TETRIS_FIGURE_H_

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>

#include "../base/BaseConstants.h"

namespace s21 {
enum class Shape { Z = 1, S, L, J, T, O, I, EMPTY };

namespace FigureCords {
static const Cords cordsMatrix[8][4] = {
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},     // empty
    {{0, -1}, {0, 0}, {-1, 0}, {-1, 1}},  // Z
    {{0, -1}, {0, 0}, {1, 0}, {1, 1}},    // S
    {{-1, -1}, {0, -1}, {0, 0}, {0, 1}},  // L
    {{1, -1}, {0, -1}, {0, 0}, {0, 1}},   // J
    {{-1, 0}, {0, 0}, {1, 0}, {0, 1}},    // T
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}},     // O
    {{0, -1}, {0, 0}, {0, 1}, {0, 2}},    // I
};
};

class Figure {
 public:
  /**
   * @brief Constructs a new Figure object with a default shape.
   */
  Figure();

  /**
   * @brief Default destructor.
   */
  ~Figure() = default;

  /**
   * @brief Equality operator to compare two Figure objects.
   *
   * @param other The other Figure object to compare with.
   * @return true if the current object is equal to the other; false otherwise.
   */
  bool operator==(const Figure& other) const;

  /**
   * @brief Inequality operator to compare two Figure objects.
   *
   * @param other The other Figure object to compare with.
   * @return true if the current object is not equal to the other; false
   * otherwise.
   */
  bool operator!=(const Figure& other) const;

  /**
   * @brief Sets the shape of the figure.
   *
   * @param s The shape to set.
   */
  void setShape(Shape s);

  /**
   * @brief Sets a random shape for the figure.
   */
  void setRandomShape();

  /**
   * @brief Gets the current shape of the figure.
   *
   * @return The current shape.
   */
  Shape getShape() const;

  /**
   * @brief Gets the coordinates of the figure.
   *
   * @return A vector of coordinates representing the figure.
   */
  std::vector<Cords> getCords() const;

  /**
   * @brief Rotates the figure.
   */
  void Rotate();

  /**
   * @brief Adjusts the position of the figure to ensure it is within bounds.
   */
  void adjustPosition();

  /**
   * @brief Moves the figure down by one unit.
   */
  void moveDown();

  /**
   * @brief Moves the figure left by one unit.
   */
  void moveLeft();

  /**
   * @brief Moves the figure right by one unit.
   */
  void moveRight();

  /**
   * @brief Gets the minimum x-coordinate of the figure.
   *
   * @return The minimum x-coordinate.
   */
  int getMinX();

  /**
   * @brief Gets the minimum y-coordinate of the figure.
   *
   * @return The minimum y-coordinate.
   */
  int getMinY();

  /**
   * @brief Gets the maximum x-coordinate of the figure.
   *
   * @return The maximum x-coordinate.
   */
  int getMaxX();

  /**
   * @brief Gets the maximum y-coordinate of the figure.
   *
   * @return The maximum y-coordinate.
   */
  int getMaxY();

 private:
  Shape shape_;               ///< Current shape of the figure
  std::vector<Cords> cords_;  ///< Coordinates of the figure

  /**
   * @brief Sets the coordinates of the figure based on its shape.
   */
  void setShapeCords();
};

}  // namespace s21

#endif  // BRICKGAME_TETRIS_FIGURE_H_
