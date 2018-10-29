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
  bool pressing = false;
  bool delete_p = false;
};
struct commandBlock {
  int x1, y1, x2, y2;
  std::string d_name = ""; //画面に表示する名前
  eCommand type;  //コマンドタイプ
  int value = -1; //入力される値
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

class ProgramScene : public AbstractScene {
  public: 
    ProgramScene(IOnSceneChangedListener* impl, const Parameter& parameter);
    virtual ~ProgramScene() = default;
    void update() override;
    void draw() const override;

  private: 
    int _fontsize; //フォントサイズ
    int _dfontsize; //実際に画面に表示されるフォントサイズ
    bool _oldmouseinput_left = false, _oldmouseinput_right = false; //前のフレームでクリックされていたか
    std::pair<int, int> _startpush_left, _endpush_left, _startpush_right, _endpush_right; //クリックが始まった座標と終わった座標
    eCommand _select_clist = eCommand::Empty; //commandlist(右)で選択されている項目
    eAction _select_alist = eAction::Empty;  //actionlistで選択されている項目
    int _select_cuser = 0; //usercommand(左)で選択されている項目
    int _select_auser = 0; //useraction
    bool _select_category = 0; //commandlistに表示されるカテゴリー(0:commandlist 1:action)
    
    Button _useraddbutton; //コマンド・アクションを追加するボタン
    std::vector<Button> _tabbutton;     //commandlistとactionlistを切り替えるタブボタン
    std::vector<Button> _deletebutton;  //usrecommand/useractionを一行消去

    std::vector<commandBlock> _usercommand; //ユーザーが組み立てるコマンドブロック
    std::vector<actionBlock> _useraction; //ユーザーが組み立てるアクションブロック
    std::vector<commandBlock> _commandlist; //コマンドのリスト(右側)
    std::vector<actionBlock> _actionlist; //アクションのリスト(右側)

    std::map<eCommand, std::vector<std::string>> _cdescription; //Key:コマンドの名前 value:説明文
    std::map<eAction, std::vector<std::string>> _adescription; //アクションブロックの説明文
    bool inRange(int nx, int ny, int mx, int my, int rangex, int rangey);
    template <typename T>
    void deleteBlock(T& list);
    template <typename T>
    void dressBlock(T& list, int x1, int y1, int x2, int y2);
};

#endif