#include "TitleScene.hpp"
#include "dxlib/DxLib.h"

TitleScene::TitleScene(IOnSceneChangedListener *impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{}

void TitleScene::update() {}

void TitleScene::draw() const {
  DrawString(100, 100, "Title Scene", GetColor(255, 255, 255));
}