#include "LevelB.h"

LevelB::LevelB() : Scene{{0.0f}, nullptr} {}
LevelB::LevelB(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}
LevelB::~LevelB() { shutdown(); }

void LevelB::initialise()
{
    mGameState.nextSceneID = -1;

    mGameState.map = new Map(
        LEVEL_WIDTH, LEVEL_HEIGHT,
        (unsigned int *)mLevelData,
        "assets/game/tileset.png",
        TILE_SIZE, 4, 1, mOrigin
    );

    // Player
    std::map<Direction, std::vector<int>> knightAnim = {
        {RIGHT, {0,1,2,3,4,5,6,7}}, {LEFT, {0,1,2,3,4,5,6,7}},
    };

    mGameState.player = new Entity(
        {mOrigin.x - 700.0f, mOrigin.y - 150.0f},
        {86.0f, 75.0f},
        "assets/game/knight_run.png",
        ATLAS, {1, 8}, knightAnim, PLAYER
    );
    mGameState.player->setJumpingPower(500.0f);
    mGameState.player->setColliderDimensions({30.0f, 50.0f});
    mGameState.player->setAcceleration({0.0f, GRAVITY});

    //ENEMIES 
    mGameState.enemyCount = 3;
    mGameState.enemies = new Entity[mGameState.enemyCount];

    float mapLeft = mOrigin.x - (LEVEL_WIDTH * TILE_SIZE) / 2.0f;

    //Slime Wanderers
    std::map<Direction, std::vector<int>> slimeAnim = {
        {RIGHT, {0,1,2,3}}, {LEFT, {0,1,2,3}},
    };

    for (int i = 0; i < 2; i++)
    {
        mGameState.enemies[i].setTexture("assets/game/slime.png");
        mGameState.enemies[i].setTextureType(ATLAS);
        mGameState.enemies[i].setSpriteSheetDimensions({3, 8});
        mGameState.enemies[i].setAnimationAtlas(slimeAnim);
        mGameState.enemies[i].setEntityType(NPC);
        mGameState.enemies[i].setScale({50.0f, 40.0f});
        mGameState.enemies[i].setColliderDimensions({40.0f, 30.0f});
        mGameState.enemies[i].setAcceleration({0.0f, GRAVITY});
        mGameState.enemies[i].setAIType(WANDERER);
        mGameState.enemies[i].setAIState(WALKING);
        mGameState.enemies[i].setSpeed(70);
        mGameState.enemies[i].setFrameSpeed(10);
        mGameState.enemies[i].setDirection(LEFT);
    }

    mGameState.enemies[0].setPosition({mapLeft + 4 * TILE_SIZE, mOrigin.y + TILE_SIZE * 2});
    mGameState.enemies[0].setPatrolBounds(mapLeft + 2 * TILE_SIZE, mapLeft + 7 * TILE_SIZE);

    mGameState.enemies[1].setPosition({mapLeft + 22 * TILE_SIZE, mOrigin.y + TILE_SIZE * 2});
    mGameState.enemies[1].setPatrolBounds(mapLeft + 22 * TILE_SIZE, mapLeft + 25 * TILE_SIZE);

    //Skeleton Follower (2) 
    std::map<Direction, std::vector<int>> skelAnim = {
        {RIGHT, {0,1,2,3,4,5,6,7,8,9}}, {LEFT, {0,1,2,3,4,5,6,7,8,9}},
    };

    mGameState.enemies[2].setTexture("assets/game/skeleton_walk.png");
    mGameState.enemies[2].setTextureType(ATLAS);
    mGameState.enemies[2].setSpriteSheetDimensions({1, 10});
    mGameState.enemies[2].setAnimationAtlas(skelAnim);
    mGameState.enemies[2].setEntityType(NPC);
    mGameState.enemies[2].setScale({70.0f, 47.0f});
    mGameState.enemies[2].setColliderDimensions({40.0f, 45.0f});
    mGameState.enemies[2].setAcceleration({0.0f, GRAVITY});
    mGameState.enemies[2].setAIType(FOLLOWER);
    mGameState.enemies[2].setAIState(IDLE);
    mGameState.enemies[2].setSpeed(90);
    mGameState.enemies[2].setFrameSpeed(10);
    mGameState.enemies[2].setDirection(LEFT);
    mGameState.enemies[2].setPosition({mapLeft + 16 * TILE_SIZE, mOrigin.y + TILE_SIZE * 2});

    //EXIT DOOR 
    float mapRight = mOrigin.x + (LEVEL_WIDTH * TILE_SIZE) / 2.0f;
    mGameState.doorTexture = LoadTexture("assets/game/door.png");
    mGameState.exitPosition = {mapRight - 2.5f * TILE_SIZE, mOrigin.y + TILE_SIZE * 0.5f};
    mGameState.hasDoor = true;
}

void LevelB::update(float deltaTime)
{
    mGameState.player->update(deltaTime, nullptr, mGameState.map, nullptr, 0);

    for (int i = 0; i < mGameState.enemyCount; i++)
        mGameState.enemies[i].update(deltaTime, mGameState.player, mGameState.map, nullptr, 0);

    if (mGameState.player->checkEnemyCollision(mGameState.enemies, mGameState.enemyCount))
        mGameState.nextSceneID = -2;

    float mapBottom = mOrigin.y + (LEVEL_HEIGHT * TILE_SIZE) / 2.0f;
    if (mGameState.player->getPosition().y > mapBottom + 100.0f)
        mGameState.nextSceneID = -2;

    float mapRight = mOrigin.x + (LEVEL_WIDTH * TILE_SIZE) / 2.0f;
    if (mGameState.player->getPosition().x > mapRight - 2 * TILE_SIZE)
        mGameState.nextSceneID = 3;
}

void LevelB::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    mGameState.map->render();

    if (mGameState.hasDoor)
    {
        Rectangle src = {0, 0, (float)mGameState.doorTexture.width, (float)mGameState.doorTexture.height};
        Rectangle dst = {mGameState.exitPosition.x, mGameState.exitPosition.y, 50.0f, 150.0f};
        DrawTexturePro(mGameState.doorTexture, src, dst, {25.0f, 75.0f}, 0.0f, WHITE);
        DrawText("EXIT", (int)mGameState.exitPosition.x - 20, (int)mGameState.exitPosition.y - 100, 20, GOLD);
    }

    for (int i = 0; i < mGameState.enemyCount; i++)
        mGameState.enemies[i].render();
    mGameState.player->render();
}

void LevelB::shutdown()
{
    delete mGameState.player;    mGameState.player = nullptr;
    delete[] mGameState.enemies; mGameState.enemies = nullptr;
    delete mGameState.map;       mGameState.map = nullptr;
    if (mGameState.hasDoor) { UnloadTexture(mGameState.doorTexture); mGameState.hasDoor = false; }
}
