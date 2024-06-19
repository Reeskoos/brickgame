#include "BaseConsoleView.h"

namespace s21 {

void BaseConsoleView::drawWindow(const Cords& topLeft,
                                 const Cords& bottomRight) {
  for (int i = topLeft.y_; i <= bottomRight.y_; ++i) {
    mvaddch(i, topLeft.x_, ACS_VLINE);
    mvaddch(i, bottomRight.x_, ACS_VLINE);
  }
  for (int i = topLeft.x_; i <= bottomRight.x_; ++i) {
    mvaddch(topLeft.y_, i, ACS_HLINE);
    mvaddch(bottomRight.y_, i, ACS_HLINE);
  }

  mvaddch(topLeft.y_, topLeft.x_, ACS_ULCORNER);
  mvaddch(topLeft.y_, bottomRight.x_, ACS_URCORNER);
  mvaddch(bottomRight.y_, topLeft.x_, ACS_LLCORNER);
  mvaddch(bottomRight.y_, bottomRight.x_, ACS_LRCORNER);
}

void BaseConsoleView::renderGameField(WidgetChoice widget, int lvl, int score,
                                      int best_score) {
  /* main border */
  drawWindow({0, 0},
             {ConstSizes::console_window_w, ConstSizes::console_window_h});
  /* field border */
  drawWindow({0, 0},
             {ConstSizes::field_width + 1, ConstSizes::field_height + 1});

  mvaddch(0, ConstSizes::field_width + 1, ACS_TTEE);
  mvaddch(ConstSizes::field_height + 1, ConstSizes::field_width + 1, ACS_BTEE);

  int side_panel_h = 1;
  if (widget == WidgetChoice::TETRIS) {
    side_panel_h = ConstSizes::console_info_panel_h;

    drawWindow({ConstSizes::field_width + 2, 1},
               {ConstSizes::console_window_w - 1, side_panel_h});
    mvprintw(1, ConstSizes::field_width + 3, "Next");
    side_panel_h += 1;
  }

  drawWindow({ConstSizes::field_width + 2, side_panel_h},
             {ConstSizes::console_window_w - 1, side_panel_h + 2});
  mvprintw(side_panel_h, ConstSizes::field_width + 3, "Level");
  mvprintw(side_panel_h + 1, ConstSizes::field_width + 8, "%d", lvl);
  side_panel_h += 3;

  drawWindow({ConstSizes::field_width + 2, side_panel_h},
             {ConstSizes::console_window_w - 1, side_panel_h + 2});
  mvprintw(side_panel_h, ConstSizes::field_width + 3, "Score");
  int score_len = static_cast<int>(std::to_string(score).length());
  mvprintw(side_panel_h + 1, ConstSizes::field_width + 9 - score_len, "%d",
           score);
  side_panel_h += 3;

  drawWindow({ConstSizes::field_width + 2, side_panel_h},
             {ConstSizes::console_window_w - 1, side_panel_h + 2});
  mvprintw(side_panel_h, ConstSizes::field_width + 3, "Best");
  int b_score_len = static_cast<int>(std::to_string(best_score).length());
  mvprintw(side_panel_h + 1, ConstSizes::field_width + 9 - b_score_len, "%d",
           best_score);
}

void BaseConsoleView::renderGameOverMenu(bool win, int lvl, int score,
                                         int best_score) {
  clear();
  drawWindow({0, 0},
             {ConstSizes::console_window_w, ConstSizes::console_window_h});

  if (win) {
    mvprintw(2, ConstSizes::console_window_w / 2 - 4, "VICTORY");
  } else {
    mvprintw(2, ConstSizes::console_window_w / 2 - 4, "GAMEOVER");
  }

  mvprintw(4, ConstSizes::console_window_w / 2 - 4, "Score: %d", score);
  mvprintw(6, ConstSizes::console_window_w / 2 - 4, "Level: %d", lvl);
  mvprintw(8, ConstSizes::console_window_w / 2 - 7, "Best score: %d",
           best_score);

  mvprintw(10, ConstSizes::console_window_w / 2 - 5, "Press any key");
  mvprintw(11, ConstSizes::console_window_w / 2 - 5, "to continue");
  nodelay(stdscr, FALSE);
  getch();
}

void BaseConsoleView::renderPauseInfo(int lvl, int score, int best_score) {
  clear();
  drawWindow({0, 0},
             {ConstSizes::console_window_w, ConstSizes::console_window_h});

  mvprintw(5, ConstSizes::console_window_w / 2 - 10, "The game is on pause");
  mvprintw(8, ConstSizes::console_window_w / 2 - 7, "Score:  %d", score);
  mvprintw(10, ConstSizes::console_window_w / 2 - 7, "Level:  %d", lvl);
  mvprintw(12, ConstSizes::console_window_w / 2 - 7, "Best score:  %d",
           best_score);

  mvprintw(14, ConstSizes::console_window_w / 2 - 10, "Press Tab to continue");
  mvprintw(15, ConstSizes::console_window_w / 2 - 5, "or Esc to exit");

  nodelay(stdscr, FALSE);
}

void BaseConsoleView::renderStartInfo() {
  clear();
  drawWindow({0, 0},
             {ConstSizes::console_window_w, ConstSizes::console_window_h});

  mvprintw(9, ConstSizes::console_window_w / 2 - 6, "Press space");
  mvprintw(10, ConstSizes::console_window_w / 2 - 5, "to start");
  mvprintw(11, ConstSizes::console_window_w / 2 - 7, "or Esc to exit");
  nodelay(stdscr, FALSE);
}

UserAction BaseConsoleView::getAction() {
  UserAction action = UserAction::NO_ACT;
  int key = getch();
  switch (key) {
    case KEY_LEFT:
      action = UserAction::LEFT_BTN;
      break;
    case KEY_RIGHT:
      action = UserAction::RIGHT_BTN;
      break;
    case KEY_UP:
      action = UserAction::UP_BTN;
      break;
    case KEY_DOWN:
      action = UserAction::DOWN_BTN;
      break;
    case 13:
      action = UserAction::ENTER_BTN;
      break;
    case 9:
      action = UserAction::TAB_BTN;
      break;
    case 32:
      action = UserAction::SPACE_BTN;
      break;
    case 27:
      action = UserAction::ESC_BTN;
      break;
    default:
      break;
  }
  nodelay(stdscr, TRUE);
  return action;
}

WidgetChoice BaseConsoleView::initMenu() {
  WidgetChoice ret_val = WidgetChoice::MENU;
  size_t selectedItem = 0;

  renderMenu(selectedItem);

  int ch;
  nodelay(stdscr, FALSE);
  do {
    ch = getch();
    if (ch == KEY_UP && selectedItem > 0) {
      --selectedItem;
    } else if (ch == KEY_DOWN && selectedItem < 2) {
      ++selectedItem;
    }
    renderMenu(selectedItem);
  } while (ch != '\n');

  switch (selectedItem) {
    case 0:
      ret_val = WidgetChoice::SNAKE;
      break;
    case 1:
      ret_val = WidgetChoice::TETRIS;
      break;
    case 2:
      ret_val = WidgetChoice::EXIT;
      break;
  }

  return ret_val;
}

void BaseConsoleView::renderMenu(size_t& selectedItem) {
  clear();

  int start_y = 10;
  int start_x = ConstSizes::console_window_w / 2;

  drawWindow({0, 0},
             {ConstSizes::console_window_w, ConstSizes::console_window_h});
  mvprintw(start_y - 4, start_x - 4, "BRICKGAME");

  std::vector<std::string> choices = {"Snake", "Tetris", "Exit"};

  for (std::size_t i = 0; i < choices.size(); ++i) {
    if (i == selectedItem) {
      attron(A_REVERSE);
      mvprintw(++start_y + i, start_x - (int)choices[i].length() / 2, "%s",
               choices[i].c_str());
      attroff(A_REVERSE);
    } else {
      mvprintw(++start_y + i, start_x - (int)choices[i].length() / 2, "%s",
               choices[i].c_str());
    }
  }

  refresh();
}

}  // namespace s21