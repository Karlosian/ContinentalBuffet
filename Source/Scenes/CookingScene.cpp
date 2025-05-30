#include "CookingScene.h"
#include "GRoot.h"
#include "UIPackage.h"
#include "GComponent.h"
#include "GButton.h"
#include "GList.h"

#include <iostream>

using namespace ax;

static int s_sceneID = 2000;

void CookingScene::loadStartScreen() {
    fairygui::UIPackage::addPackage("UI/CookingPackage");
    fairygui::GComponent* cookingSceneComponent = fairygui::UIPackage::createObject("Package1", "Component1")->as<fairygui::GComponent>();
    cookingSceneComponent->setPosition(0, 0);
    root->addChild(cookingSceneComponent);

    fairygui::GObject* processListComponent = cookingSceneComponent->getChild("n2");

    if (processListComponent == nullptr || processListComponent->as<fairygui::GComponent>() == nullptr)
    {
        std::cerr << "Error: processListComponent is null." << std::endl;
        return;
    }

    fairygui::GObject* processListObject = processListComponent->as<fairygui::GComponent>()->getChild("n13");

    if (processListObject == nullptr || processListObject->as<fairygui::GList>() == nullptr) {
        std::cerr << "Error: processListObject is null." << std::endl;
        return;
    }

    fairygui::GList* processList = processListObject->as<fairygui::GList>();

    processList->setVirtual();

    processList->getScrollPane()->scrollRight(50, true);
    
    fairygui::GObject* leftArrowObject = cookingSceneComponent->getChild("n4");

    if (leftArrowObject != nullptr && leftArrowObject->as<fairygui::GButton>() != nullptr)
    {
        fairygui::GButton* leftArrow = leftArrowObject->as<fairygui::GButton>();
        leftArrow->addClickListener([](fairygui::EventContext* context) {

        });
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
