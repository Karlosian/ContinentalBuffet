#include "BubbleGame.h"
#include "AudioEngine.h"
USING_NS_AX;


BubbleGame* BubbleGame::createScene()
{
    return BubbleGame::create();
}

void BubbleGame::startGame(){
   // Reset score
    score = 0;
    updateScoreLabel();  // You'd write this function to show score

    // Start game timer (e.g., 30 seconds)
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
        endGame();  // Call your endGame function
    }
}, 1.0f, "game_timer");


    // Maybe show UI elements (timer, score, etc.)
    scoreLabel->setVisible(true);
    timerLabel->setVisible(true);    
    return;
}

void BubbleGame::endGame(){
    return;
}

void BubbleGame::spawnBubble(){
    //create the bubble sprite
    auto bubble = ax::Sprite::create("BubbleGame/bubble.png");
    if (!bubble) return; 
    //scale and disable anti-aliasing for pixel art
    bubble->setScale(4.0f);
    bubble->getTexture()->setAliasTexParameters(); 
    
    //random X position along the screen width
    auto visibleSize = ax::Director::getInstance()->getVisibleSize();
    float randomX = AXRANDOM_0_1() * visibleSize.width;

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
    
    this->createUI();
    this->startCountdown();

    return true;
}

void BubbleGame::createUI() {
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
scoreLabel->setPosition(Vec2(100, 680));  // Adjust position as needed
scoreLabel->setColor(ax::Color3B::WHITE);
this->addChild(scoreLabel, 10);

//create timerLabel
timerLabel = ax::Label::createWithTTF("Time: 30", "fonts/Marker Felt.ttf", 24);
timerLabel->setPosition(Vec2(1200, 680));  // Adjust position
timerLabel->setColor(ax::Color3B::WHITE);
this->addChild(timerLabel, 10);

}

BubbleGame::BubbleGame() { }
BubbleGame::~BubbleGame() { }