#include "ProgramScene.hpp"
#include "Define.hpp"
#include "Text.hpp"
#include "dxlib/DxLib.h"

static const int WHITE = GetColor(255, 255, 255);
static const int GRAY = GetColor(169, 169, 169);
static const int BLACK = GetColor(0, 0, 0);
static const int RED = GetColor(255, 0, 0);
static const int CWIDTH = 340;    //コマンドブロックの幅
static const int CHEIGHT = 48;    //コマンドブロックの高さ
static const int DBSIZE = 10;     //コマンドブロック右上のバツボタンのサイズ(正方形)
static const int BWIDTH = 48;     //ボタンの幅
static const int BHEIGHT = 48;    //ボタンの高さ
static const int AWIDTH = (CWIDTH+BWIDTH)/3; //アクションブロックの幅
static const int AHEIGHT = 48;               //アクションブロックの高さ
static const int COMMANDNUM = 6;  //コマンドの数
static const int ACTIONNUM = 3;   //アクションの数
static const int PNT = 32;        //ウィンドウのそれぞれの隅に空けるスペース

//許して☆

ProgramScene::ProgramScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{
  _fontsize = CHEIGHT / 2;
  _startpush = std::make_pair(-1, -1), _endpush = std::make_pair(-1, -1);
  SetFontSize(_fontsize);

  //コマンドリストの初期化(valueなどは入れない)
  int pntx = Define::WIN_W - PNT - CWIDTH, pnty = PNT;
  for(int i=0; i<COMMANDNUM; ++i) {
    _commandlist.emplace_back((commandBlock){pntx, pnty, pntx+CWIDTH, pnty+=CHEIGHT});
  }
  _commandlist[0].type = eCommand::RestMyHp; _commandlist[0].d_name = "ジブンノノコリHPが( )イジョウノトキ...";
  _commandlist[1].type = eCommand::RestEnemyHp; _commandlist[1].d_name = "アイテノノコリHPが( )イジョウノトキ...";
  _commandlist[2].type = eCommand::MyLastAction; _commandlist[2].d_name = "ジブンノサイゴノコウドウが( )ノトキ...";
  _commandlist[3].type = eCommand::EnemyLastAction; _commandlist[3].d_name = "アイテノサイゴノHPが( )ノトキ...";
  _commandlist[4].type = eCommand::Random; _commandlist[4].d_name = "1/( )ノカクリツデ...";
  _commandlist[5].type = eCommand::Else; _commandlist[5].d_name = "ウエノジョウケンニアテハマラナカッタトキ...";
  //アクションリストの初期化
  _actionlist.resize(ACTIONNUM);
  _actionlist.back() = (actionBlock){Define::WIN_W-PNT-AWIDTH, PNT, Define::WIN_W-PNT, PNT+AHEIGHT};
  for(int i=ACTIONNUM-2; i>=0; --i) {
    int x2=_actionlist[i+1].x1, y2=_actionlist[i+1].y2;
    _actionlist[i] = (actionBlock){x2-AWIDTH, PNT, x2, y2};
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

  for(auto &e : _usercommand) {
    if(inRange(e.x2-DBSIZE, e.y1, mouse_x, mouse_y, DBSIZE, DBSIZE)) e.onbutton = true;
    else e.onbutton = false;
    if(
      inRange(e.x2-DBSIZE, e.y1, _startpush.first, _startpush.second, DBSIZE, DBSIZE)
      && inRange(e.x2-DBSIZE, e.y1, _endpush.first, _endpush.second, DBSIZE, DBSIZE)
    ) {
      e.type = eCommand::Empty; e.value = -1; e.d_name = "";
    }
    else if(inRange(e.x1, e.y1, _startpush.first, _startpush.second, CWIDTH, CHEIGHT)) {
      _select_cuser = &e - &_usercommand[0]; _select_category = 0;
    }
  }
  for(auto &e : _useraction) {
    if(inRange(e.x2-DBSIZE, e.y1, mouse_x, mouse_y, DBSIZE, DBSIZE)) e.onbutton = true;
    else e.onbutton = false;
    if(
      inRange(e.x2-DBSIZE, e.y1, _startpush.first, _startpush.second, DBSIZE, DBSIZE)
      && inRange(e.x2-DBSIZE, e.y1, _endpush.first, _endpush.second, DBSIZE, DBSIZE)
    ) {
      e.type = eAction::Empty; e.d_name = "";
    }
    else if(inRange(e.x1, e.y1, _startpush.first, _startpush.second, CWIDTH, CHEIGHT)) {
      _select_auser = &e - &_useraction[0]; _select_category = 1;
    }
  }

  if(_select_category == 0) {
    for(auto &e : _commandlist) {
      if(inRange(e.x1, e.y1, _startpush.first, _startpush.second, CWIDTH, CHEIGHT)) {
        _select_clist = e.type;
      }
    }
  } else {
    for(auto &e : _actionlist) {
      if(inRange(e.x1, e.y1, _startpush.first, _startpush.second, AWIDTH, AHEIGHT)) {
        _select_alist = e.type;
      }
    }
  }
}

void ProgramScene::draw() const {
  if(_select_category == 0) { //commandlist
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
  } else { //actionlist
    for(auto &e : _actionlist) {
      DrawBox(e.x1, e.y1, e.x2+1, e.y2+1, BLACK, TRUE);
      DrawBox(e.x1, e.y1, e.x2+1, e.y2+1, WHITE, FALSE);
      if(e.type == _select_alist) DrawBox(e.x1+4, e.y1+4, e.x2-3, e.y2-3, WHITE, FALSE);
      DrawFormatString(e.x1+_fontsize/2, e.y1+_fontsize/2, WHITE, e.d_name.c_str());
    }
  }

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
