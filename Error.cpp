#include "Error.hpp"
#include <string>
#include "dxlib/DxLib.h"
#include "Define.hpp"

using namespace std;

void Error::finish(char* errorMessage, LPCSTR lpszFuncName, int lineN)
{
  char funcName[1024];
  sprintf(funcName, "%s", lpszFuncName);
  printfDx("異常が発生しました。\n%s\n%s(%d)", errorMessage, funcName, lineN);
  while(!ProcessMessage()) {
    ClearDrawScreen();
    ScreenFlip();
  }
  DxLib_End();
  exit(99);
}