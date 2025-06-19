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
#include <iomanip>
#include <functional>

using namespace ax;

static int s_sceneID = 2000;
//sfx from https://jdwasabi.itch.io/8-bit-16-bit-sound-effects-pack

// This method loads in all the components in the Cooking Scene
void CookingScene::loadStartScreen() {
    //load font 
    std::string fontPath = FileUtils::getInstance()->fullPathForFilename("fonts/arial.ttf");
    fairygui::UIConfig::registerFont("arial", fontPath);
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

    // Set to the process list to bake by default
    actionIndex = 0;

    // Find the process GList
    fairygui::GObject* processListObject = processListComponent->as<fairygui::GComponent>()->getChild("n13");
    if (processListObject == nullptr || processListObject->as<fairygui::GList>() == nullptr) {
        std::cerr << "Error: processListObject is null." << std::endl;
        return;
    }
    processList = processListObject->as<fairygui::GList>();
    processList->getScrollPane()->setScrollStep(667); // Set 1 scroll to go 415 pixels

    // Add the left button on the process list
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

    //Add the right button on the process list
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

    // Add the action button (the one that takes the chosen ingredients and bundles it with an action such as "bake")
    fairygui::GObject* actionButtonObject = cookingSceneComponent->getChild("n11");
    if (actionButtonObject != nullptr && actionButtonObject->as<fairygui::GButton>() != nullptr) {
        fairygui::GButton* actionButton = actionButtonObject->as<fairygui::GButton>();
        actionButton->addClickListener([this](fairygui::EventContext* context) {
            //if (currentMeal->getCurrentStep()->getIngredients().size() > 0) {
                std::vector<std::string> cookingProcesses = CookingProcess::getCookingProcesses();
                currentMeal->addStepAction(cookingProcesses[actionIndex]);

                if (currentMeal->getCurrentStep()->getIngredients().size() == 0) {
                    Ingredient placeholder ("Ensemble of used ingredients", 100, "mL");
                    currentMeal->addIngredientToCurrentStep(placeholder);
                    // Create a placeholder ingredient when the user just selects an ingredient
                }
                currentMeal->addNewStep();

                updateElementOnActionList();

                // Play sound
                std::string fullPath = FileUtils::getInstance()->fullPathForFilename("sound/action.wav");
                if (!FileUtils::getInstance()->isFileExist(fullPath))
                    AXLOGE("ERROR: Audio file not found at: %s", fullPath.c_str());
                else
                    int soundId = AudioEngine::play2d("sound/action.wav", false, 1.0f);
            //}
        });
    }

    // Find the ingredient GList (the one in the pantry that contains all the ingredients in the player's inventory
    fairygui::GObject* ingredientListObject = cookingSceneComponent->getChild("n9");
    if (ingredientListObject == nullptr || ingredientListObject->as<fairygui::GList>() == nullptr) {
        std::cerr << "Error: ingredientListObject is null." << std::endl;
        return;
    }

    // Initialize ingredientList and set the itemRenderer (this is a feature that is unique to fairygui, which runs everytime ingredientList has it's number of items reset)
    ingredientList               = ingredientListObject->as<fairygui::GList>();
    ingredientList->itemRenderer = CC_CALLBACK_2(CookingScene::renderListItems, this);
    ingredientList->setNumItems(Player::getInventory().size());

    // Find the textfield to display the actions taken by the player
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
        std::cout << recipe.getRecipeName() << " " << currentMeal->getRecipeAccuracy() << std::endl;
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

    // Button to navigate to the bubble game (the one that lets you acquire new recipes [not yet implemented])
    fairygui::GObject* bubbleButtonObject = cookingSceneComponent->getChild("n17");
    if (bubbleButtonObject == nullptr || bubbleButtonObject->as<fairygui::GButton>() == nullptr) {
        std::cerr << "Error: bubbleButtonObject is null." << std::endl;
        return;
    }
    fairygui::GButton* bubbleButton = bubbleButtonObject->as<fairygui::GButton>();
    bubbleButton->addClickListener([this](fairygui::EventContext* context) {
        Director::getInstance()->replaceScene(utils::createInstance<BubbleGame>());
    });

    // Preload the end popup screen
    fairygui::GObject* endPopUpObject = cookingSceneComponent->getChild("n20");
    if (endPopUpObject == nullptr || endPopUpObject->as<fairygui::GComponent>() == nullptr) {
        std::cerr << "Error: endPopUpObject is null." << std::endl;
        return;
    }
    endPopUpComponent = endPopUpObject->as<fairygui::GComponent>();
    endPopUpComponent->setVisible(false);  // Hide the end popup initially
    endPopUpComponent->setTouchable(false);

    // Initialize the recipe book popup and its button
    fairygui::GObject* recipeBookObject = cookingSceneComponent->getChild("n21");
    if (recipeBookObject == nullptr || recipeBookObject->as<fairygui::GComponent>() == nullptr) {
        std::cerr << "Error: recipeBookObject is null." << std::endl;
        return;
    }
    recipeBookComponent = recipeBookObject->as<fairygui::GComponent>();
    recipeBookComponent->setVisible(false);  // Hide the recipe book initially
    recipeBookComponent->setTouchable(false);

    fairygui::GObject* recipeBookButtonObject = cookingSceneComponent->getChild("n22");
    if (recipeBookButtonObject == nullptr || recipeBookButtonObject->as<fairygui::GButton>() == nullptr) {
        std::cerr << "Error: recipeBookButtonObject is null." << std::endl;
        return;
    }
    fairygui::GButton* recipeBookButton = recipeBookButtonObject->as<fairygui::GButton>();
    recipeBookButton->addClickListener([this](fairygui::EventContext* context) {
        // Toggle the visibility of the recipe book popup
        bool isVisible = recipeBookComponent->isVisible();
        recipeBookComponent->setVisible(!isVisible);
        recipeBookComponent->setTouchable(!isVisible);
        recipeBookList->setNumItems(Player::getPlayerRecipes().size());

        // Play sound
        std::string fullPath = FileUtils::getInstance()->fullPathForFilename("sound/Select.wav");
        if (!FileUtils::getInstance()->isFileExist(fullPath))
            AXLOGE("ERROR: Audio file not found at: %s", fullPath.c_str());
        else
            int soundId = AudioEngine::play2d("sound/Select.wav", false, 1.0f);
    });

    fairygui::GObject* recipeBookListObject = recipeBookComponent->getChild("n5");
    if (recipeBookListObject == nullptr || recipeBookListObject->as<fairygui::GList>() == nullptr) {
        std::cerr << "Error: recipeBookListObject is null." << std::endl;
        return;
    }
    recipeBookList = recipeBookListObject->as<fairygui::GList>();
    recipeBookList->itemRenderer = CC_CALLBACK_2(CookingScene::renderRecipeBookItems, this);
    recipeBookList->setNumItems(Player::getPlayerRecipes().size());  // Set the number of items in the recipe book list
    recipeBookList->getScrollPane()->setScrollStep(816);

    // Add the left and right buttons for the recipe book list
    fairygui::GObject* recipeBookLeftArrowObject = recipeBookComponent->getChild("n3");
    if (recipeBookLeftArrowObject == nullptr || recipeBookLeftArrowObject->as<fairygui::GButton>() == nullptr) {
        std::cerr << "Error: recipeBookLeftArrowObject is null." << std::endl;
        return;
    }
    recipeBookLeftArrow = recipeBookLeftArrowObject->as<fairygui::GButton>();
    recipeBookLeftArrow->addClickListener([this](fairygui::EventContext* context) {
        this->recipeBookList->getScrollPane()->scrollLeft(1, false);

        recipeBookIndex = std::max(recipeBookIndex - 1, 0);
        if (recipeBookIndex == 0) recipeBookLeftArrow->setVisible(false);
        if (recipeBookIndex != Player::getPlayerRecipes().size() / 2) recipeBookRightArrow->setVisible(true);

        std::string fullPath = FileUtils::getInstance()->fullPathForFilename("sound/page_flip.mp3");
        if (!FileUtils::getInstance()->isFileExist(fullPath))
            AXLOGE("ERROR: Audio file not found at: %s", fullPath.c_str());
        else
            int soundId = AudioEngine::play2d("sound/page_flip.mp3", false, 1.0f);
    });
    recipeBookLeftArrow->setVisible(false);  // Hide the left arrow initially

    fairygui::GObject* recipeBookRightArrowObject = recipeBookComponent->getChild("n4");
    if (recipeBookRightArrowObject == nullptr || recipeBookRightArrowObject->as<fairygui::GButton>() == nullptr) {
        std::cerr << "Error: recipeBookRightArrowObject is null." << std::endl;
        return;
    }
    recipeBookRightArrow = recipeBookRightArrowObject->as<fairygui::GButton>();
    recipeBookRightArrow->addClickListener([this](fairygui::EventContext* context) {
        this->recipeBookList->getScrollPane()->scrollRight(1, false);

        recipeBookIndex = std::min(recipeBookIndex + 1, (int)(ceil(Player::getPlayerRecipes().size()/2.0) - 1.0));
        if (recipeBookIndex == (int)(ceil(Player::getPlayerRecipes().size()/2.0) - 1.0)) recipeBookRightArrow->setVisible(false);
        if (recipeBookIndex != 0) recipeBookLeftArrow->setVisible(true);

        std::string fullPath = FileUtils::getInstance()->fullPathForFilename("sound/page_flip.mp3");
        if (!FileUtils::getInstance()->isFileExist(fullPath))
            AXLOGE("ERROR: Audio file not found at: %s", fullPath.c_str());
        else
            int soundId = AudioEngine::play2d("sound/page_flip.mp3", false, 1.0f);
    });

    // Create an empty Meal instance to keep track of the player's actions
    currentMeal = new Meal();

    // Add the fairygui component to root to be displayed
    root->addChild(cookingSceneComponent);
}

