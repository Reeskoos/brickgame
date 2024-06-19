#include "ConsoleView.h"
namespace s21 {

ConsoleView::ConsoleView(SnakeController *s_c, TetrisController *t_c)
    : snake_view_(s_c), tetris_view_(t_c) {}

void ConsoleView::initNcurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);

  start_color();
  initColors();
}

void ConsoleView::initColors() {
  init_color(COLOR_WHITE + 1, 500, 500, 500); /* Gray */
  init_color(COLOR_WHITE + 2, 1000, 500, 0);  /* Orange */

  init_pair(1, COLOR_RED, COLOR_RED);
  init_pair(2, COLOR_GREEN, COLOR_GREEN);
  init_pair(3, COLOR_CYAN, COLOR_CYAN);
  init_pair(4, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(5, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(6, COLOR_WHITE + 2, COLOR_WHITE + 2);
  init_pair(7, COLOR_BLUE, COLOR_BLUE);
  init_pair(8, COLOR_WHITE + 1, COLOR_WHITE + 1);
}

void ConsoleView::Start() {
  setlocale(LC_ALL, "");
  initNcurses();
  mainLoop();
}

void ConsoleView::mainLoop() {
  WidgetChoice c = initMenu();
  while (c != WidgetChoice::EXIT) {
    switch (c) {
      case WidgetChoice::SNAKE:
        snake_view_.Start();
        break;
      case WidgetChoice::TETRIS:
        tetris_view_.Start();
        break;
      default:
        break;
    }
    c = initMenu();
  }
  curs_set(1);
  endwin();
}

}  // namespace s21