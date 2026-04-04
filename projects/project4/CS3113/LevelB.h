#ifndef LEVELB_H
#define LEVELB_H

#include "Scene.h"

class LevelB : public Scene
{
private:
    static constexpr int LEVEL_WIDTH  = 35;
    static constexpr int LEVEL_HEIGHT = 10;

    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,1,
        1,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,0,0,0,0,2,2,0,0,0,0,0,0,0,2,2,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,2,2,2,2,2,2,2,0,0,2,2,2,2,0,0,2,2,2,0,0,2,2,2,2,0,0,2,2,2,0,0,2,2,1,
        1,3,3,3,3,3,3,3,0,0,3,3,3,3,0,0,3,3,3,0,0,3,3,3,3,0,0,3,3,3,0,0,3,3,1,
        1,3,3,3,3,3,3,3,0,0,3,3,3,3,0,0,3,3,3,0,0,3,3,3,3,0,0,3,3,3,0,0,3,3,1
    };

public:
    static constexpr float TILE_SIZE = 50.0f;
    static constexpr float GRAVITY   = 981.0f;

    LevelB();
    LevelB(Vector2 origin, const char *bgHexCode);
    ~LevelB();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
