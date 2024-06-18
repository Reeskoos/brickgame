#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QString>
#include <QTimer>
#include <QWidget>

#include "../../brick_game/snake/SnakeModel.h"
#include "../../brick_game/tetris/TetrisModel.h"
#include "../../controller/Controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

/**
 * @brief Enum representing the current widget being displayed.
 */
enum CurWidget { MENU, GAMEOVER_MENU, TETRIS, SNAKE };

/**
 * @brief The MainWindow class manages the main window of the game application.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Colors array used for rendering the games.
   */
  const QColor Colors[9] = {
      QColor(0, 0, 0),       QColor(204, 20, 20),  // red
      QColor(46, 199, 38),                         // green
      QColor(71, 248, 255),                        // light blue
      QColor(183, 9, 186),                         // purple
      QColor(255, 255, 10),                        // yellow
      QColor(255, 136, 0),                         // orange
      QColor(27, 98, 250),                         // blue
      QColor(114, 114, 120),                       // grey
  };

  using SnakeController = Controller<SnakeModel, UserAction::UP_BTN>;
  using TetrisController = Controller<TetrisModel, UserAction::NO_ACT>;

  /**
   * @brief Constructs a new MainWindow object.
   *
   * @param s_c Pointer to the Snake controller.
   * @param t_c Pointer to the Tetris controller.
   * @param parent Pointer to the parent widget.
   */
  MainWindow(SnakeController *s_c, TetrisController *t_c,
             QWidget *parent = nullptr);

  /**
   * @brief Destroys the MainWindow object.
   */
  ~MainWindow();

  /**
   * @brief Handles key press events.
   *
   * @param event The key event.
   */
  void keyPressEvent(QKeyEvent *event) override;

  /**
   * @brief Handles paint events.
   *
   * @param event The paint event.
   */
  void paintEvent(QPaintEvent *event) override;

 private slots:
  /**
   * @brief Slot for handling Snake button click event.
   */
  void SnakeButtonClicked();

  /**
   * @brief Slot for handling Tetris button click event.
   */
  void TetrisButtonClicked();

  /**
   * @brief Slot for handling Game Over to Menu button click event.
   */
  void GameOverToMenuClicked();

  /**
   * @brief Slot for handling Game Over Play Again button click event.
   */
  void GameOverPlayAgainClicked();

  /**
   * @brief Slot for handling Main Menu Exit button click event.
   */
  void MainMenuExitClicked();

 private:
  Ui::MainWindow *ui;
  QTimer *m_timer_;
  CurWidget cur_widget_;
  UserAction action_{};

  SnakeModel::GameData *snake_data_;
  TetrisModel::GameData *tetris_data_;

  TetrisController *tetris_controller_;
  SnakeController *snake_controller_;

  /**
   * @brief Clears the screen.
   */
  void ClearScreen();

  /**
   * @brief Updates the window.
   */
  void UpdateWindow();

  /**
   * @brief Renders the Snake game.
   */
  void RenderSnakeGame();

  /**
   * @brief Updates the Snake model.
   */
  void UpdateSnakeModel();

  /**
   * @brief Renders the Tetris game.
   */
  void RenderTetrisGame();

  /**
   * @brief Updates the Tetris model.
   */
  void UpdateTetrisModel();

  /**
   * @brief Initializes the game.
   */
  void initGame();

  /**
   * @brief Renders the Game Over menu.
   *
   * @param win Boolean indicating if the player won.
   * @param score The player's score.
   * @param best_score The best score.
   */
  void RenderGameOverMenu(bool win, int score, int best_score);

  /**
   * @brief Renders the start screen.
   *
   * @param p_label Pointer to the label widget.
   */
  static void RenderStartScreen(QLabel *p_label);

  /**
   * @brief Renders the pause screen.
   *
   * @param p_label Pointer to the label widget.
   */
  static void RenderPauseScreen(QLabel *p_label);
};

}  // namespace s21
#endif  // MAINWINDOW_H
