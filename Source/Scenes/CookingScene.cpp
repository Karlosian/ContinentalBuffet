#include "CookingScene.h"
#include "Shop.h"
#include "BubbleGame.h"

#include "../Classes/Ingredient.h"
#include "../Classes/Player.h"

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
    processList->getScrollPane()->setScrollStep(667); // Set 1 scroll to go 415 pixels

    // Set to bake by default
    actionIndex = 0;

    // Add the left button
    fairygui::GObject* leftArrowObject = cookingSceneComponent->getChild("n4");
    if (leftArrowObject != nullptr && leftArrowObject->as<fairygui::GButton>() != nullptr) {
        leftArrow = leftArrowObject->as<fairygui::GButton>();
        leftArrow->addClickListener([this](fairygui::EventContext* context) {
            this->processList->getScrollPane()->scrollLeft(1, true);
            std::string fullPath = FileUtils::getInstance()->fullPathForFilename("sound/Select.wav");
            actionIndex          = std::max(actionIndex - 1, 0);

            // Update visibility of arrows based on actionIndex (at the start of the list, hide the Left arrow)
            if (actionIndex == 0) leftArrow->setVisible(false);
            if (actionIndex != 11) rightArrow->setVisible(true);

            // Play click sound
            if (!FileUtils::getInstance()->isFileExist(fullPath)) AXLOGE("ERROR: Audio file not found at: %s", fullPath.c_str());
            else int soundId = AudioEngine::play2d("sound/Select.wav", false, 1.0f);
        });
        leftArrow->setVisible(false);  // Hide the up arrow initially
    }

    //Add the right button
    fairygui::GObject* rightArrowObject = cookingSceneComponent->getChild("n5");
    if (rightArrowObject != nullptr && rightArrowObject->as<fairygui::GButton>() != nullptr) {
        rightArrow = rightArrowObject->as<fairygui::GButton>();
        rightArrow->addClickListener([this](fairygui::EventContext* context) {
            this->processList->getScrollPane()->scrollRight(1, true);
            std::string fullPath = FileUtils::getInstance()->fullPathForFilename("sound/Select.wav");
            actionIndex          = std::min(actionIndex + 1, 11);
            if (actionIndex == 11) rightArrow->setVisible(false);
            if (actionIndex != 0)  leftArrow->setVisible(true);

            // Play click sound
            if (!FileUtils::getInstance()->isFileExist(fullPath)) AXLOGE("ERROR: Audio file not found at: %s", fullPath.c_str());
            else int soundId = AudioEngine::play2d("sound/Select.wav", false, 1.0f);
        });
    }

    // Add the action button
    fairygui::GObject* actionButtonObject = cookingSceneComponent->getChild("n11");
    if (actionButtonObject != nullptr && actionButtonObject->as<fairygui::GButton>() != nullptr) {
        fairygui::GButton* actionButton = actionButtonObject->as<fairygui::GButton>();
        actionButton->addClickListener([this](fairygui::EventContext* context) {
            if (currentMeal->getCurrentStep()->getIngredients().size() > 0) {
                std::vector<std::string> cookingProcesses = CookingProcess::getCookingProcesses();
                currentMeal->addStepAction(cookingProcesses[actionIndex]);
                currentMeal->addNewStep();
                updateElementOnActionList();
            }
        });
    }

    fairygui::GObject* ingredientListObject = cookingSceneComponent->getChild("n9");
    if (ingredientListObject == nullptr || ingredientListObject->as<fairygui::GList>() == nullptr) {
        std::cerr << "Error: ingredientListObject is null." << std::endl;
        return;
    }

    ingredientList = ingredientListObject->as<fairygui::GList>();
    std::vector<Ingredient> ingredients = Player::getInventory();

    std::vector<std::string> labels;
    for (Ingredient ingredient : ingredients) {
        labels.push_back(ingredient.getName());
    }

    // Add render here
    ingredientList->itemRenderer = CC_CALLBACK_2(CookingScene::renderListItems, this, labels);
    ingredientList->setNumItems(ingredients.size());

    fairygui::GObject* processTextObject = cookingSceneComponent->getChild("n8");
    if (processTextObject == nullptr || processTextObject->as<fairygui::GTextField>() == nullptr) {
        std::cerr << "Error: processTextObject is null." << std::endl;
        return;
    }
    processText = processTextObject->as<fairygui::GTextField>();
    processText->setText("Your actions will appear here");

    // Button to serve the meal (Finish button basically)
    fairygui::GObject* serveButtonObject = cookingSceneComponent->getChild("n13");
    if (serveButtonObject == nullptr || serveButtonObject->as<fairygui::GButton>() == nullptr) {
        std::cerr << "Error: serveButtonObject is null." << std::endl;
        return;
    }
    serveButton = serveButtonObject->as<fairygui::GButton>();
    serveButton->addClickListener([this](fairygui::EventContext* context) {
        Recipe recipe = currentMeal->findMatchingRecipe();
        std::cout << recipe.name << " " << currentMeal->getRecipeAccuracy() << std::endl;
        loadEndPopUp();  // Load the end popup screen
    });

    // Button to navigate to the shop
    fairygui::GObject* shopButtonObject = cookingSceneComponent->getChild("n16");
    if (shopButtonObject == nullptr || shopButtonObject->as<fairygui::GButton>() == nullptr) {
        std::cerr << "Error: shopButtonObject is null." << std::endl;
        return;
    }
    fairygui::GButton* shopButton = shopButtonObject->as<fairygui::GButton>();
    shopButton->addClickListener([this](fairygui::EventContext* context) {
        Director::getInstance()->replaceScene(utils::createInstance<Shop>());
    });

    // Button to navigate to the bubble game
    fairygui::GObject* bubbleButtonObject = cookingSceneComponent->getChild("n17");
    if (bubbleButtonObject == nullptr || bubbleButtonObject->as<fairygui::GButton>() == nullptr) {
        std::cerr << "Error: bubbleButtonObject is null." << std::endl;
        return;
    }
    fairygui::GButton* bubbleButton = bubbleButtonObject->as<fairygui::GButton>();
    bubbleButton->addClickListener([this](fairygui::EventContext* context) {
        Director::getInstance()->replaceScene(utils::createInstance<BubbleGame>());
    });

    currentMeal = new Meal();

    // Load the end popup screen
    fairygui::GObject* endPopUpObject = cookingSceneComponent->getChild("n20");
    if (endPopUpObject == nullptr || endPopUpObject->as<fairygui::GComponent>() == nullptr) {
        std::cerr << "Error: endPopUpObject is null." << std::endl;
        return;
    }
    endPopUpComponent = endPopUpObject->as<fairygui::GComponent>();
    endPopUpComponent->setVisible(false);  // Hide the end popup initially
    endPopUpComponent->setTouchable(false);

    root->addChild(cookingSceneComponent);
}

