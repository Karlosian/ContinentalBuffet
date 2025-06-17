#include "BubbleGame.h"
#include "AudioEngine.h"
#include "FairyGUI.h"
#include "GComponent.h"
#include "GButton.h"
#include "GObject.h"
#include "CookingScene.h"
#include "../Classes/Player.h"

#include <iostream>

USING_NS_AX;






BubbleGame* BubbleGame::createScene()
{
    return BubbleGame::create();
}


void BubbleGame::startGame(){
   // Reset score
    score = 0;
    updateScoreLabel();

    // Start game timer
    float gameDuration = 30.0f;

    this->scheduleOnce([=](float) {
        endGame(); 
    }, gameDuration, "game_end");

    // Start spawning bubbles
    this->schedule([=](float) {
        spawnBubble();  
    }, 1.0f, "bubble_spawn");  // spawn every 1 second


    this->schedule([this](float) {
    timeLeft--;
    timerLabel->setString("Time: " + std::to_string(timeLeft));

    if (timeLeft <= 0) {
        this->unschedule("game_timer");
        endGame();  
    }
}, 1.0f, "game_timer");


    //show UI elements (timer, score, etc.)
    scoreLabel->setVisible(true);
    timerLabel->setVisible(true);    
    return;
}

void BubbleGame::endGame(){
    fairygui::GObject* backObj = comp->getChild("n0");
    fairygui::GButton* backButton = backObj->as<fairygui::GButton>();


     //stop all scheduled functions
    this->unschedule("bubble_spawn");
    this->unschedule("game_timer");
    this->unschedule("game_end");
    
    scoreLabel->setVisible(false);
    timerLabel->setVisible(false);
    
    // search the children of BubbleGame, "//" represents all nodes
    this->enumerateChildren("bubble", [](Node* node) -> bool {
        auto sprite = dynamic_cast<Sprite*>(node);
            //remove
            sprite->stopAllActions();
            sprite->removeFromParent();
        
        return false; // Continue enumeration
});
    
    //create game over screen
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto overlay = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(overlay, 100, "game_over_overlay");
    
    auto gameOverLabel = Label::createWithTTF("GAME OVER!", "fonts/Marker Felt.ttf", 64);
    gameOverLabel->setPosition(origin + Vec2(visibleSize.width/2, visibleSize.height/2 + 150));
    gameOverLabel->setColor(Color3B::RED);
    this->addChild(gameOverLabel, 101);
    
    //final score display
    auto finalScoreLabel = Label::createWithTTF("Final Score: " + std::to_string(score), "fonts/Marker Felt.ttf", 36);
    finalScoreLabel->setPosition(origin + Vec2(visibleSize.width/2, visibleSize.height/2 + 70));
    finalScoreLabel->setColor(Color3B::WHITE);
    this->addChild(finalScoreLabel, 101);
 
    //make button visible
    comp->setPosition(visibleSize.width/2 - 75, visibleSize.height/2 +100);
    //add listener
    backButton->addClickListener([](fairygui::EventContext* context) {
            Director::getInstance()->replaceScene(utils::createInstance<CookingScene>());
        });
    //add label stating what reward you got for playing
    int rewardInfo = parsePlayerReward(score);

    ax::Label* rewardLabel = nullptr;
        switch (rewardInfo)
        {
        case -1:
        rewardLabel = Label::createWithTTF("Rewarded: $" + std::to_string(score), "fonts/Marker Felt.ttf", 36);
            break;

        case -2:
            rewardLabel = Label::createWithTTF("Rewarded: $" + std::to_string(score/2.0), "fonts/Marker Felt.ttf", 36);

        default:
            rewardLabel = 
            Label::createWithTTF("Rewarded: $" + std::to_string(score/2.0) + " and the recipe\n" + Recipe::getAllRecipes()[rewardInfo].getRecipeName(), 
            "fonts/Marker Felt.ttf", 36);
            break;
        }


    rewardLabel->setPosition(origin + Vec2(visibleSize.width/2, visibleSize.height/2));
    rewardLabel->setColor(Color3B::WHITE);
    this->addChild(rewardLabel, 101);


    //add  animation to the game over screen
    gameOverLabel->setScale(0.1f);
    gameOverLabel->runAction(ScaleTo::create(0.3f, 1.0f));

    return;
}

