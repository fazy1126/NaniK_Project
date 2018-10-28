#include "ProgramScene.hpp"
#include "Define.hpp"
#include "Text.hpp"
#include "dxlib/DxLib.h"

static const int WHITE = GetColor(255, 255, 255);
static const int GRAY = GetColor(169, 169, 169);
static const int BLACK = GetColor(0, 0, 0);
static const int RED = GetColor(255, 0, 0);
static const int CWIDTH = 351;    //コマンドブロックの幅
static const int CHEIGHT = 48;    //コマンドブロックの高さ
static const int DBSIZE = 10;     //コマンドブロック右上のバツボタンのサイズ(正方形)
static const int AWIDTH = (CWIDTH)/3; //アクションブロックの幅
static const int AHEIGHT = 48;        //アクションブロックの高さ
static const int COMMANDNUM = 6;  //コマンドの数
static const int ACTIONNUM = 3;   //アクションの数
static const int PNT = 32;        //ウィンドウのそれぞれの隅に空けるスペース

//許して☆

ProgramScene::ProgramScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{
  _fontsize = CHEIGHT / 2;
  _startpush_left = _endpush_left = _startpush_right = _endpush_right = std::make_pair(-1, -1);
  SetFontSize(_fontsize);

  //コマンドリストの初期化(valueなどは入れない)
  _commandlist.resize(COMMANDNUM);
  int pntx = Define::WIN_W - PNT - CWIDTH, pnty = PNT + CHEIGHT / 2;
  for(int i=0; i<COMMANDNUM; ++i) {
    _commandlist[i] = {pntx, pnty, pntx+CWIDTH, pnty+=CHEIGHT};
  }
  _commandlist[0].type = eCommand::RestMyHp; _commandlist[0].d_name = "ジブンノノコリHPが( )イジョウノトキ...";
  _commandlist[1].type = eCommand::RestEnemyHp; _commandlist[1].d_name = "アイテノノコリHPが( )イジョウノトキ...";
  _commandlist[2].type = eCommand::MyLastAction; _commandlist[2].d_name = "ジブンノサイゴノコウドウが( )ノトキ...";
  _commandlist[3].type = eCommand::EnemyLastAction; _commandlist[3].d_name = "アイテノサイゴノHPが( )ノトキ...";
  _commandlist[4].type = eCommand::Random; _commandlist[4].d_name = "1/( )ノカクリツデ...";
  _commandlist[5].type = eCommand::Else; _commandlist[5].d_name = "ウエノジョウケンニアテハマラナカッタトキ...";
  //アクションリストの初期化
  _actionlist.resize(ACTIONNUM);
  _actionlist.back() = (actionBlock){Define::WIN_W-PNT-AWIDTH, PNT+CHEIGHT/2, Define::WIN_W-PNT, PNT+AHEIGHT+CHEIGHT/2};
  for(int i=ACTIONNUM-2; i>=0; --i) {
    int x2=_actionlist[i+1].x1, y2=_actionlist[i+1].y2;
    _actionlist[i] = (actionBlock){x2-AWIDTH, PNT+CHEIGHT/2, x2, y2};
  }
  _actionlist[0].type = eAction::Attack; _actionlist[0].d_name = "コウゲキ";
  _actionlist[1].type = eAction::Block; _actionlist[1].d_name = "ボウギョ";
  _actionlist[2].type = eAction::CounterAttack; _actionlist[2].d_name = "ハンゲキ";

  //説明文の初期化
  _cdescription.emplace(eCommand::RestMyHp, Text::getIns()->getRestmyhp());
  _cdescription.emplace(eCommand::RestEnemyHp, Text::getIns()->getRestenemyhp());
  _cdescription.emplace(eCommand::MyLastAction, Text::getIns()->getMylastaction());
  _cdescription.emplace(eCommand::EnemyLastAction, Text::getIns()->getEnemylastaction());
  _cdescription.emplace(eCommand::Random, Text::getIns()->getRandom());
  _cdescription.emplace(eCommand::Else, Text::getIns()->getElse());
  _adescription.emplace(eAction::Attack, Text::getIns()->getAttack());
  _adescription.emplace(eAction::Block, Text::getIns()->getBlock());
  _adescription.emplace(eAction::CounterAttack, Text::getIns()->getCounterattack());

  //コマンド組み立てリストの初期化(ユーザー側) elseのみを追加
  pntx = PNT, pnty = PNT;
  _usercommand.emplace_back(
    (commandBlock){pntx, pnty, pntx+CWIDTH, pnty+CHEIGHT, _commandlist[5].d_name, _commandlist[5].type}
  );
  _useraction.emplace_back(
    (actionBlock){pntx+CWIDTH+_fontsize*2+20, pnty, pntx+CWIDTH+_fontsize*2+20+AWIDTH, pnty+AHEIGHT}
  );
  _useraction[0].type = eAction::Empty;

  //ボタンの初期化
  _useraddbutton = {
    _usercommand.back().x1, Define::WIN_H-AHEIGHT/2, _useraction.back().x2+AHEIGHT, Define::WIN_H, "+"
  };
  pntx = Define::WIN_W - CWIDTH - PNT, pnty = PNT;
  _tabbutton = {
    (Button){pntx, pnty, pntx+100, pnty+CHEIGHT/2, "コマンド"},
    (Button){pntx+100, pnty, pntx+100*2, pnty+CHEIGHT/2, "コウドウ"}
  };
  pntx = _useraction.back().x2 + 10, pnty = _useraction.back().y1+AHEIGHT/4;
  _deletebutton = {
    (Button){pntx, pnty, pntx+AHEIGHT/2, pnty+AHEIGHT/2, "-"}
  };
}