void CookingScene::loadEndPopUp() {
    endPopUpComponent->setVisible(true);
    endPopUpComponent->setTouchable(true);

    Recipe recipe = currentMeal->findMatchingRecipe();

    fairygui::GObject* dishServed = endPopUpComponent->getChild("n8");
    if (dishServed == nullptr || dishServed->as<fairygui::GTextField>() == nullptr) {
        std::cerr << "Error: dishServed is null." << std::endl;
        return;
    }
    fairygui::GTextField* dishServedText = dishServed->as<fairygui::GTextField>();
    if (currentMeal->getRecipeAccuracy() < 0.5) {
        dishServedText->setText("Unknown Substance (Failed)");
    }
    else {
        dishServed->setText(recipe.getRecipeName());
    }

    fairygui::GObject* accuracyTextObject = endPopUpComponent->getChild("n7");
    if (accuracyTextObject == nullptr || accuracyTextObject->as<fairygui::GTextField>() == nullptr) {
        std::cerr << "Error: accuracyTextObject is null." << std::endl;
        return;
    }
    fairygui::GTextField* accuracyText = accuracyTextObject->as<fairygui::GTextField>();
    if (currentMeal->getRecipeAccuracy() < 0.5) {
        accuracyText->setText("Please do better...");
    }
    else {
        accuracyText->setText("Recipe Accuracy: " + toString(currentMeal->getRecipeAccuracy() * 100.0) + "%.");
    }

    fairygui::GObject* cookNewDishButtonObject = endPopUpComponent->getChild("n9");
    if (cookNewDishButtonObject == nullptr || cookNewDishButtonObject->as<fairygui::GButton>() == nullptr) {
        std::cerr << "Error: cookNewDishButtonObject is null." << std::endl;
        return;
    }

    fairygui::GButton* cookNewDishButton = cookNewDishButtonObject->as<fairygui::GButton>();
    cookNewDishButton->addClickListener([this](fairygui::EventContext* context) {
        endPopUpComponent->setVisible(false);
        endPopUpComponent->setTouchable(false);

        currentMeal->substractUsedIngredients();
        currentMeal = new Meal();  // Reset the current meal for a new cooking session
        for (int i = 0; i < actionList.size(); i++) {
            actionList.pop_back();  // Clear the action list
        }

        ingredientList->setNumItems(Player::getInventory().size());
        actionIndex = 0;              // Reset action index
    });
}

