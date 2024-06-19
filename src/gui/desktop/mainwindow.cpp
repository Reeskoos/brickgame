#include <QDir>

#include "./ui_mainwindow.h"
#include "mainwindow.h"

namespace s21 {

MainWindow::MainWindow(SnakeController *s_c, TetrisController *t_c,
                       QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      cur_widget_(CurWidget::MENU),
      action_(UserAction::NO_ACT),
      snake_data_(nullptr),
      tetris_data_(nullptr),
      snake_controller_(s_c),
      tetris_controller_(t_c) {
  ui->setupUi(this);
  ui->stackedWidget->setCurrentIndex(0);
  setWindowTitle("Brick Game");
  m_timer_ = new QTimer(this);

  connect(m_timer_, &QTimer::timeout, this, &MainWindow::UpdateWindow);
  connect(ui->initSnakeButton, &QPushButton::released, this,
          &MainWindow::SnakeButtonClicked);
  connect(ui->initTetrisButton, &QPushButton::released, this,
          &MainWindow::TetrisButtonClicked);
  connect(ui->gm_to_menu, &QPushButton::released, this,
          &MainWindow::GameOverToMenuClicked);
  connect(ui->gm_play_again, &QPushButton::released, this,
          &MainWindow::GameOverPlayAgainClicked);
  connect(ui->exitButton, &QPushButton::released, this,
          &MainWindow::MainMenuExitClicked);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::SnakeButtonClicked() {
  cur_widget_ = CurWidget::SNAKE;
  setWindowTitle("Snake Game");
  initGame();
}

void MainWindow::TetrisButtonClicked() {
  cur_widget_ = CurWidget::TETRIS;
  setWindowTitle("Tetris Game");
  initGame();
}

void MainWindow::initGame() {
  if (cur_widget_ == CurWidget::SNAKE) {
    snake_controller_->setModelToDefault();
    snake_data_ = &snake_controller_->getModelData();
  } else if (cur_widget_ == CurWidget::TETRIS) {
    tetris_controller_->setModelToDefault();
    tetris_data_ = &tetris_controller_->getModelData();
  }
  ui->stackedWidget->setCurrentIndex((int)cur_widget_);
  m_timer_->start(10);
}

void MainWindow::paintEvent(QPaintEvent *event) {
  QMainWindow::paintEvent(event);

  if (cur_widget_ == CurWidget::SNAKE) {
    bool gameover_or_exit = snake_data_->game_state == GameState::GAMEOVER ||
                            snake_data_->game_state == GameState::EXIT;
    if (gameover_or_exit) {
      ClearScreen();
      RenderGameOverMenu(snake_data_->win, snake_data_->cur_score,
                         snake_data_->best_score);
    } else {
      switch (snake_data_->game_state) {
        case GameState::START: {
          RenderStartScreen(ui->snake_start_info);
          break;
        }
        case GameState::PAUSE: {
          RenderPauseScreen(ui->snake_start_info);
          break;
        }
        default:
          ui->snake_start_info->setText("");
          RenderSnakeGame();
          break;
      }
    }
  } else if (cur_widget_ == CurWidget::TETRIS) {
    bool gameover_or_exit = tetris_data_->game_state == GameState::GAMEOVER ||
                            tetris_data_->game_state == GameState::EXIT;
    if (gameover_or_exit) {
      ClearScreen();
      RenderGameOverMenu(false, tetris_data_->cur_score,
                         tetris_data_->best_score);
    } else {
      switch (tetris_data_->game_state) {
        case GameState::START: {
          RenderStartScreen(ui->tetris_start_info);
          break;
        }
        case GameState::PAUSE: {
          RenderPauseScreen(ui->tetris_start_info);
          break;
        }
        default:
          ui->tetris_start_info->setText("");
          RenderTetrisGame();
          break;
      }
    }
  }
}

void MainWindow::ClearScreen() {
  QPainter painter(this);
  painter.eraseRect(rect());
  painter.end();
}

void MainWindow::RenderGameOverMenu(bool win, int score, int best_score) {
  m_timer_->stop();
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(CurWidget::GAMEOVER_MENU));

  ui->InfoLabel->setText(win ? "VICTORY" : "GAME OVER");
  ui->gm_score->setText(QString::number(score));
  ui->gm_best_score->setText(QString::number(best_score));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  int key = event->key();
  switch (key) {
    case Qt::Key_Left:
      action_ = UserAction::LEFT_BTN;
      break;
    case Qt::Key_Right:
      action_ = UserAction::RIGHT_BTN;
      break;
    case Qt::Key_Up:
      action_ = UserAction::UP_BTN;
      break;
    case Qt::Key_Down:
      action_ = UserAction::DOWN_BTN;
      break;
    case Qt::Key_Enter:
      action_ = UserAction::ENTER_BTN;
      break;
    case Qt::Key_Tab:
      action_ = UserAction::TAB_BTN;
      break;
    case Qt::Key_Space:
      action_ = UserAction::SPACE_BTN;
      break;
    case Qt::Key_Escape:
      action_ = UserAction::ESC_BTN;
      break;
    default:
      break;
  }
}

void MainWindow::UpdateWindow() {
  if (cur_widget_ == CurWidget::SNAKE) {
    UpdateSnakeModel();
  } else if (cur_widget_ == CurWidget::TETRIS) {
    UpdateTetrisModel();
  }
  repaint();
}

void MainWindow::UpdateSnakeModel() {
  snake_controller_->updateModelData(action_);
  snake_data_ = &snake_controller_->getModelData();
  action_ = UserAction::NO_ACT;
  ui->level->setText(QString::number(snake_data_->lvl));
  ui->score->setText(QString::number(snake_data_->cur_score));
  ui->bestScore->setText(QString::number(snake_data_->best_score));
}

void MainWindow::UpdateTetrisModel() {
  tetris_controller_->updateModelData(action_);
  tetris_data_ = &tetris_controller_->getModelData();
  action_ = UserAction::NO_ACT;
  ui->curlvl->setText(QString::number(tetris_data_->lvl));
  ui->curScore->setText(QString::number(tetris_data_->cur_score));
  ui->topScore->setText(QString::number(tetris_data_->best_score));
}

void MainWindow::RenderSnakeGame() {
  QRect snakeFieldRect = ui->SnakeField->geometry();
  QPainter qp(this);

  QImage food("images/food.png");
  QImage head("images/head.png");

  int pixel_size = ConstSizes::pixel_size;

  auto drawFood = [&]() {
    QRectF appleRect(
        snakeFieldRect.x() + snake_data_->fruit_coord.x_ * pixel_size,
        snakeFieldRect.y() + snake_data_->fruit_coord.y_ * pixel_size,
        pixel_size, pixel_size);
    qp.drawImage(appleRect, food);
  };

  auto drawSnakeHead = [&]() {
    QRectF headRect(
        snakeFieldRect.x() + snake_data_->snake_coord[0].x_ * pixel_size,
        snakeFieldRect.y() + snake_data_->snake_coord[0].y_ * pixel_size,
        pixel_size, pixel_size);

    QTransform transform;
    int rotation = 0;
    switch (snake_data_->direction) {
      case Direction::DOWN:
        rotation = 180;
        break;
      case Direction::LEFT:
        rotation = -90;
        break;
      case Direction::RIGHT:
        rotation = 90;
        break;
      default:
        break;
    }
    head = head.transformed(transform.rotate(rotation));
    qp.drawImage(headRect, head);
  };

  const QColor body_color(223, 187, 137);
  auto drawSnakeBody = [&]() {
    for (std::size_t i = 0; i < snake_data_->snake_coord.size(); ++i) {
      if (i == 0) {
        drawSnakeHead();
      } else {
        qp.setBrush(body_color);
        qp.setPen(body_color);
        qp.drawRect(
            snakeFieldRect.x() + snake_data_->snake_coord[i].x_ * pixel_size,
            snakeFieldRect.y() + snake_data_->snake_coord[i].y_ * pixel_size,
            pixel_size - 1, pixel_size - 1);
      }
    }
  };

  drawFood();
  drawSnakeBody();

  qp.end();
}

void MainWindow::RenderTetrisGame() {
  QPainter qp(this);
  const QRect field_rect = ui->TetrisField->geometry();
  const QRect next_rect = ui->NextFigure->geometry();

  const auto t_data = tetris_data_;
  const auto &colors = Colors;
  const int pixel_size = ConstSizes::pixel_size;
  const int field_height = ConstSizes::field_height;
  const int field_width = ConstSizes::field_width;

  const QBrush dark_gray(QColor(90, 90, 90));
  const QPen black(Qt::black);

  qp.setBrush(dark_gray);
  qp.setPen(black);

  auto drawFigure = [&](const auto &cords, const QRect &rect, int color_index,
                        bool isNext = false) {
    for (const auto &item : cords) {
      int x_offset = isNext ? item.x_ - 2.65 : item.x_;
      int y_offset = isNext ? item.y_ + 1 : item.y_ - 1;
      qp.setBrush(colors[color_index]);
      qp.drawRect(rect.x() + x_offset * pixel_size,
                  rect.y() + y_offset * pixel_size, pixel_size - 1,
                  pixel_size - 1);
    }
  };

  auto drawGameField = [&](const auto &game_field, const QRect &rect) {
    for (int i = 0; i < field_height; ++i) {
      for (int j = 0; j < field_width; ++j) {
        if (game_field[i][j].first) {
          qp.setBrush(colors[game_field[i][j].second]);
          qp.drawRect(rect.x() + j * pixel_size, rect.y() + i * pixel_size,
                      pixel_size - 1, pixel_size - 1);
        }
      }
    }
  };

  drawFigure(t_data->projection.getCords(), field_rect, 8);
  drawFigure(t_data->cur_figure.getCords(), field_rect,
             static_cast<int>(t_data->cur_figure.getShape()));
  drawFigure(t_data->next_figure.getCords(), next_rect,
             static_cast<int>(t_data->next_figure.getShape()), true);
  drawGameField(t_data->game_field, field_rect);
}

void MainWindow::RenderStartScreen(QLabel *p_label) {
  p_label->setText("Press SPACE to start");
  p_label->setStyleSheet("border: none;");
}

void MainWindow::RenderPauseScreen(QLabel *p_label) {
  p_label->setText(
      "Game is set on pause. <br> Press TAB to continue<br> ESC to exit.");
}

void MainWindow::GameOverToMenuClicked() {
  cur_widget_ = CurWidget::MENU;
  ui->stackedWidget->setCurrentIndex((int)cur_widget_);
}

void MainWindow::GameOverPlayAgainClicked() {
  if (cur_widget_ == CurWidget::SNAKE) {
    SnakeButtonClicked();
  } else if (cur_widget_ == CurWidget::TETRIS) {
    TetrisButtonClicked();
  } else {
    GameOverToMenuClicked();
  }
}

void MainWindow::MainMenuExitClicked() { close(); }

}  // namespace s21
