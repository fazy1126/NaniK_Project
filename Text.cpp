#include "Text.hpp"
#include "dxlib/DxLib.h"
#include <fstream>

Text::Text() {
  _restmyhp = myLoadText("DescriptionText/RestMyHp.txt");
  _restenemyhp = myLoadText("DescriptionText/RestEnemyHp.txt");
  _mylastaction = myLoadText("DescriptionText/MyLastAction.txt");
  _enemylastaction = myLoadText("DescriptionText/EnemyLastAction.txt");
  _random = myLoadText("DescriptionText/Random.txt");
  _else = myLoadText("DescriptionText/Else.txt");
}

std::vector<std::string> Text::myLoadText(const char* filename) {
  std::ifstream ifs(filename);
  std::string str;
  std::vector<std::string> res;
  if(ifs.fail()) {
    MessageBox(NULL, "Failed to load file", "Error", MB_OK);
    return res;
  }
  while(getline(ifs, str)) {
    res.emplace_back(str);
  }
  return res;
}