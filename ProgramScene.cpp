#include "ProgramScene.hpp"
#include "Define.hpp"
#include <fstream>
#include "dxlib/DxLib.h"

static const int WHITE = GetColor(255, 255, 255);
static const int GRAY = GetColor(169, 169, 169);
static const int BLACK = GetColor(0, 0, 0);
static const int WIDTH = 340;    //コマンドブロックの幅
static const int HEIGHT = 48;    //コマンドブロックの高さ
static const int COMMANDNUM = 6; //コマンドの数
static const int PNT = 32;       //ウィンドウのそれぞれの隅に空けるスペース

//許して☆
using namespace std;

ProgramScene::ProgramScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{
  _fontsize = HEIGHT / 2;
  SetFontSize(_fontsize);

  //コマンドリストの初期化(valueなどは入れない)
  int pntx = Define::WIN_W - PNT - WIDTH, pntx2 = pntx + WIDTH, pnty = PNT;
  for(int i=0; i<COMMANDNUM; ++i) {
    _commandlist.emplace_back((commandBlock){pntx, pnty, pntx2, pnty+=HEIGHT});
  }
  _commandlist[0].name = "RestMyHp"; _commandlist[0].d_name = "ジブンノノコリHPが( )イジョウノトキ...";
  _commandlist[1].name = "RestEnemyHp"; _commandlist[1].d_name = "アイテノノコリHPが( )イジョウノトキ...";
  _commandlist[2].name = "MyLastAction"; _commandlist[2].d_name = "ジブンノサイゴノコウドウが( )ノトキ...";
  _commandlist[3].name = "EnemyLastAction"; _commandlist[3].d_name = "アイテノサイゴノHPが( )ノトキ...";
  _commandlist[4].name = "Random"; _commandlist[4].d_name = "1/( )ノカクリツデ...";
  _commandlist[5].name = "Else"; _commandlist[5].d_name = "ウエノジョウケンニアテハマラナカッタトキ...";
}

void ProgramScene::update() {
  //マウスの状態を更新
  int mouse_x, mouse_y;
  GetMousePoint(&mouse_x, &mouse_y);
  if(!_oldmouseinput && GetMouseInput() & MOUSE_INPUT_LEFT) {
    _startpush = std::make_pair(mouse_x, mouse_y);
    _endpush = std::make_pair(5000, 5000);
    _oldmouseinput = true;
  }
  else if(_oldmouseinput && !(GetMouseInput() & MOUSE_INPUT_LEFT)) {
    _endpush = std::make_pair(mouse_x, mouse_y);
    _oldmouseinput = false;
  }


}

void ProgramScene::draw() const {
  for(auto &e : _commandlist) {
    DrawBox(e.x1, e.y1, e.x2, e.y2, GRAY, TRUE);
    DrawBox(e.x1, e.y1, e.x2, e.y2, BLACK, FALSE);
    DrawFormatString(e.x1+_fontsize/2, e.y1+_fontsize/2, WHITE, e.d_name.c_str());
  }
  for(auto &e : _command) {
  }
}

bool ProgramScene::inRange(int nx, int ny, int mx, int my, int rangex, int rangey) {
  if(nx-rangex <= mx && mx <= nx+rangex && ny-rangey <= my && my <= ny+rangey) return true;
  else return false;
}

pair<bool, vector<string>> getFileContents(string path) {
  ifstream ifs(path.c_str());
  string str;
  vector<string> res;
  if(ifs.fail()) {
    MessageBox(NULL, "Failed to load file", "ERROR", MB_OK);
    return make_pair(false, res);
  }
  while(getline(ifs, str)) {
    res.emplace_back(str);
  }
  return make_pair(true, res);
}