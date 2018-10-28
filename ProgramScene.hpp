#ifndef DEF_PROGRAMSCENE_HPP
#define DEF_PROGRAMSCENE_HPP

#include "AbstractScene.hpp"
#include "eCommand.hpp"
#include "eAction.hpp"
#include <vector>
#include <map>

/*
x1/y1: 左端上の座標
x2/y2: 右端下の座標
*/
struct Button {
  int x1, y1, x2, y2;
  std::string name;
};
struct commandBlock {
  int x1, y1, x2, y2;
  std::string d_name = ""; //画面に表示する名前
  eCommand type;  //コマンドタイプ
  eAction action; //コウドウ
  int value = -1; //入力される値
  bool onbutton = false; //バツボタンの上にカーソルが乗っているか
};

class ProgramScene : public AbstractScene {
  public: 
    ProgramScene(IOnSceneChangedListener* impl, const Parameter& parameter);
    virtual ~ProgramScene() = default;
    void update() override;
    void draw() const override;

  private: 
    int _fontsize; //フォントサイズ
    bool _oldmouseinput = false; //前のフレームで左クリックされていたか
    std::pair<int, int> _startpush, _endpush; //クリックが始まった座標と終わった座標
    eCommand _select_list = eCommand::Empty; //commandlist(右)で選択されている項目
    int _select_user = -1; //usercommand(左)で選択されている項目
    bool _select_category = 0; //commandlistに表示されるカテゴリー(0:commandlist 1:action)
    std::vector<commandBlock> _usercommand; //ユーザーが組み立てるコマンドブロック
    std::vector<commandBlock> _commandlist; //コマンドのリスト(右側)
    std::vector<Button> _button_user; //ユーザーコマンドと
    std::map<eCommand, std::vector<std::string>> _description; //Key:コマンドの名前 value:説明文

    bool inRange(int nx, int ny, int mx, int my, int rangex, int rangey);
};

#endif