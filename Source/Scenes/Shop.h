#ifndef SHOP_H_
#define SHOP_H_

#include "axmol.h"

#include "GRoot.h"
#include "UIPackage.h"
#include "GComponent.h"

class Shop : public ax::Scene {
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
    void loadStartScreen();

    bool init() override;
    void update(float delta) override;

    void menuCloseCallback(ax::Object* sender);

    // Vacy, add your method declaration here (ex. calculate cost or something)

    Shop();
    ~Shop() override;
};

#endif  // !SHOP_H_
