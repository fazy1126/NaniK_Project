#ifndef DEF_ABSTRACTSCENE_HPP
#define DEF_ABSTRACTSCENE_HPP

#include "IOnChangedListener.hpp"
#include "Parameter.hpp"

class IOnSceneChangedListener;

class AbstractScene {
  protected:
    IOnSceneChangedListener* _implSceneChanged;
  public:
    AbstractScene(IOnSceneChangedListener* impl, const Parameter& parameter);
    virtual ~AbstractScene() = default;
    virtual void initialize() = 0;
    virtual void update() = 0;
    virtual void draw() const = 0;
};

#endif