void CookingScene::renderRecipeBookItems(int index, fairygui::GObject* obj) {
    std::vector<Recipe> recipes = Player::getPlayerRecipes();

    if (index < 0 || index >= recipes.size()) {
        std::cerr << "Index out of bounds in renderRecipeBookItems: " << index << std::endl;
        return;
    }

    fairygui::GComponent* itemComponent = obj->as<fairygui::GComponent>();
    if (!itemComponent) {
        std::cerr << "Error: itemComponent is null." << std::endl;
        return;
    }

    // Find the title textfield and set it to the name of the recipe
    fairygui::GTextField* recipeName = itemComponent->getChild("n0")->as<fairygui::GTextField>();
    if (!recipeName) {
        std::cerr << "Error: recipeName is null." << std::endl;
        return;
    }
    recipeName->setText(Player::getPlayerRecipes()[index].getRecipeName());

    // Find the description textfield and set it to the description of the recipe
    fairygui::GTextField* recipeDescription = itemComponent->getChild("n1")->as<fairygui::GTextField>();
    if (!recipeDescription) {
        std::cerr << "Error: recipeDescription is null." << std::endl;
        return;
    }
    recipeDescription->setText(Player::getPlayerRecipes()[index].getDescription());

    // Find the ingredients in the recipe and set them to the textfield
    fairygui::GTextField* recipeIngredients = itemComponent->getChild("n2")->as<fairygui::GTextField>();
    if (!recipeIngredients) {
        std::cerr << "Error: recipeIngredients is null." << std::endl;
        return;
    }
    std::string ingredientsText;
    for (Ingredient& ingredient : Player::getPlayerRecipes()[index].getRecipeIngredients()) {
        ingredientsText += ingredient.getName() + ", ";
    }
    recipeIngredients->setText(ingredientsText);

    // Find the steps in the recipe and set them to the textfield
    fairygui::GTextField* recipeSteps = itemComponent->getChild("n3")->as<fairygui::GTextField>();
    if (!recipeSteps) {
        std::cerr << "Error: recipeSteps is null." << std::endl;
        return;
    }
    std::string stepsText;
    for (CookingProcess& step : Player::getPlayerRecipes()[index].getRecipeSteps()) {
        stepsText += step.getName() + " : ";
        for (Ingredient& ingredient : step.getIngredients()) {
            stepsText += ingredient.getName() + ", ";
        }
        stepsText += "\n";
    }
    recipeSteps->setText(stepsText);
}