// Overload toString method to ensure two decimal places
std::string CookingScene::toString(double c) const {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << c;
    return ss.str();
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
                button->addClickListener([index, this, button](fairygui::EventContext* context) {
                    std::vector<Ingredient> ingredients = Player::getInventory();
                    std::cout << ingredients[index].getName() << std::endl;
                    if (!Player::getIngredientsChosen()[index]) {
                        currentMeal->addIngredientToCurrentStep(ingredients[index]);
                        button->setAlpha(0.5f);  // Set button to semi-transparent
                        Player::setIngredientsChosen(index, true);
                        std::cout << "Added ingredient: " << ingredients[index].getName() << std::endl;
                    }

                    else {
                        currentMeal->removeIngredientFromCurrentStep(ingredients[index]);
                        button->setAlpha(0.0f);  // Set button to fully opaque
                        Player::setIngredientsChosen(index, false);
                        std::cout << "Removed ingredient: " << ingredients[index].getName() << std::endl;
                    }
                    //updateElementOnActionList();
                });
                buttons.push_back(button);
            }

            fairygui::GLoader* imgLoader = itemComponent->getChild("n0")->as<fairygui::GLoader>();
            if (imgLoader) {
                std::vector<Ingredient> inventory = Player::getInventory();
                imgLoader->setURL("UI/Assets/" + std::to_string(inventory[index].getNameIndex() + 3) + ".png");
                std::cout << ("UI/Assets/" + std::to_string(inventory[index].getNameIndex() + 3) + ".png") << std::endl;
            }

            fairygui::GTextField* ingredientAmountText = itemComponent->getChild("n6")->as<fairygui::GTextField>();
            if (ingredientAmountText) {
                std::vector<Ingredient> ingredients = Player::getInventory();
                int amount = ingredients[index].getQuantity();
                ingredientAmountText->setText(std::to_string(amount));
            }
        }

        std::vector<Ingredient> ingredients = Player::getInventory();
        std::cout << "Bum : " << labels[index] << " : " << ingredients[index].getQuantity() << std::endl;
    }
}

void CookingScene::updateElementOnActionList() {
    std::string textToDisplay = "";
    for (CookingProcess& step : currentMeal->getRecipeSteps()) {
        textToDisplay += step.getName() + " : ";
        for (Ingredient& i : step.getIngredients()) {
            textToDisplay += i.getName() + ", ";
        }
        textToDisplay += "\n";
    }

    std::cout << "Current Meal Steps: " << textToDisplay << std::endl;

    if (textToDisplay != "") {
        processText->setText(textToDisplay);
        for (int i = 0; i < buttons.size(); i++) {
            Player::setIngredientsChosen(i, false);  // Reset the selection state for all ingredients
            buttons[i]->setAlpha(0.0f);
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
