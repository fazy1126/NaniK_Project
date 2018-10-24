#include "dxlib/DxLib.h"
#include "Mouse.hpp"

bool Mouse::update() {
  _mouse = GetMouseInput();
  _wheel = GetMouseWheelRotVol();
  GetMousePoint(&_x, &_y);
  return true;
}

int Mouse::getPressingStatus(int buttonCode) {
  if(_mouse & buttonCode) return 1;
  else return 0;
}

int Mouse::getWheelStatus() {
  return _wheel;
}

std::pair<int, int> Mouse::getCursolePoint() {
  return std::make_pair(_x, _y);
}