// Getter for the GList storing the recipes
void CookingScene::updateRecipeBookList() {
    recipeBookList->setNumItems(Player::getPlayerRecipes().size());
}

// This method is used to dynamically update the elements in the pantry GList
void CookingScene::renderListItems(int index, fairygui::GObject* obj) {
    // Get the inventory of the player
    std::vector<Ingredient> inventory = Player::getInventory();

    // Checks if the index in the GList array is actually matching that of the player inventory
    if (index < 0 || index >= inventory.size()) {
        std::cerr << "Index out of bounds in renderRecipeBookItems: " << index << std::endl;
        return;
    }

    // Find the template component to make the ingredient button box
    fairygui::GComponent* itemComponent = obj->as<fairygui::GComponent>();

    // Checks if it exists in the fairygui project
    if (!itemComponent) {
        std::cerr << "Error: itemComponent is null." << std::endl;
        return;
    }

    // Find the label in the component and set it to the name of the ingredient
    fairygui::GTextField* label = itemComponent->getChild("n1")->as<fairygui::GTextField>();
    if (label) label->setText(inventory[index].getName());

    // Find the button and set it's onClick behaviour
    fairygui::GButton* button = itemComponent->getChild("n2")->as<fairygui::GButton>();
    if (!button) {
        std::cerr << "Error: button is null." << std::endl;
        return;
    }

    button->addClickListener([index, this, button](fairygui::EventContext* context) {
        // Find Ingredient
        std::vector<Ingredient> ingredients = Player::getInventory();

        // Selected State
        if (!Player::getIngredientsChosen()[index]) {
            currentMeal->addIngredientToCurrentStep(ingredients[index]);
            button->setAlpha(0.5f);  // Set button to semi-transparent
            Player::setIngredientsChosen(index, true);
            std::cout << "Added ingredient: " << ingredients[index].getName() << std::endl;
        }

        // Unselected State
        else {
            currentMeal->removeIngredientFromCurrentStep(ingredients[index]);
            button->setAlpha(0.0f);  // Set button to fully opaque
            Player::setIngredientsChosen(index, false);
            std::cout << "Removed ingredient: " << ingredients[index].getName() << std::endl;
        }

        // Play sound
        std::string fullPath = FileUtils::getInstance()->fullPathForFilename("sound/ingredientSelect.mp3");
        if (!FileUtils::getInstance()->isFileExist(fullPath))
            AXLOGE("ERROR: Audio file not found at: %s", fullPath.c_str());
        else
            int soundId = AudioEngine::play2d("sound/ingredientSelect.mp3", false, 1.0f);

        // updateElementOnActionList();
    });
    // Add to button vector to modify later if necessary
    buttons.push_back(button);

    // Load the image associated with the ingredient
    fairygui::GLoader* imgLoader = itemComponent->getChild("n0")->as<fairygui::GLoader>();
    if (!imgLoader) {
        std::cerr << "Error: imgLoader is null." << std::endl;
        return;
    }
    imgLoader->setURL("UI/Assets/" + std::to_string(inventory[index].getNameIndex() + 3) + ".png");
    std::cout << ("UI/Assets/" + std::to_string(inventory[index].getNameIndex() + 3) + ".png") << std::endl;

    // Update the textfield to show the quantity of that ingredient in the player's inventory
    fairygui::GTextField* ingredientAmountText = itemComponent->getChild("n6")->as<fairygui::GTextField>();
    if (!ingredientAmountText) {
        std::cerr << "Error: ingredientAmountText is null." << std::endl;
        return;
    }
    int amount                          = inventory[index].getQuantity();
    ingredientAmountText->setText(std::to_string(amount));
}

