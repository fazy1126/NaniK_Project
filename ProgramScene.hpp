#ifndef DEF_PROGRAMSCENE_HPP
#define DEF_PROGRAMSCENE_HPP

#include "AbstractScene.hpp"
#include "eCommand.hpp"

struct menuElement {
  
};

class ProgramScene : public AbstractScene {
  public: 
    ProgramScene(IOnSceneChangedListener* impl, const Parameter& parameter);
    virtual ~ProgramScene() = default;
    void update() override;
    void draw() const override;
};

#endif