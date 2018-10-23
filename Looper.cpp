#include "Looper.hpp"
#include "TitleScene.hpp"
//#include "Error.hpp"

using namespace std;

Looper::Looper() {
  Parameter parameter;
  _sceneStack.push(make_shared<TitleScene>(this, parameter));
}

bool Looper::loop() const {
  _sceneStack.top()->update();
  _sceneStack.top()->draw();
  return true;
}

void Looper::onSceneChanged(const eScene scene, const Parameter& parameter, const bool stackClear)
{}