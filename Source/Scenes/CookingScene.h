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
    // Scene states
    GameState _gameState                            = GameState::init;
    ax::EventListenerTouchAllAtOnce* _touchListener = nullptr;
    ax::EventListenerKeyboard* _keyboardListener    = nullptr;
    ax::EventListenerMouse* _mouseListener          = nullptr;
    int _sceneID                                    = 0;

    // Fairygui Objects
    fairygui::GRoot* root;
    fairygui::GList* processList;
    fairygui::GTextField* processText;

    fairygui::GButton* leftArrow;
    fairygui::GButton* rightArrow;
    fairygui::GButton* serveButton;

    fairygui::GList* ingredientList;
    fairygui::GComponent* endPopUpComponent;
    std::vector<fairygui::GButton*> buttons;

    fairygui::GComponent* recipeBookComponent;
    fairygui::GList* recipeBookList;
    fairygui::GButton* recipeBookLeftArrow;
    fairygui::GButton* recipeBookRightArrow;
    int recipeBookIndex = 0;

    // Information of meal made by player
    Meal* currentMeal;
    int actionIndex;

public:
    // Loaders Methods
    void loadStartScreen();
    void loadEndPopUp();

    // Scene initialization methods (override those provided by ax::Scene)
    bool init() override;
    void update(float delta) override;

    // Event methods
    void menuCloseCallback(ax::Object* sender);
    void renderListItems(int index, fairygui::GObject* obj);
    void renderRecipeBookItems(int index, fairygui::GObject* obj);
    void updateElementOnActionList();

    // Overload toString method
    std::string toString(double c) const;

    // Constructors and Destructors
    CookingScene();
    ~CookingScene() override;
};

#endif // !COOKINGSCENE_H_
