#ifndef BRICKGAME_CONTROLLER_H_
#define BRICKGAME_CONTROLLER_H_

#include "../brick_game/base/BaseConstants.h"

namespace s21 {

/**
 * @brief Controller class template for managing the interaction between the
 * user and the model.
 *
 * @tparam Model The type of the model being controlled.
 * @tparam defaultAction The default user action to be performed.
 */
template <class Model, UserAction defaultAction>
class Controller {
 public:
  /**
   * @brief Constructs a new Controller object.
   *
   * @param model Pointer to the model object that this controller will manage.
   */
  Controller(Model *model) : model_(model) {}

  /**
   * @brief Default destructor.
   */
  ~Controller() = default;

  /**
   * @brief Updates the model data based on the provided user action.
   *
   * @param action The user action to be performed. If no action is provided,
   * the default action is used.
   */
  void updateModelData(UserAction action = defaultAction) {
    model_->updateData(action);
  }

  /**
   * @brief Resets the model data to its default state.
   */
  void setModelToDefault() { model_->setDefault(); }

  /**
   * @brief Returns the current game data from the model.
   *
   * @return Reference to the current GameData object of the model.
   */
  typename Model::GameData &getModelData() { return model_->getModelData(); }

 private:
  Model *model_;  ///< Pointer to the model object being controlled.
};

}  // namespace s21

#endif  // BRICKGAME_CONTROLLER_H_
