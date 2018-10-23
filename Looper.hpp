#ifndef DEF_LOOPER_HPP
#define DEF_LOOPER_HPP

#include <stack>
#include <memory>
#include "AbstractScene.hpp"
#include "IOnChangedListener.hpp"

class Looper final : public IOnSceneChangedListener {
  public:
    Looper();
    ~Looper() = default;
    bool loop() const;
    void onSceneChanged(const eScene scene, const Parameter& parameter, const bool stackClear) override;

  private:
    std::stack<std::shared_ptr<AbstractScene>> _sceneStack;
};

#endif