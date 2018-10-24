#ifndef DEF_MOUSE_HPP
#define DEF_MOUSE_HPP

#include "Singleton.hpp"
#include <utility>

class Mouse final : public Singleton<Mouse> {
  Mouse() = default;
  friend Singleton<Mouse>;

  public:
    bool update();
    int getPressingStatus(int buttonCode);
    int getWheelStatus();
    std::pair<int, int> getCursolePoint();
  private:
    bool isAvailableCode(int buttonCode);
    int _x, _y;
    int _mouse;
    int _wheel;
};

#endif