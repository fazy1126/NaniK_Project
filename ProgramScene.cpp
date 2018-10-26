#include "ProgramScene.hpp"

ProgramScene::ProgramScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{}

void ProgramScene::update() {
  int mouse_x, mouse_y;
  std::tie(mouse_x, mouse_y) = Mouse::getIns()->getCursolePoint();

}