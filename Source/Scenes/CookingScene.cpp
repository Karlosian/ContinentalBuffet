#include "CookingScene.h"
#include "GRoot.h"
#include "UIPackage.h"
#include "GComponent.h"
#include "GButton.h"

using namespace ax;

static int s_sceneID = 2000;

void CookingScene::loadStartScreen() {
    fairygui::UIPackage::addPackage("UI/Package2");
    fairygui::GComponent* cookingSceneComponent = fairygui::UIPackage::createObject("Package2", "Component1")->as<fairygui::GComponent>();
    cookingSceneComponent->setPosition(0, 0);
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

void CookingScene::update(float delta) {}

void CookingScene::menuCloseCallback(ax::Object* sender)
{
    _director->end();
}

CookingScene::CookingScene()
{
    _sceneID = s_sceneID++;
}

CookingScene::~CookingScene()
{
    _sceneID = -1;
}
