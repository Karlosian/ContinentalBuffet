#include "Shop.h"
#include "CookingScene.h"

#include "GRoot.h"
#include "UIPackage.h"
#include "GComponent.h"
#include "GButton.h"

#include <iostream>

using namespace ax;

static int s_sceneID = 2000;

void Shop::loadStartScreen() {
    fairygui::UIPackage::addPackage("UI/Shop");
    fairygui::GComponent* homepageComponent = fairygui::UIPackage::createObject("Package2", "Component1")->as<fairygui::GComponent>();
    homepageComponent->setPosition(0, 0);
    root->addChild(homepageComponent);
}

bool Shop::init() {
    if (!Scene::init())
        return false;

    auto visibleSize = _director->getVisibleSize();
    auto origin      = _director->getVisibleOrigin();
    auto safeArea    = _director->getSafeAreaRect();
    auto safeOrigin  = safeArea.origin;

    root = fairygui::GRoot::create(this);
    root->retain();
    loadStartScreen();

    scheduleUpdate();
    return true;
}

void Shop::update(float delta) {}

void Shop::menuCloseCallback(ax::Object* sender)
{
    _director->end();
}

Shop::Shop()
{
    _sceneID = s_sceneID++;
}

Shop::~Shop()
{
    _sceneID = -1;
}
