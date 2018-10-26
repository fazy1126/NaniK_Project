#ifndef DEF_SYSTEMMAIN_HPP
#define DEF_SYSTEMMAIN_HPP

#include "dxlib/DxLib.h"

class SystemMain {
  public:
    SystemMain() = default;
    ~SystemMain() = default;
    bool initialize();
    void finalize() const;
    void main() const;
  private: 
    LPCTSTR _font_path;
};

#endif