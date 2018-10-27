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
  if(_oldmouse_input == false && GetMouseInput() & MOUSE_INPUT_LEFT) {
    _start_push = std::make_pair(mouse_x, mouse_y);
    _end_push = std::make_pair(5000, 5000);
    _oldmouse_input = true;
  }
  else if(_oldmouse_input == true && !(GetMouseInput() & MOUSE_INPUT_LEFT)) {
    _end_push = std::make_pair(mouse_x, mouse_y);
    _oldmouse_input = false;
  }

  for(auto &e : _menu) {
    //押し始めた位置と離した位置が範囲内にあればページ遷移(TODO programSceneの実装)
    if(
      inRange(e.x, e.y, _start_push.first, _start_push.second, RANGEX, RANGEY)
      && inRange(e.x, e.y, _end_push.first, _end_push.second, RANGEX, RANGEY)
    ) {
      Parameter parameter;
      const bool stackClear = false;
      _implSceneChanged->onSceneChanged(eScene::Game, parameter, stackClear);
      return;
    }

    //押されていない状態で範囲内にカーソルがあればサイズ拡大
    if(_oldmouse_input == false && inRange(e.x, e.y, mouse_x, mouse_y, RANGEX, RANGEY)) e.size = 1.3;
    //押されている状態で押し始めた位置が範囲内ならば拡大
    else if(_oldmouse_input == true && inRange(e.x, e.y, _start_push.first, _start_push.second, RANGEX, RANGEY)) e.size = 1.3;
    //それ以外ならば元のサイズに戻す
    else e.size = 1.0;
  }
}

void TitleScene::draw() const {
  for(auto &e : _menu) {
    DrawRotaGraph(e.x, e.y, e.size, 0.0, e.handle, TRUE);
  }
}

bool TitleScene::inRange(int nx, int ny, int mx, int my, int rangex, int rangey) {
  if(nx-rangex <= mx && mx <= nx+rangex && ny-rangey <= my && my <= ny+rangey) return true;
  else return false;
}