#include "TitleScene.hpp"
#include "GameScene.hpp"
#include "Define.hpp"
#include "Keyboard.hpp"
#include "Image.hpp"
#include "dxlib/DxLib.h"

static const int WHITE = GetColor(255, 255, 255);
static const int RANGEX = 200;
static const int RANGEY = 100;

TitleScene::TitleScene(IOnSceneChangedListener *impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{
}

void TitleScene::initialize() {
  std::string name = "ゲームスタート";
  int start, end;
  std::tie(start, end) = Image::getIns()->getMenu();
  _menu.emplace_back(MenuElement(640, 400, name, start));
  name = "ゲームシュウリョウ";
  _menu.emplace_back(MenuElement(640, 600, name, end));
}

void TitleScene::update() {
  int mouse_x, mouse_y;
  GetMousePoint(&mouse_x, &mouse_y);
  for(auto &e : _menu) {
    if(e.x-RANGEX <= mouse_x && mouse_x <= e.x+RANGEX && e.y-RANGEY <= mouse_y && mouse_y <= e.y+RANGEY){
      e.size = 1.3;
      int newmouse_input = (GetMouseInput() & MOUSE_INPUT_LEFT);
      if(_oldmouse_input == 0 && newmouse_input == 1){
        Parameter parameter;
        parameter.set(GameScene::ParameterTagLevel, Define::eLevel::Level1);
        const bool stackClear = false;
        _implSceneChanged->onSceneChanged(eScene::Game, parameter, stackClear);
        return;
      }
      _oldmouse_input = newmouse_input;
    }
    else {
      e.size = 1.0;
    }
  }
}

void TitleScene::draw() const {
  for(auto &e : _menu) {
    float rate = 1.0;
    DrawRotaGraph(e.x, e.y, rate, 0.0, e.handle, TRUE);
  }
}