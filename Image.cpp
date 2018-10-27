#include "Image.hpp"
#include "dxlib/DxLib.h"

Image::Image() {
  _gamestart = myLoadGraph("Image/Menu/gamestart.png");
}

void Image::release() {
  const int size = _images.size();
  for(int i=0; i<size; i++){
    DeleteGraph(_images[i]);
  }
  _images.clear();
}

std::pair<int, int> Image::getMenu() {
  return std::make_pair(_gamestart, _gameend);
}

int Image::myLoadGraph(const char* filename) {
  int ret = LoadGraph(filename);
  _images.emplace_back(ret);
  return ret;
}

int Image::myLoadDivGraph(const char* filename, int n, int xn, int yn, int w, int h, int* buf) {
  int ret = LoadDivGraph(filename, n, xn, yn, w, h, buf);
  for(int i=0; i<n; i++) {
    _images.emplace_back(buf[i]);
  }
  return ret;
}