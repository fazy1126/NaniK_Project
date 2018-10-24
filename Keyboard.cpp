#include "dxlib/DxLib.h"
#include "Keyboard.hpp"

bool Keyboard::update() {
  char nowKeyStatus[KEY_NUM];
  GetHitKeyStateAll(nowKeyStatus);
  for(int i=0; i<KEY_NUM; i++){
    if(nowKeyStatus[i] != 0){
      if(_releasingCount[i] > 0){
        _releasingCount[i] = 0;
      }
      _pressingCount[i]++;
    }
    else{
      if(_pressingCount[i] > 0){
        _pressingCount[i] = 0;
      }
      _releasingCount[i]++;
    }
  }
  return true;
}

int Keyboard::getPressingCount(int keyCode) {
  if(!isAvailableCode(keyCode)) return -1;
  return _pressingCount[keyCode];
}

int Keyboard::getReleaseingCount(int keyCode) {
  if(!isAvailableCode(keyCode)) return -1;
  return _pressingCount[keyCode];
}

bool Keyboard::isAvailableCode(int keyCode) {
  if(0 <= keyCode && keyCode < KEY_NUM) return true;
  else return false;
}