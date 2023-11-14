#pragma once


#include <vector>
#include "cocos2d.h"

using namespace cocos2d;
enum class GameState
{
    Playing,
    Building
} ;
class Game : public Scene
{
private:
    GameState gameState;
    TMXTiledMap* tileMap;
    Sprite* currentBuidingSprite;
    
    void onTouchesMoved(const std::vector<Touch*>& touches, Event  *event);
    void onMouseDown(Event *event);

    void onMouseMove(Event *event);
    void onMouseScroll(Event *event);
    Vec2 cartisanToIsometric(const Vec2& wPos) const;
    Vec2 screenToLayerSpace(const Vec2& pos, TMXLayer* layer) const;
    void onBuildEvent(Ref *event);
public:
    static Scene* createScene();
    bool init() override;
    CREATE_FUNC(Game);
};

