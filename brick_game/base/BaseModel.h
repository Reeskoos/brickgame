#ifndef BRICKGAME_BASE_MODEL_H_
#define BRICKGAME_BASE_MODEL_H_

#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

#include "BaseConstants.h"

#define STATES_CNT 7
#define USER_ACTIONS_CNT 9

namespace s21 {
/**
 * @brief Base model class that provides some common functionality.
 */
class BaseModel {
 public:
  /**
   * @brief Get current time in milliseconds.
   *
   * @return long long The current time in milliseconds.
   */
  static long long getCurTime() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    return ms.count();
  }

  /**
   * @brief Read the current best score from a file.
   *
   * @param filename The name of the file to read.
   * @return int The best score read from the file.
   */
  static int readBestScore(const std::string& filename) {
    int val = 0;
    std::ifstream file(filename);
    if (file.is_open()) {
      file >> val;
      file.close();
    }
    return val;
  }

  /**
   * @brief Write the current best score to a file.
   *
   * @param val The best score to write.
   * @param filename The name of the file to write to.
   */
  static void writeBestScore(int val, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
      file << val;
      file.close();
    }
  }
};

/**
 * @brief A template class for representing a state-action matrix.
 *
 * A state-action matrix maps a state and an action to a function pointer.
 * This class is templated on a model, and stores the function pointers in a
 * 2D array, where the first dimension is the state, and the second dimension
 * is the action.
 */
template <typename Model>
class StateActionMatrix {
 private:
  /// @brief An action in the matrix.
  using Action = void (Model::*)();

  /// @brief The function pointer matrix.
  Action matrix[STATES_CNT][USER_ACTIONS_CNT] = {};

  /// @brief A proxy class for accessing the function pointer matrix.
  ///
  /// This class provides a more convenient syntax for accessing the matrix.
  class Proxy {
   private:
    Action* row;

   public:
    /// @brief Constructs a Proxy object.
    ///
    /// @param row The row of the matrix to proxy.
    explicit Proxy(Action* row) : row(row) {}

    /// @brief Returns the function pointer for a given action.
    ///
    /// @param action The action to access.
    /// @return The function pointer for the action.
    Action& operator[](UserAction action) {
      return row[static_cast<size_t>(action)];
    }

    /// @brief Returns the function pointer for a given index.
    ///
    /// @param action The index to access.
    /// @return The function pointer for the index.
    Action& operator[](int action) { return row[action]; }
  };

 public:
  StateActionMatrix() {
    for (int i = 0; i < STATES_CNT; i++) {
      for (int j = 0; j < USER_ACTIONS_CNT; j++) {
        matrix[i][j] = nullptr;
      }
    }
  }

  /// @brief Returns a Proxy object for a given state.
  ///
  /// @param state The state to access.
  /// @return The Proxy object for the state.
  Proxy operator[](GameState state) {
    return Proxy(matrix[static_cast<size_t>(state)]);
  }
};
}  // namespace s21

#endif