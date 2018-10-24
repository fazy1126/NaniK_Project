#include "TitleScene.hpp"
#include "GameScene.hpp"
#include "Define.hpp"
#include "Keyboard.hpp"
#include "dxlib/DxLib.h"

TitleScene::TitleScene(IOnSceneChangedListener *impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{}

void TitleScene::update() {
  if(Keyboard::getIns()->getPressingCount(KEY_INPUT_E) == 1) {
    Parameter parameter;
    parameter.set(GameScene::ParameterTagLevel, Define::eLevel::Easy);
    const bool stackClear = false;
    _implSceneChanged->onSceneChanged(eScene::Game, parameter, stackClear);
    return;
  }
}

void TitleScene::draw() const {
  DrawString(100, 100, "Title Scene", GetColor(255, 255, 255));
}