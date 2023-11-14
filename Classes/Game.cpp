#include "Game.h"
#include "ui/UIButton.h"

Scene* Game::createScene()
{
    return create();
}

bool Game::init()
{
    if ( !Scene::init() )
    {
        return false;
    }


    ui::Button * button = ui::Button::create("tree.png");
    button->addClickEventListener(CC_CALLBACK_1(Game::onBuildEvent, this));
    button->setAnchorPoint(Vec2(0.5, 0));
    button->setPosition(Vec2(50, 50));
    addChild(button);

    tileMap = TMXTiledMap::create("iso-test-zorder.tmx");
    addChild(tileMap, 0);
    
    Director::getInstance()->setDepthTest(true);

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesMoved = CC_CALLBACK_2(Game::onTouchesMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Game::onMouseDown, this);
    mouseListener->onMouseMove = CC_CALLBACK_1(Game::onMouseMove, this);
    mouseListener->onMouseScroll = CC_CALLBACK_1(Game::onMouseScroll, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    scheduleUpdate();
    
    currentBuidingSprite = Sprite::create("tree.png");
    currentBuidingSprite->setAnchorPoint(Vec2(0.5, 0));
    addChild(currentBuidingSprite);
    currentBuidingSprite->setVisible(false);

    return true;
}

void Game::onBuildEvent(Ref* sender)
{
    gameState = GameState::Building;
    currentBuidingSprite->setTexture("tree.png");
    currentBuidingSprite->setVisible(true);
}

void Game::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
    if(gameState == GameState::Playing)
    {
        auto touch = touches[0];

        auto diff = touch->getDelta();
        auto currentPos = tileMap->getPosition();
        tileMap->setPosition(currentPos + diff);
    }
}

void Game::onMouseDown(cocos2d::Event *event) {
    auto* e = (EventMouse*)event;
    if(gameState ==GameState::Building)
    {
        currentBuidingSprite->setVisible(false);
        gameState = GameState::Playing;
        auto layer = tileMap->getLayer("trees3");
        Vec2 tilePos = cartisanToIsometric(Vec2(e->getCursorX()-tileMap->getPositionX(),
            e->getCursorY()-tileMap->getPositionY()));
        Size layerSize = layer->getLayerSize();
        if(tilePos.x >= 0 && tilePos.x < layerSize.width && tilePos.y>=0 && tilePos.y < layerSize.height)
        {
            layer->setTileGID(4, tilePos);
        }
    }
}


void Game::onMouseMove(Event *event)
{
    EventMouse* e = (EventMouse*)event;
    if(gameState == GameState::Building)
    {
        auto* layer = tileMap->getLayer("trees3");
        const Vec2& layerPos = screenToLayerSpace(Vec2(e->getCursorX()-tileMap->getPositionX(),
            e->getCursorY()-tileMap->getPositionY()), layer);
        currentBuidingSprite->setPosition(layerPos + tileMap->getPosition()); 
    }
}

void Game::onMouseScroll(Event *event)
{
    EventMouse* e = (EventMouse*)event;
    float scale = tileMap->getScale();
    scale += e->getScrollY() * 0.01;
    scale = clampf(scale, 0.6, 1.5);
    tileMap->setScale(scale);
}

Vec2 Game::cartisanToIsometric(const Vec2 &wPos) const {
    float mw = tileMap->getMapSize().width;
    float mh = tileMap->getMapSize().height;
    float tw = tileMap->getTileSize().width * tileMap->getScaleX();
    float th = tileMap->getTileSize().height * tileMap->getScaleY();
    float isox = floorf(mh - wPos.y / th + wPos.x / tw - mw / 2);
    float isoy = floorf(mh - wPos.y / th - wPos.x / tw + mw / 2 - 1 / 2);
    return Vec2(isox, isoy);
}

Vec2 Game::screenToLayerSpace(const Vec2& pos, TMXLayer *layer) const
{
    Vec2 tileCoord = cartisanToIsometric(pos);
    return layer->getPositionAt(tileCoord)* tileMap->getScale(); 
}


