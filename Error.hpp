#ifndef DEF_ERROR_HPP
#define DEF_ERROR_HPP

#include <Windows.h>

class Error {
  public:
    static void finish(char* errorMessage, LPCSTR lpszFuncName, int lineN);
};

#endif