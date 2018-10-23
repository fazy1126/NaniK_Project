#ifndef DEF_TITLESCENE_HPP
#define DEF_TITLESCENE_HPP

#include "AbstractScene.hpp"

class TitleScene : public AbstractScene{
  public:
    TitleScene(IOnSceneChangedListener *impl, const Parameter& parameter);
    virtual ~TitleScene() = default;
    void update() override;
    void draw() const override;
};

#endif