#ifndef DEF_IONCHANGEDLISTENER_HPP
#define DEF_IONCHANGEDLISTENER_HPP

#include "eScene.hpp"
#include "AbstractScene.hpp"
#include "Parameter.hpp"

class IOnSceneChangedListener {
  public:
    IOnSceneChangedListener() = default;
    virtual ~IOnSceneChangedListener() = default;
    virtual void onSceneChanged(const eScene scene, const Parameter& parameter, const bool stackClear) = 0;
};

#endif