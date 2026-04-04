#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"

constexpr int MAX_ENEMIES = 10;

struct GameState
{
    Entity *player;
    Entity *enemies;
    int enemyCount;
    Map *map;
    int nextSceneID;

    Texture2D doorTexture;
    Vector2 exitPosition;
    bool hasDoor;
};

class Scene
{
protected:
    GameState mGameState;
    Vector2 mOrigin;
    const char *mBGColourHexCode = "#000000";
    bool mIsGameplay = true;  // false for menu/win/lose screens

public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    GameState   getState()           const { return mGameState;      }
    Vector2     getOrigin()          const { return mOrigin;         }
    const char* getBGColourHexCode() const { return mBGColourHexCode;}
    bool        isGameplay()         const { return mIsGameplay;     }
};

#endif
