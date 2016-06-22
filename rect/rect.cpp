#include "rect.h"

void Rect::calcValues(void) {
  top = _top;
  left = _left;
  bottom = _top + _height;
  right = _left + _width;
  centerx = (left + right) / 2;
  centery = (top + bottom) / 2;
}

void Rect::move(double vel[2]) {
  _top += vel[0];
  _left += vel[1];
  calcValues();
}

void Rect::setTop(double t) {
  _top = t;
  calcValues();
}

void Rect::setBottom(double b) {
  _top = b - _height;
  calcValues();
}

void Rect::setLeft(double l) {
  _left = l;
  calcValues();
}

void Rect::setRight(double r) {
  _left = r - _width;
  calcValues();
}

bool Rect::contains(Rect r) {
  if (r.left < this->left)
    return false;
  if (r.top < this->top)
    return false;
  if (r.right > this->right)
    return false;
  if (r.bottom > this->bottom)
    return false;
  return true;
}

Rect::Rect() {
  _top = -1;
  _left = -1;
  _width = 0;
  _height = 0;
  calcValues();
}
Rect::Rect(double _t, double _l, double w, double h) {
  _top = _t;
  _left = _l;
  _width = w;
  _height = h;
  calcValues();
}

Rect Rect::copy(void) {
  return Rect(_top, _left, _width, _height);
}
