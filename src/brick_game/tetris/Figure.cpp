#include "Figure.h"

namespace s21 {

Figure::Figure() {
  cords_.resize(4);
  setRandomShape();
}

bool Figure::operator==(const Figure &other) const {
  return shape_ == other.shape_ && cords_ == other.cords_;
}

bool Figure::operator!=(const Figure &other) const { return !(*this == other); }

void Figure::setShape(Shape s) {
  shape_ = s;
  setShapeCords();
  while (getMinY() < 1) moveDown();
}

void Figure::setShapeCords() {
  int cur_shape = static_cast<int>(shape_);
  for (int i = 0; i < 4; i++) {
    cords_[i].x_ = FigureCords::cordsMatrix[cur_shape][i].x_ +
                   (ConstSizes::field_width / 2);
    cords_[i].y_ = FigureCords::cordsMatrix[cur_shape][i].y_ - 1;
  }
}

void Figure::setRandomShape() {
  std::random_device rd;
  std::default_random_engine gen(rd());
  std::uniform_int_distribution<int> distribution(1, 7);
  auto rand_shape = static_cast<Shape>(distribution(gen));
  setShape(rand_shape);
}

std::vector<Cords> Figure::getCords() const { return cords_; };

Shape Figure::getShape() const { return shape_; }

void Figure::Rotate() {
  if (shape_ == Shape::O) return;
  int center_x = getMaxX() - 1;
  int center_y = getMaxY() - 1;

  int rotationMatrix[2][2] = {{0, -1}, {1, 0}};

  for (int i = 0; i < 4; i++) {
    int x = cords_[i].x_ - (center_x);
    int y = cords_[i].y_ - (center_y);
    cords_[i].x_ =
        (x * rotationMatrix[0][0] + y * rotationMatrix[0][1]) + (center_x);
    cords_[i].y_ =
        (x * rotationMatrix[1][0] + y * rotationMatrix[1][1]) + (center_y);
  }
  adjustPosition();
}

void Figure::adjustPosition() {
  while (getMinY() <= 0) moveDown();
  while (getMaxX() >= static_cast<int>(ConstSizes::field_width)) moveLeft();
  while (getMinX() < 0) moveRight();
}

int Figure::getMinX() {
  int res = cords_[0].x_;
  for (int i = 0; i < 4; i++) {
    res = std::min(res, cords_[i].x_);
  }
  return res;
}

int Figure::getMaxX() {
  int res = cords_[0].x_;
  for (int i = 0; i < 4; i++) {
    res = std::max(res, cords_[i].x_);
  }
  return res;
}

int Figure::getMinY() {
  int res = cords_[0].y_;
  for (int i = 0; i < 4; i++) {
    res = std::min(res, cords_[i].y_);
  }
  return res;
}

int Figure::getMaxY() {
  int res = cords_[0].y_;
  for (int i = 0; i < 4; i++) {
    res = std::max(res, cords_[i].y_);
  }
  return res;
}

void Figure::moveDown() {
  if (getMaxY() >= ConstSizes::field_height) return;
  for (auto &i : cords_) {
    i.y_ += 1;
  }
}

void Figure::moveLeft() {
  if (getMinX() < 1) return;
  for (auto &i : cords_) {
    i.x_ -= 1;
  }
}

void Figure::moveRight() {
  if (getMaxX() == ConstSizes::field_width - 1) return;
  for (auto &i : cords_) {
    i.x_ += 1;
  }
}
}  // namespace s21