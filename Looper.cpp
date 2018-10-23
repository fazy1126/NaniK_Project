#include "Looper.hpp"
#include "TitleScene.hpp"
#include "GameScene.hpp"
#include "Error.hpp"
#include "Macro.hpp"

using namespace std;

Looper::Looper() {
  Parameter parameter;
  _sceneStack.push(make_shared<TitleScene>(this, parameter));
}

bool Looper::loop() {
  _sceneStack.top()->update();
  _sceneStack.top()->draw();
  _fps.draw();
  _fps.wait();
  return true;
}

void Looper::onSceneChanged(const eScene scene, const Parameter& parameter, const bool stackClear)
{
  if(stackClear){
    while(!_sceneStack.empty()) _sceneStack.pop();
  }
  switch(scene){
    case Title:
      _sceneStack.push(make_shared<TitleScene>(this, parameter));
      break;
    case Game:
      _sceneStack.push(make_shared<GameScene>(this, parameter));
      break;
    default:
      ERR("あるはずのないシーンが呼ばれました");
      break;
  }
}