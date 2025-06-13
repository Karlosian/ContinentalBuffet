#ifndef HOMEPAGE_H_
#define HOMEPAGE_H_

#include "axmol.h"

#include "GRoot.h"
#include "UIPackage.h"
#include "GComponent.h"

class Homepage : public ax::Scene {
    enum class GameState
    {
        init = 0,
        update,
        pause,
        end,
        menu1,
        menu2,
    };

private:
    GameState _gameState                            = GameState::init;
    ax::EventListenerTouchAllAtOnce* _touchListener = nullptr;
    ax::EventListenerKeyboard* _keyboardListener    = nullptr;
    ax::EventListenerMouse* _mouseListener          = nullptr;
    int _sceneID                                    = 0;

    fairygui::GRoot* root;

public:
    // Loader methods
    void loadStartScreen();

    // Overriten update methods from ax::Scene
    bool init() override;
    void update(float delta) override;
    void menuCloseCallback(ax::Object* sender);

    // Constructors and destructors
    Homepage();
    ~Homepage() override;
};

#endif  // !HOMEPAGE_H_
