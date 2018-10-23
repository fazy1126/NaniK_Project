#include "dxlib/DxLib.h"
#include "SystemMain.hpp"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
  SystemMain system;
  if(system.initialize()){
    system.main();
  }
  system.finalize();
}