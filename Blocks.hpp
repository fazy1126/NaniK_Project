#ifndef DEF_BLOCKS_HPP
#define DEF_BLOCKS_HPP

#include "eCommand.hpp"
#include "eAction.hpp"
#include "Keyboard.hpp"
#include "dxlib/DxLib.h"
#include <string>
#include <vector>

/*
x1/y1: 左端上の座標
x2/y2: 右端下の座標
*/
struct Button {
  int x1, y1, x2, y2;
  std::string name;
  bool pressing = false;
  bool delete_p = false;
};

struct commandBlock {
  int x1, y1, x2, y2;
  std::string d_name = ""; //画面に表示する名前
  eCommand type;  //コマンドタイプ
  int value = -1; //入力される値
                  /*対応する数字: Attack:0 Block:1 CounterAttack:2*/
  bool onbutton = false; //バツボタンの上にカーソルが乗っているか
  bool delete_p = false;
};

struct actionBlock {
  int x1, y1, x2, y2;
  std::string d_name = "";
  eAction type;
  bool onbutton = false;
  bool delete_p = false;
};

struct numInputBlock {
  int x1, y1, x2, y2;
  bool inputstate = false; //入力状態
  std::pair<int, int> mouse; //最後に押されたマウスの座標
  std::string value = "";
  void getInput() {
    std::vector<int> keyCount(10);
    keyCount[0] = Keyboard::getIns()->getPressingCount(KEY_INPUT_0);
    keyCount[1] = Keyboard::getIns()->getPressingCount(KEY_INPUT_1);
    keyCount[2] = Keyboard::getIns()->getPressingCount(KEY_INPUT_2);
    keyCount[3] = Keyboard::getIns()->getPressingCount(KEY_INPUT_3);
    keyCount[4] = Keyboard::getIns()->getPressingCount(KEY_INPUT_4);
    keyCount[5] = Keyboard::getIns()->getPressingCount(KEY_INPUT_5);
    keyCount[6] = Keyboard::getIns()->getPressingCount(KEY_INPUT_6);
    keyCount[7] = Keyboard::getIns()->getPressingCount(KEY_INPUT_7);
    keyCount[8] = Keyboard::getIns()->getPressingCount(KEY_INPUT_8);
    keyCount[9] = Keyboard::getIns()->getPressingCount(KEY_INPUT_9);
    if(value.length()<=1) {
      for(int i=0; i<10; i++) {
        if(keyCount[i] == 1) {
          value += ('0'+i);
        }
      }
    }
    if(Keyboard::getIns()->getPressingCount(KEY_INPUT_BACK) == 1 && value.length()>=1) 
      value.erase(value.end()-1);
  }
};

#endif