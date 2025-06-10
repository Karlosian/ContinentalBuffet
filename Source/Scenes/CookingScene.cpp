#include "CookingScene.h"

#include "../Classes/Ingredient.h"

#include "GRoot.h"
#include "UIPackage.h"
#include "GComponent.h"
#include "GButton.h"
#include "GList.h"
#include "GGraph.h"
#include "GLabel.h"
#include "GLoader.h"
#include "GTextField.h"
#include "AudioEngine.h"

#include <iostream>
#include <functional>

using namespace ax;

static int s_sceneID = 2000;
//sfx from https://jdwasabi.itch.io/8-bit-16-bit-sound-effects-pack
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
            std::string fullPath = FileUtils::getInstance()->fullPathForFilename("sound/Select.wav");
            if (!FileUtils::getInstance()->isFileExist(fullPath))
            {
                AXLOGE("ERROR: Audio file not found at: %s", fullPath.c_str());
            }
            else
            {
                // AXLOGD("Audio file found at: %s", fullPath.c_str());
                int soundId = AudioEngine::play2d("sound/Select.wav", false, 1.0f);
            }
        });
    }

    //Add the down button
    fairygui::GObject* downArrowObject = cookingSceneComponent->getChild("n5");
    if (downArrowObject != nullptr && downArrowObject->as<fairygui::GButton>() != nullptr) {
        fairygui::GButton* downArrow = downArrowObject->as<fairygui::GButton>();
        downArrow->addClickListener([this](fairygui::EventContext* context) {
            this->processList->getScrollPane()->scrollDown(1, true);
            std::string fullPath = FileUtils::getInstance()->fullPathForFilename("sound/Select.wav");
            if (!FileUtils::getInstance()->isFileExist(fullPath))
            {
                AXLOGE("ERROR: Audio file not found at: %s", fullPath.c_str());
            }
            else
            {
                //AXLOGD("Audio file found at: %s", fullPath.c_str());
                int soundId = AudioEngine::play2d("sound/Select.wav", false, 1.0f);
            }
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

    std::vector<std::string> labels;
    for (Ingredient ingredient : ingredients) {
        labels.push_back(ingredient.getName());
    }

    // Add render here
    //ingredientList->setDefaultItem("ui://UI/MyButtonListItem");
    ingredientList->itemRenderer = CC_CALLBACK_2(CookingScene::renderListItems, this, labels);
    ingredientList->setNumItems(ingredients.size());

    root->addChild(cookingSceneComponent);
}

void CookingScene::renderListItems(int index, fairygui::GObject* obj, const std::vector<std::string>& labels) {
    if (index >= 0 && index < labels.size()) {
        fairygui::GComponent* itemComponent = obj->as<fairygui::GComponent>();
        if (itemComponent) {
            fairygui::GTextField* label = itemComponent->getChild("n1")->as<fairygui::GTextField>();
            if (label) {
                label->setText(labels[index]);
            }
            fairygui::GButton* button = itemComponent->getChild("n2")->as<fairygui::GButton>();
            if (button) {
                button->addClickListener([index](fairygui::EventContext* context) {
                    std::vector<Ingredient> ingredients = Ingredient::getIngredients();
                    std::cout << ingredients[index].getName() << std::endl;
                    // Handle button click for the specific ingredient
                });
            }
            fairygui::GLoader* imgLoader = itemComponent->getChild("n0")->as<fairygui::GLoader>();
            if (imgLoader) {
                imgLoader->setURL("UI/Assets/" + std::to_string(index + 3) + ".png");

            }
        }
    }
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