void ProgramScene::update() {
  //マウスの状態を更新
  int mouse_x, mouse_y, wheel;
  GetMousePoint(&mouse_x, &mouse_y);
  wheel = GetMouseWheelRotVol();
  if(!_oldmouseinput_left && GetMouseInput() & MOUSE_INPUT_LEFT) {
    _startpush_left = std::make_pair(mouse_x, mouse_y);
    _endpush_left = std::make_pair(-1, -1);
    _oldmouseinput_left = true;
  }
  else if(_oldmouseinput_left && !(GetMouseInput() & MOUSE_INPUT_LEFT)) {
    _endpush_left = std::make_pair(mouse_x, mouse_y);
    _oldmouseinput_left = false;
  }
  if(!_oldmouseinput_right && GetMouseInput() & MOUSE_INPUT_RIGHT) {
    _startpush_right = std::make_pair(mouse_x, mouse_y);
    _endpush_right = std::make_pair(-1, -1);
    _oldmouseinput_right = true;
  }
  else if(_oldmouseinput_right && !(GetMouseInput() & MOUSE_INPUT_RIGHT)) {
    _endpush_right = std::make_pair(mouse_x, mouse_y);
    _oldmouseinput_right = false;
  }

  //usercommand
  for(auto &e : _usercommand) {
    if(inRange(e.x2-DBSIZE, e.y1, mouse_x, mouse_y, DBSIZE, DBSIZE)) e.onbutton = true;
    else e.onbutton = false;
    if(
      inRange(e.x2-DBSIZE, e.y1, _startpush_left.first, _startpush_left.second, DBSIZE, DBSIZE)
      && inRange(e.x2-DBSIZE, e.y1, _endpush_left.first, _endpush_left.second, DBSIZE, DBSIZE)
    ) {
      e.type = eCommand::Empty; e.value = -1; e.d_name = "";
    }
    else if(inRange(e.x1, e.y1, _startpush_left.first, _startpush_left.second, CWIDTH, CHEIGHT)) {
      _select_cuser = &e - &_usercommand[0]; _select_category = 0;
    }
    else if(
      inRange(e.x1, e.y1, _startpush_right.first, _startpush_right.second, CWIDTH, CHEIGHT)
      && inRange(e.x1, e.y1, _endpush_right.first, _endpush_right.second, CWIDTH, CHEIGHT)
    ) {
      e.type = eCommand::Empty; e.value = -1; e.d_name = "";
    }
  }
  //useraction
  for(auto &e : _useraction) {
    if(inRange(e.x2-DBSIZE, e.y1, mouse_x, mouse_y, DBSIZE, DBSIZE)) e.onbutton = true;
    else e.onbutton = false;
    if(
      inRange(e.x2-DBSIZE, e.y1, _startpush_left.first, _startpush_left.second, DBSIZE, DBSIZE)
      && inRange(e.x2-DBSIZE, e.y1, _endpush_left.first, _endpush_left.second, DBSIZE, DBSIZE)
    ) {
      e.type = eAction::Empty; e.d_name = "";
    }
    else if(inRange(e.x1, e.y1, _startpush_left.first, _startpush_left.second, AWIDTH, AHEIGHT)) {
      _select_auser = &e - &_useraction[0]; _select_category = 1;
    }
    else if(
      inRange(e.x1, e.y1, _startpush_right.first, _startpush_right.second, AWIDTH, AHEIGHT)
      && inRange(e.x1, e.y1, _endpush_right.first, _endpush_right.second, AWIDTH, AHEIGHT)
    ) {
      e.type = eAction::Empty; e.d_name = "";
    }
  }
  //common(usercommand&useraction)
  auto userc = _usercommand.back(); auto usera = _useraction.back();
  if(inRange(PNT, 0, mouse_x, mouse_y, (usera.x2-PNT), Define::WIN_H)) {
    int dy = 0;
    if(wheel > 0) dy = 20;
    else if(wheel < 0) dy = -20;
    for(auto &e : _usercommand) {
      e.y1 += dy; e.y2 += dy;
    }
    for(auto &e : _useraction) {
      e.y1 += dy; e.y2 += dy;
    }
    for(auto &e : _deletebutton) {
      e.y1 += dy; e.y2 += dy;
    }
  }
  //button
  if(
    inRange(_useraddbutton.x1, _useraddbutton.y1, _startpush_left.first, _startpush_left.second, (_useraddbutton.x2-_useraddbutton.x1), (_useraddbutton.y2-_useraddbutton.y1))
    && inRange(_useraddbutton.x1, _useraddbutton.y1, _endpush_left.first, _endpush_left.second, (_useraddbutton.x2-_useraddbutton.x1), (_useraddbutton.y2-_useraddbutton.y1))
  ){
    _usercommand.emplace_back(
      (commandBlock){_usercommand.back().x1, _usercommand.back().y2, _usercommand.back().x2, _usercommand.back().y2+CHEIGHT}
    );
    _useraction.emplace_back(
      (actionBlock){_useraction.back().x1, _useraction.back().y2, _useraction.back().x2, _useraction.back().y2+AHEIGHT}
    );
    _deletebutton.emplace_back(
      (Button){_deletebutton.back().x1, _deletebutton.back().y2+AHEIGHT/2, _deletebutton.back().x2, _deletebutton.back().y2+AHEIGHT, "-"}
    );
  }
  for(auto &e : _tabbutton) {
    if(inRange(e.x1, e.y1, _startpush_left.first, _startpush_left.second, e.x2-e.x1, e.y2-e.y1)) {
      if(&e - &_tabbutton[0] == 0) _select_category = 0; //Command
      else _select_category = 1; //Action
    }
  }
  for(auto &e : _deletebutton) {
    if(inRange(e.x1, e.y1, _startpush_left.first, _startpush_left.second, e.x2-e.x1, e.y2-e.y1)) {
      e.pressing = true;
      if(_usercommand.size() > 1 && inRange(e.x1, e.y1, _endpush_left.first, _endpush_left.second, e.x2-e.x1, e.y2-e.y1)) {
        int idx = &e-&_deletebutton[0];
        _deletebutton[idx].delete_p = true;
        _usercommand[idx].delete_p = true;
        _useraction[idx].delete_p = true;
      }
    }
    else e.pressing = false;
  }
  //list
  if(_select_category == 0) {
    for(auto &e : _commandlist) {
      if(inRange(e.x1, e.y1, _startpush_left.first, _startpush_left.second, CWIDTH, CHEIGHT)) {
        _select_clist = e.type;
      }
      else if(
        _select_clist == e.type
        && inRange(e.x1, e.y1, _startpush_right.first, _startpush_right.second, CWIDTH, CHEIGHT)
        && inRange(e.x1, e.y1, _endpush_right.first, _endpush_right.second, CWIDTH, CHEIGHT)
      ) {
          auto tmp = _usercommand[_select_cuser];
          tmp.d_name = e.d_name; tmp.type = e.type;
          _usercommand[_select_cuser] = tmp;
      }
    }
  } else {
    for(auto &e : _actionlist) {
      if(inRange(e.x1, e.y1, _startpush_left.first, _startpush_left.second, AWIDTH, AHEIGHT)) {
        _select_alist = e.type;
      }
      else if(
        _select_alist == e.type
        && inRange(e.x1, e.y1, _startpush_right.first, _startpush_right.second, CWIDTH, CHEIGHT)
        && inRange(e.x1, e.y1, _endpush_right.first, _endpush_right.second, CWIDTH, CHEIGHT)
      ) {
          auto tmp = _useraction[_select_auser];
          tmp.d_name = e.d_name; tmp.type = e.type;
          _useraction[_select_auser] = tmp;
      }
    }
  }

  deleteBlock(_usercommand);
  dressBlock(_usercommand, PNT, PNT, PNT+CWIDTH, PNT+CHEIGHT);
  deleteBlock(_useraction);
  dressBlock(_useraction, PNT+CWIDTH+_fontsize*2+20, PNT, PNT+CWIDTH+_fontsize*2+20+AWIDTH, PNT+AHEIGHT);
  int dcnt = 0;
  for(int i=0; i<_deletebutton.size(); i++) if(_deletebutton[i].delete_p){ 
    _deletebutton[i].delete_p=false, dcnt++;
  }
  for(int i=0; i<dcnt; i++) _deletebutton.erase(_deletebutton.end()-1);
  if(_endpush_left != std::pair<int, int>(-1, -1)) {
    _startpush_left = _endpush_left = std::make_pair(-1, -1);
  }
  if(_endpush_right != std::pair<int, int>(-1, -1)) {
    _startpush_right = _endpush_left = std::make_pair(-1, -1);
  }
}

