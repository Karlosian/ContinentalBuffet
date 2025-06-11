#pragma once

#include "axmol.h"

class BubbleGame : public ax::Scene
{
    private:
    void createUI();
    void startCountdown();
    void startGame();
    void updateScoreLabel();
    void endGame();
    void spawnBubble();
    int score = 0;
    int timeLeft = 30;
    ax::Label* scoreLabel = nullptr;
    ax::Label* timerLabel = nullptr;
public:
    static BubbleGame* createScene();
    BubbleGame();              
    virtual ~BubbleGame(); 
    virtual bool init() override;

    // implement the "static create()" method manually
    CREATE_FUNC(BubbleGame);
};
