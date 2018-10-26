#ifndef DEF_TITLESCENE_HPP
#define DEF_TITLESCENE_HPP

#include "AbstractScene.hpp"
#include <vector>
#include <string> 

struct MenuElement {
  MenuElement(int nx, int ny, std::string str, int nhandle) {
    x = nx, y = ny, name = str, handle = nhandle;
  };
  int x, y;
  std::string name;
  int handle;
  float size;
};

class TitleScene : public AbstractScene{
  public:
    TitleScene(IOnSceneChangedListener *impl, const Parameter& parameter);
    virtual ~TitleScene() = default;
    void initialize() override;
    void update() override;
    void draw() const override;

  private: 
    std::vector<MenuElement> _menu;
    bool _oldmouse_input = false;
};

#endif