void BubbleGame::spawnBubble(){
    //create the bubble sprite
    auto bubble = ax::Sprite::create("BubbleGame/bubble.png");
    if (!bubble) return;
    
    bubble->setName("bubble");
    //scale and disable anti-aliasing for pixel art
    bubble->setScale(4.0f);
    bubble->getTexture()->setAliasTexParameters(); 
    


    //random X position along the screen width, with a margin of 50px
    auto visibleSize = ax::Director::getInstance()->getVisibleSize();
    float randomX = AXRANDOM_0_1() * (visibleSize.width - 100) + 50;

    //start position at y=0
    bubble->setPosition(Vec2(randomX, 0));

    //add bubble to the scene
    this->addChild(bubble, 5);

    //create upward movement and fade out actions
    float duration = 0.8f;  // how long the bubble floats up
    //move up and fade out
    auto moveUp = ax::MoveBy::create(duration, Vec2(0, visibleSize.height));
    auto fadeOut = ax::FadeOut::create(duration);
    auto upward = ax::Spawn::createWithTwoActions(moveUp, fadeOut);

    //wiggle side to side
    auto wiggleRight = ax::MoveBy::create(0.2f, Vec2(50, 0));
    auto wiggleLeft = ax::MoveBy::create(0.2f, Vec2(-50, 0));
    auto wiggleSeq = ax::Sequence::create(wiggleRight, wiggleLeft, nullptr);
    auto wiggleRepeat = ax::RepeatForever::create(wiggleSeq);

    //cleanup when done
    auto cleanup = ax::CallFunc::create([bubble]() {
    bubble->removeFromParent();
});

// Run upward + fade, then cleanup
bubble->runAction(ax::Sequence::create(upward, cleanup, nullptr));
// Run wiggle forever
bubble->runAction(wiggleRepeat);

    //add touch event listener for popping bubble
    auto listener = ax::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this, bubble](ax::Touch* touch, ax::Event* event) {
        Vec2 locationInNode = bubble->convertToNodeSpace(touch->getLocation());
        Size s = bubble->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);

        if (rect.containsPoint(locationInNode)) {
            //pop bubble on touch
            score += 1;                  //increase score
            updateScoreLabel();          //update score display

            //play pop sound
            ax::AudioEngine::play2d("sound/pop.wav");

            //show a pop animation and remove bubble
            bubble->stopAllActions();
            bubble->runAction(ax::Sequence::create(
                ax::ScaleTo::create(0.1f, 1.5f),
                ax::FadeOut::create(0.1f),
                ax::CallFunc::create([bubble]() { bubble->removeFromParent(); }),
                nullptr
            ));
            return true;  // Consume touch
        }
        return false;     // Did not touch bubble
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, bubble);
}

void BubbleGame::updateScoreLabel(){
    scoreLabel->setString("Score: " + std::to_string(score));
}

void BubbleGame::startCountdown() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto countdownLabel = Label::createWithSystemFont("3", "Arial", 96);
    countdownLabel->setPosition(origin + visibleSize / 2);
    this->addChild(countdownLabel, 10, "countdown");

    //Vector of actions that happen in an amount of time
    Vector<FiniteTimeAction*> actions;

    std::vector<std::string> countdownTexts = {"3", "2", "1", "GO!"};

    //iterate over countdownTexts
    for (const auto& text : countdownTexts) {
        //create action
        actions.pushBack(CallFunc::create([=]() {
            countdownLabel->setString(text);
            //make small
            countdownLabel->setScale(0.1f);
            countdownLabel->setColor(Color3B::RED);
            countdownLabel->setOpacity(255);
            //make bigger in 0.3 seconds
            countdownLabel->runAction(ScaleTo::create(0.3f, 1.0f));
            //sfx
              if (text == "GO!") {
            AudioEngine::play2d("sound/go.wav");
        } else {
            AudioEngine::play2d("sound/beep.wav");
        }
        }));
        actions.pushBack(DelayTime::create(1.0f));
    }
    //remove countdown label from scene
    actions.pushBack(CallFunc::create([=]() {
        this->removeChildByName("countdown");

        //now start the game
        this->startGame();
    }));

    countdownLabel->runAction(Sequence::create(actions));
}