void ProgramScene::draw() const {
  //list
  if(_select_category == 0) {
    for(auto &e : _commandlist) {
      DrawBox(e.x1, e.y1, e.x2+1, e.y2+1, BLACK, TRUE);   //背景色(塗りつぶし)
      DrawBox(e.x1, e.y1, e.x2+1, e.y2+1, WHITE, FALSE); //枠
      if(e.type == _select_clist) DrawBox(e.x1+4, e.y1+4, e.x2-3, e.y2-3, WHITE, FALSE); //選択されていたら枠を追加
      DrawFormatString(e.x1+_fontsize/2, e.y1+_fontsize/2, WHITE, e.d_name.c_str());
    }
    int y = _commandlist.back().y2-1 + CHEIGHT;
    if(_select_clist != eCommand::Empty) {
      for(auto &e : _cdescription.at(_select_clist)) {
        DrawFormatString(_commandlist.back().x1, y, WHITE, e.c_str()); y+=_fontsize;
      }
    }
  }
  else {
    for(auto &e : _actionlist) {
      DrawBox(e.x1, e.y1, e.x2+1, e.y2+1, BLACK, TRUE);
      DrawBox(e.x1, e.y1, e.x2+1, e.y2+1, WHITE, FALSE);
      if(e.type == _select_alist) DrawBox(e.x1+4, e.y1+4, e.x2-3, e.y2-3, WHITE, FALSE);
      DrawFormatString(e.x1+_fontsize/2, e.y1+_fontsize/2, WHITE, e.d_name.c_str());
    }
  }
  //user
  for(auto &e : _usercommand) {
    //背景色
    DrawBox(e.x1, e.y1, e.x2+1, e.y2+1, BLACK, TRUE);
    if(&e-&_usercommand[0] == _select_cuser) DrawBox(e.x1+4, e.y1+4, e.x2-3, e.y2-3, WHITE, FALSE);
    //バツボタン描画
    if(e.onbutton) DrawBox(e.x2-DBSIZE, e.y1, e.x2, e.y1+DBSIZE+1, RED, TRUE);
    else DrawBox(e.x2-DBSIZE, e.y1, e.x2+1, e.y1+DBSIZE+1, GRAY, TRUE);
    DrawLine(e.x2-DBSIZE, e.y1, e.x2+1, e.y1+DBSIZE+1, WHITE, 1);
    DrawLine(e.x2-1, e.y1, e.x2-DBSIZE+1, e.y1+DBSIZE+1, WHITE, 1);
    //枠
    DrawBox(e.x1, e.y1, e.x2+1, e.y2+1, WHITE, FALSE);
    DrawFormatString(e.x1+_fontsize/2, e.y1+_fontsize/2, WHITE, e.d_name.c_str());
    DrawFormatString(e.x2+10, e.y1, WHITE, "->");
  }
  for(auto &e : _useraction) {
    //背景色
    DrawBox(e.x1, e.y1, e.x2+1, e.y2+1, BLACK, TRUE);
    if(&e-&_useraction[0] == _select_auser) DrawBox(e.x1+4, e.y1+4, e.x2-3, e.y2-3, WHITE, FALSE);
    //バツボタン描画
    if(e.onbutton) DrawBox(e.x2-DBSIZE, e.y1, e.x2, e.y1+DBSIZE+1, RED, TRUE);
    else DrawBox(e.x2-DBSIZE, e.y1, e.x2+1, e.y1+DBSIZE+1, GRAY, TRUE);
    DrawLine(e.x2-DBSIZE, e.y1, e.x2+1, e.y1+DBSIZE+1, WHITE, 1);
    DrawLine(e.x2-1, e.y1, e.x2-DBSIZE+1, e.y1+DBSIZE+1, WHITE, 1);
    //枠
    DrawBox(e.x1, e.y1, e.x2+1, e.y2+1, WHITE, FALSE);
    DrawFormatString(e.x1+_fontsize/2, e.y1+_fontsize/2, WHITE, e.d_name.c_str());
  }
  //button
  DrawBox(_useraddbutton.x1, _useraddbutton.y1, _useraddbutton.x2+1, _useraddbutton.y2+1, WHITE, TRUE);
  DrawFormatString(_useraddbutton.x1+(_useraddbutton.x2-_useraddbutton.x1)/2-_fontsize/2, _useraddbutton.y1-_fontsize/2+3,
    BLACK, _useraddbutton.name.c_str());
  for(auto &e : _tabbutton) {
    int scolor = WHITE, bcolor = BLACK;
    if(&e - &_tabbutton[0] == _select_category) {
      scolor = BLACK, bcolor = WHITE;
    }
    DrawBox(e.x1, e.y1, e.x2+1, e.y2+1, bcolor, TRUE);
    DrawFormatString(e.x1+_fontsize/2, e.y1-3, scolor, e.name.c_str());
  }
  for(auto &e : _deletebutton) {
    DrawBox(e.x1, e.y1, e.x2+1, e.y2+1, WHITE, FALSE);
    if(e.pressing) DrawBox(e.x1+3, e.y1+3, e.x2-2, e.y2-2, WHITE, FALSE);
    DrawFormatString(e.x1+_fontsize/4, e.y1-_fontsize/4, WHITE, e.name.c_str());
  }
}

/*
nx/ny:対象物の左上の座標
mx/my:マウスカーソルの座標
rangex/rangey:範囲
*/
bool ProgramScene::inRange(int nx, int ny, int mx, int my, int rangex, int rangey) {
  if(nx <= mx && mx <= nx+rangex && ny <= my && my <= ny+rangey) return true;
  else return false;
}

template <typename T>
void ProgramScene::deleteBlock(T& list) {
  auto itr = list.begin();
  while(itr != list.end()) {
    if(itr->delete_p) {
      itr = list.erase(itr);
    } else {
      itr++;
    }
  }
}

template <typename T>
void ProgramScene::dressBlock(T& list, int x1, int y1, int x2, int y2) {
  list[0].x1 = x1; list[0].y1 = y1; list[0].x2 = x2; list[0].y2 = y2;
  for(int i=1; i<list.size(); i++) {
    list[i].y1 = list[i-1].y2;
    list[i].y2 = list[i].y1 + (list[i-1].y2 - list[i-1].y1);
  }
}