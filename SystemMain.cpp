#include "SystemMain.hpp"
#include "Define.hpp"
#include "Looper.hpp"

bool SystemMain::initialize() {
  SetUse3DFlag(FALSE);
  //文字コードをUTF-8に
  SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
  //非アクティブ時にも処理を続行
  SetAlwaysRunFlag(TRUE);
  //ログを出力しない
  SetOutApplicationLogValidFlag(FALSE);
  //フルスクリーン時に縦横比を維持
  SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_DESKTOP);
  //ウィンドウタイトル
  SetWindowText("GAME");
  //ウィンドウモードに変更
  ChangeWindowMode(TRUE);
  //ウィンドウサイズを変更したいときはここに倍率を指定
  SetWindowSizeExtendRate(1.0);
  //ウィンドウの色を白に
  //SetBackgroundColor(255, 255, 255);

  //色のbit数
  const int COLOR_BIT = 32;
  //ゲーム画面の画面の解像度を設定
  SetGraphMode(Define::WIN_W, Define::WIN_H, COLOR_BIT);
  if (DxLib_Init()) {
    return false;
  }
  //裏画面処理
  SetDrawScreen(DX_SCREEN_BACK);

  //フォントのロード
  _font_path = "Font/GD-DOTFONT-DQ-TTF_008.ttf";
  if(AddFontResourceEx(_font_path, FR_PRIVATE, NULL) <= 0) {
    MessageBox(NULL, "Failed to load Font", "", MB_OK);
  }
  ChangeFont("GD-DOTFONT-DQ-TTF", DX_CHARSET_DEFAULT);
  SetFontSize(24);

  return true;
}

void SystemMain::finalize() const { 
  if(!RemoveFontResourceEx(_font_path, FR_PRIVATE, NULL)){
    MessageBox(NULL, "Remove failure", "", MB_OK);
  }
  DxLib_End();
}

void SystemMain::main() const {
  Looper looper;
  while(!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
    if(!looper.loop()) break;
  }
}