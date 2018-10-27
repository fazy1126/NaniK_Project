#ifndef DEF_PROGRAMSCENE_HPP
#define DEF_PROGRAMSCENE_HPP

#include "AbstractScene.hpp"
#include "eCommand.hpp"
#include <vector>

/*
x1/y1: 左端上の座標
x2/y2: 右端下の座標
*/
struct commandBlock {
  int x1, y1, x2, y2;
  std::string name = "";
  std::string d_name = "";
  int value = -1;
  eCommand type;
};

class ProgramScene : public AbstractScene {
  public: 
    ProgramScene(IOnSceneChangedListener* impl, const Parameter& parameter);
    virtual ~ProgramScene() = default;
    void update() override;
    void draw() const override;

  private: 
    int _fontsize;
    bool _oldmouseinput = false;
    std::pair<int, int> _startpush, _endpush;
    std::vector<commandBlock> _command;
    std::vector<commandBlock> _commandlist;
    std::map<std::string, std::vector<std::string>> _description; //Key:コマンドの名前 value:説明文

    bool inRange(int nx, int ny, int mx, int my, int rangex, int rangey);
    std::pair<bool, std::vector<std::string>> getFileContents(std::string);
};

#endif