#ifndef DEF_IMAGE_HPP
#define DEF_IMAGE_HPP 

#include "Singleton.hpp"
#include <vector>

class Image final : public Singleton<Image> {
  public: 
    Image();
    ~Image() = default;
    void load() {};
    void release();

    std::pair<int, int> getMenu();

  private: 
    int myLoadGraph(const char*);
    int myLoadDivGraph(const char*, int, int, int, int, int, int*);

    std::vector<int> _images;
    int _gamestart;
    int _gameend;
};

#endif