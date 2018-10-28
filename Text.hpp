#ifndef DEF_TEXT_HPP
#define DEF_TEXT_HPP

#include "Singleton.hpp"
#include <vector>
#include <string>

using vecs = std::vector<std::string>;

class Text final : public Singleton<Text> {
  public:
    Text();
    ~Text() = default;
    void load() {};
    void release();

    vecs getRestmyhp() { return _restmyhp; }
    vecs getRestenemyhp() { return _restenemyhp; }
    vecs getMylastaction() { return _mylastaction; }
    vecs getEnemylastaction() { return _enemylastaction; }
    vecs getRandom() { return _random; }
    vecs getElse() { return _else; }

  private: 
    vecs myLoadText(const char*);

    vecs _restmyhp;
    vecs _restenemyhp;
    vecs _mylastaction;
    vecs _enemylastaction;
    vecs _random;
    vecs _else;
};

#endif