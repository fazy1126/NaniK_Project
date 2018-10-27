#include "Looper.hpp"
#include "TitleScene.hpp"
#include "GameScene.hpp"
#include "ProgramScene.hpp"
#include "Error.hpp"
#include "Macro.hpp"
#include "Keyboard.hpp"
#include "Image.hpp"

using namespace std;

Looper::Looper() {
  Image::getIns()->load();
  
  Parameter parameter;
  _sceneStack.push(make_shared<TitleScene>(this, parameter));
}

bool Looper::loop() {
  _sceneStack.top()->update();
  _sceneStack.top()->draw();
  _fps.wait();
  Keyboard::getIns()->update();
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
    case Program:
      _sceneStack.push(make_shared<ProgramScene>(this, parameter));
      break;
    case Game:
      _sceneStack.push(make_shared<GameScene>(this, parameter));
      break;
    default:
      ERR("あるはずのないシーンが呼ばれました");
      break;
  }
}