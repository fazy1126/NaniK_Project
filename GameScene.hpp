#ifndef DEF_GAMESCENE_HPP
#define DEF_GAMESCENE_HPP

#include "AbstractScene.hpp"
#include <string>

class GameScene : public AbstractScene {
  public:
    const static std::string ParameterTagStage;
    const static std::string ParameterTagLevel;
    
    GameScene(IOnSceneChangedListener *impl, const Parameter& parameter);
    virtual ~GameScene() = default;

    void initialize() override;
    void update() override;
    void draw() const override;

  private: 
    int _level;
};

#endif