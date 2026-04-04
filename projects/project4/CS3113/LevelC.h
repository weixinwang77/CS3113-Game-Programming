#ifndef LEVELC_H
#define LEVELC_H

#include "Scene.h"

class LevelC : public Scene
{
private:
    static constexpr int LEVEL_WIDTH  = 40;
    static constexpr int LEVEL_HEIGHT = 10;

    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,1,
        1,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,0,0,0,2,2,0,0,0,0,0,2,2,0,0,0,0,0,0,0,2,2,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,2,2,2,2,2,0,0,2,2,2,0,0,2,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,2,0,0,2,2,2,2,1,
        1,3,3,3,3,3,0,0,3,3,3,0,0,3,3,3,0,0,3,3,0,0,3,3,0,0,3,3,0,0,3,3,3,0,0,3,3,3,3,1,
        1,3,3,3,3,3,0,0,3,3,3,0,0,3,3,3,0,0,3,3,0,0,3,3,0,0,3,3,0,0,3,3,3,0,0,3,3,3,3,1
    };

public:
    static constexpr float TILE_SIZE = 50.0f;
    static constexpr float GRAVITY   = 981.0f;

    LevelC();
    LevelC(Vector2 origin, const char *bgHexCode);
    ~LevelC();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
