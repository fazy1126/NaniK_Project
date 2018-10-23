#include "GameScene.hpp"
#include "dxlib/DxLib.h"

const std::string GameScene::ParameterTagStage = "ParameterTagStage";
const std::string GameScene::ParameterTagLevel = "ParameterTagLevel";

GameScene::GameScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter) {
  _level = parameter.get(ParameterTagLevel);
}

void GameScene::update()
{}

void GameScene::draw() const {
  DrawFormatString(100, 100, GetColor(255, 255, 255), "GAME LEVEL IS %d.", _level);
}