#ifndef COOKINGSCENE_H_
#define COOKINGSCENE_H_

#include <axmol.h>

#include "GRoot.h"
#include "UIPackage.h"
#include "GComponent.h"
#include "GTextField.h"
#include "GButton.h"
#include "GList.h"
#include "GObject.h"

#include "../Classes/Meal.h"

class CookingScene : public ax::Scene {
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
    fairygui::GList* processList;
    fairygui::GTextField* processText;

    fairygui::GButton* leftArrow;
    fairygui::GButton* rightArrow;
    fairygui::GButton* serveButton;

    fairygui::GList* ingredientList;
    fairygui::GComponent* endPopUpComponent;

    std::vector<fairygui::GButton*> buttons;
    Meal* currentMeal;
    int actionIndex;

public:
    void loadStartScreen();
    void loadEndPopUp();

    bool init() override;
    void update(float delta) override;

    void menuCloseCallback(ax::Object* sender);
    void renderListItems(int index, fairygui::GObject* obj);
    void updateElementOnActionList();

    // Overload toString method
    std::string toString(double c) const;

    CookingScene();
    ~CookingScene() override;
};

#endif // !COOKINGSCENE_H_
