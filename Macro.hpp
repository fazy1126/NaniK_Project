#ifndef DEF_MACRO_HPP
#define DEF_MACRO_HPP

#include "Error.hpp"
#include "dxlib/DxLib.h"

#define ERR(str) Error::finish(str,_T(__FUNCTION__),__LINE__)

#endif