// This method loads in the pop up that shows up after the player "serves" his meal
void CookingScene::loadEndPopUp() {
    // Show the end popup
    endPopUpComponent->setVisible(true);
    endPopUpComponent->setTouchable(true);

    // Find the closest matching recipe to the one the user made
    Recipe recipe = currentMeal->findMatchingRecipe();

    // Display what dish was served on a GTextField
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

    // Gives money to the user based on the found recipe's price and how accurate the player was to it
    double profitFromMeal = recipe.getCost() * (currentMeal->getRecipeAccuracy() * 1.5);
    Player::addMoney(profitFromMeal);

    // Display how close (in percentage) the meal made the player was to the found recipe
    fairygui::GObject* accuracyTextObject = endPopUpComponent->getChild("n7");
    if (accuracyTextObject == nullptr || accuracyTextObject->as<fairygui::GTextField>() == nullptr) {
        std::cerr << "Error: accuracyTextObject is null." << std::endl;
        return;
    }
    fairygui::GTextField* accuracyText = accuracyTextObject->as<fairygui::GTextField>();

    // The meal made by the player didn't match well with any of the recipes in recipes.json
    if (currentMeal->getRecipeAccuracy() < 0.5) accuracyText->setText("Please do better...");
    else {
        accuracyText->setText(
            "Recipe Accuracy: " + toString(currentMeal->getRecipeAccuracy() * 100.0)        // Show accuracy
            + "% (" + toString(recipe.getCost() * currentMeal->getRecipeAccuracy()) + "$)"  // Show money gained
        );
    }

    // Find and set the restart button behaviour
    fairygui::GObject* cookNewDishButtonObject = endPopUpComponent->getChild("n9");
    if (cookNewDishButtonObject == nullptr || cookNewDishButtonObject->as<fairygui::GButton>() == nullptr) {
        std::cerr << "Error: cookNewDishButtonObject is null." << std::endl;
        return;
    }
    fairygui::GButton* cookNewDishButton = cookNewDishButtonObject->as<fairygui::GButton>();
    cookNewDishButton->addClickListener([this](fairygui::EventContext* context) {
        // Hide the end popup screen
        endPopUpComponent->setVisible(false);
        endPopUpComponent->setTouchable(false);

        // Deduct the ingredients used by the player from their inventory
        currentMeal->substractUsedIngredients();

        // Create a new instance of this scene [This is a temporary solution until a better one is found]
        Director::getInstance()->replaceScene(utils::createInstance<CookingScene>());
    });
}

// Overload toString method to ensure two decimal places
std::string CookingScene::toString(double c) const {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << c;
    return ss.str();
}

// This method is used to dynamically update the text inside of the action Textfield (the one that displays all the action taken by the player)
void CookingScene::updateElementOnActionList() {
    // Initialize a string to show all the actions taken by the player to display on the GTextfield
    std::string textToDisplay = "";

    // Cycle through the steps taken by the player
    for (CookingProcess& step : currentMeal->getRecipeSteps()) {
        // Add which process was used at each step
        textToDisplay += step.getName() + " : ";

        // Cycle through the ingredients used in that step to show along the process
        for (Ingredient& i : step.getIngredients()) {
            textToDisplay += i.getName() + ", ";
        }
        textToDisplay += "\n";
    }

    // If there is indeed actions taken by the player, display it on the GTextfield
    if (textToDisplay != "") {
        processText->setText(textToDisplay);

        // Go back and uncheck every buttons in the pantry
        for (int i = 0; i < buttons.size(); i++) {
            Player::setIngredientsChosen(i, false);  // Reset the selection state for all ingredients
            buttons[i]->setAlpha(0.0f);
        }
    }
}

// Overload the init() method from ax::Scene to run the loadStartScreen() upon launch
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