bool BubbleGame::init()
{
    if (!Scene::init())
       { return false;}

    //debug
    auto listener = EventListenerKeyboard::create();
listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        AXLOGD("ESC pressed - calling endGame()");
        this->endGame();
    }
};
_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
       
    
    this->createUI();
    this->startCountdown();

    return true;
}

void BubbleGame::createUI() {
//fairygui init
    root = fairygui::GRoot::create(this);
    root->retain();
    
    //initialize ui package
    fairygui::UIPackage::addPackage("UI/BackButton");
    comp = fairygui::UIPackage::createObject("BackButton", "BackButtonComponent")->as<fairygui::GComponent>();
    if (!comp) {
        AXLOGE("Error: Failed to create FairyGUI component.");
    }

//add button
    if (comp && root) {
    comp->setPosition(-9999, -9999);
    root->addChild(comp);
    }
auto bg = Sprite::create("BubbleGame/bg.png");
bg->setAnchorPoint(Vec2::ZERO);
bg->setPosition(Vec2::ZERO);

//get visible screen size
auto visibleSize = Director::getInstance()->getVisibleSize();
auto bgSize = bg->getContentSize();

//scale to fit screen
bg->setScaleX(visibleSize.width / bgSize.width);
bg->setScaleY(visibleSize.height / bgSize.height);

this->addChild(bg, -1);    

//create score label
scoreLabel = ax::Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 24);
scoreLabel->setPosition(Vec2(100, 680));  
scoreLabel->setColor(ax::Color3B::WHITE);
this->addChild(scoreLabel, 10);

//create timerLabel
timerLabel = ax::Label::createWithTTF("Time: 30", "fonts/Marker Felt.ttf", 24);
timerLabel->setPosition(Vec2(1200, 680)); 
timerLabel->setColor(ax::Color3B::WHITE);
this->addChild(timerLabel, 10);

}

//returns -1 if adding just money, -2 if all recipes are obtained, or the index of the recipe that it adds to the player's inventory
int BubbleGame::parsePlayerReward(int score) {
    int i = -1;
    //add money equivalent to score if under 15
    if (score < 15) {
        Player::addMoney((double)score);
        return i;
    }

    //add money half of score
    Player::addMoney((double)score / 2.0);

    bool isValidIndex = false;
    std::vector<Recipe> allRecipes = Recipe::getAllRecipes();
    std::vector<Recipe> playerRecipes = Player::getPlayerRecipes();

    //safety check: if player has all recipes
    if (playerRecipes.size() == allRecipes.size()) {
            return -2;
        }

    //find valid index of recipe    
    while (!isValidIndex) {
        i = std::floor(AXRANDOM_0_1() * allRecipes.size());
        Recipe potentialRecipe = allRecipes[i];

        //check if the recipe is already in the player's inventory
        bool alreadyOwned = false;
        for (Recipe r : playerRecipes) {
            if (r.getRecipeName() == potentialRecipe.getRecipeName()) {
                alreadyOwned = true;
                break;
            }
        }

        if (!alreadyOwned) {
            isValidIndex = true;
            Player::addRecipeToInventory(potentialRecipe); 
            AXLOGD("Added recipe with index {} to Player's inventory.", i);
            
            for(int j = 0; j < Player::getPlayerRecipes().size(); j++){
                AXLOGD("Current Player Recipes: {}", j+1);
            }

            /*CookingScene* cookingSceneInstance = CookingScene::currentInstance;
            cookingSceneInstance->updateRecipeBookList();*/
        }

        
    }

    return i;
}

BubbleGame::BubbleGame() : score(0), timeLeft(30), scoreLabel(nullptr), timerLabel(nullptr), root(nullptr), comp(nullptr) {
}
BubbleGame::~BubbleGame() { }