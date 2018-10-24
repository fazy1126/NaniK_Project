#ifndef DEF_KEYBOARD_HPP
#define DEF_KEYBOARD_HPP

#include "Singleton.hpp"
#include <array>

class Keyboard final : public Singleton<Keyboard> {
  Keyboard() = default;
  friend Singleton<Keyboard>;

  public:
    bool update();
    int getPressingCount(int keyCode); //押されているフレーム数を取得
    int getReleaseingCount(int keyCode); //話されているフレーム数を取得
  private:
    static const int KEY_NUM = 256;
    std::array<int, KEY_NUM> _pressingCount;
    std::array<int, KEY_NUM> _releasingCount;

    bool isAvailableCode(int keyCode);
};

#endif