#include "CookingScene.h"

#include "../Classes/Ingredient.h"

#include "GRoot.h"
#include "UIPackage.h"
#include "GComponent.h"
#include "GButton.h"
#include "GList.h"
#include "GGraph.h"
#include "GLabel.h"
#include "GTextField.h"

#include <iostream>
#include <functional>

using namespace ax;

static int s_sceneID = 2000;

void CookingScene::loadStartScreen() {
    // Load the cooking scene UI package and add it to the scene
    fairygui::UIPackage::addPackage("UI/CookingPackage");
    fairygui::GComponent* cookingSceneComponent = fairygui::UIPackage::createObject("Package1", "Component1")->as<fairygui::GComponent>();
    cookingSceneComponent->setPosition(0, 0);

    // Find the component containing the process list
    fairygui::GObject* processListComponent = cookingSceneComponent->getChild("n2");
    if (processListComponent == nullptr || processListComponent->as<fairygui::GComponent>() == nullptr) {
        std::cerr << "Error: processListComponent is null." << std::endl;
        return;
    }

    // Find the process GList
    fairygui::GObject* processListObject = processListComponent->as<fairygui::GComponent>()->getChild("n13");
    if (processListObject == nullptr || processListObject->as<fairygui::GList>() == nullptr) {
        std::cerr << "Error: processListObject is null." << std::endl;
        return;
    }

    processList = processListObject->as<fairygui::GList>();
    processList->getScrollPane()->setScrollStep(415); // Set 1 scroll to go 666 pixels

    // Add the up button
    fairygui::GObject* upArrowObject = cookingSceneComponent->getChild("n4");
    if (upArrowObject != nullptr && upArrowObject->as<fairygui::GButton>() != nullptr) {
        fairygui::GButton* upArrow = upArrowObject->as<fairygui::GButton>();
        upArrow->addClickListener([this](fairygui::EventContext* context) {
            this->processList->getScrollPane()->scrollUp(1, true);
        });
    }

    //Add the down button
    fairygui::GObject* downArrowObject = cookingSceneComponent->getChild("n5");
    if (downArrowObject != nullptr && downArrowObject->as<fairygui::GButton>() != nullptr) {
        fairygui::GButton* downArrow = downArrowObject->as<fairygui::GButton>();
        downArrow->addClickListener([this](fairygui::EventContext* context) {
            this->processList->getScrollPane()->scrollDown(1, true);
        });
    }

    // Add the action button
    fairygui::GObject* actionButtonObject = cookingSceneComponent->getChild("n5");
    if (actionButtonObject != nullptr && actionButtonObject->as<fairygui::GButton>() != nullptr) {
        fairygui::GButton* actionButton = actionButtonObject->as<fairygui::GButton>();
        actionButton->addClickListener([this](fairygui::EventContext* context) {
            // Run action
        });
    }

    fairygui::GObject* ingredientListObject = cookingSceneComponent->getChild("n9");
    if (ingredientListObject == nullptr || ingredientListObject->as<fairygui::GList>() == nullptr) {
        std::cerr << "Error: ingredientListObject is null." << std::endl;
        return;
    }
    fairygui::GList* ingredientList = ingredientListObject->as<fairygui::GList>();

    Ingredient::getIngredientList();
    std::vector<Ingredient> ingredients = Ingredient::getIngredients();

    fairygui::GButton* item = fairygui::GButton::create();
    //std::cout << item << std::endl;

    for (int i = 0; i < ingredients.size(); i++) {
        // Create a GButton instance
        fairygui::GButton* button = fairygui::GButton::create();

        // --- Customize the button's appearance ---
        // By default, GButton::create() is very basic.
        // You need to add children to it to make it visually appealing.

        // Add a background image/shape (e.g., a simple colored rectangle)
        fairygui::GGraph* buttonBg = fairygui::GGraph::create();
        buttonBg->drawRect(150, 150,                               // width, height
                           2,                                     // border size
                           ax::Color4F::WHITE,                    // border color
                           ax::Color4F(0.0f, 0.5f, i * 0.1f, 1.0f));  // fill color (blue)
        
        //button->addChild(buttonBg);                               // Add the background to the button

        fairygui::GLabel* text = fairygui::GLabel::create();
        text->setTitle("ASDADAD");
        text->setTitleFontSize(20);  // Set the font size for the text
        text->setAlpha(1.0f);        // Set the text opacity
        text->setTitleColor(ax::Color3B::WHITE);  // Set the text color
        button->addChild(text);

        // Set the button's size to match its content
        button->setSize(200, 50);  // Set the button's actual size

        // Set a name for identification in the click handler
        button->name = ax::StringUtils::format("button_%d", i + 1);

        button->setTitle("Ingredient 1");  // Set the button's title
        button->addClickListener([this, button, i](fairygui::EventContext* context) {
            // Handle button click
            std::cout << "Button " << i + 1 << " clicked!" << std::endl;
            button->setAlpha(0.5f);  // Example action: change button opacity
        });

        // Add the button to the GList
        ingredientList->addChild(button);
    }

    root->addChild(cookingSceneComponent);
}

bool CookingScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = _director->getVisibleSize();
    auto origin      = _director->getVisibleOrigin();
    auto safeArea    = _director->getSafeAreaRect();
    auto safeOrigin  = safeArea.origin;

    root = fairygui::GRoot::create(this);
    root->retain();
    CookingScene::loadStartScreen();

    scheduleUpdate();
    return true;
}

void CookingScene::update(float delta) {
    
}

void CookingScene::menuCloseCallback(ax::Object* sender) {
    _director->end();
}

CookingScene::CookingScene() {
    _sceneID = s_sceneID++;
}

CookingScene::~CookingScene() {
    _sceneID = -1;
}
