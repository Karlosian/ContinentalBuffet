#pragma once

#include "axmol.h"
#include "FairyGUI.h"
#include "GComponent.h"
#include "GRoot.h"

class BubbleGame : public ax::Scene
{
    private:
    void createUI();
    void startCountdown();
    void startGame();
    void updateScoreLabel();
    void endGame();
    void spawnBubble();
    static int parsePlayerReward(int score);
    fairygui::GRoot* root;
    int score = 0;
    int timeLeft = 30;
    ax::Label* scoreLabel;
    ax::Label* timerLabel;
    //load at scene creation to avoid lagspikes
    fairygui::GComponent* comp;
public:
    static BubbleGame* createScene();
    BubbleGame();              
    virtual ~BubbleGame(); 
    virtual bool init() override;

    // implement the "static create()" method manually
    CREATE_FUNC(BubbleGame);
};
