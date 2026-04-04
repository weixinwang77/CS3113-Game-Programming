#include "LevelA.h"

LevelA::LevelA() : Scene{{0.0f}, nullptr} {}
LevelA::LevelA(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}
LevelA::~LevelA() { shutdown(); }

void LevelA::initialise()
{
    mGameState.nextSceneID = -1;

    mGameState.map = new Map(
        LEVEL_WIDTH, LEVEL_HEIGHT,
        (unsigned int *)mLevelData,
        "assets/game/tileset.png",
        TILE_SIZE, 4, 1, mOrigin
    );

    // ----------- PLAYER (use new) -----------
    std::map<Direction, std::vector<int>> knightAnim = {
        {RIGHT, {0,1,2,3,4,5,6,7}},
        {LEFT,  {0,1,2,3,4,5,6,7}},
    };

    mGameState.player = new Entity(
        {mOrigin.x - 600.0f, mOrigin.y - 150.0f},
        {86.0f, 75.0f},
        "assets/game/knight_run.png",
        ATLAS, {1, 8}, knightAnim, PLAYER
    );
    mGameState.player->setJumpingPower(500.0f);
    mGameState.player->setColliderDimensions({30.0f, 50.0f});
    mGameState.player->setAcceleration({0.0f, GRAVITY});

    // ----------- ENEMIES (default ctor + setters to avoid texture unload) -----------
    mGameState.enemyCount = 2;
    mGameState.enemies = new Entity[mGameState.enemyCount];

    std::map<Direction, std::vector<int>> slimeAnim = {
        {RIGHT, {0,1,2,3}}, {LEFT, {0,1,2,3}},
    };

    float mapLeft = mOrigin.x - (LEVEL_WIDTH * TILE_SIZE) / 2.0f;

    for (int i = 0; i < mGameState.enemyCount; i++)
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
        mGameState.enemies[i].setSpeed(60);
        mGameState.enemies[i].setFrameSpeed(10);
        mGameState.enemies[i].setDirection(LEFT);
    }

    mGameState.enemies[0].setPosition({mapLeft + 5 * TILE_SIZE, mOrigin.y + TILE_SIZE * 2});
    mGameState.enemies[0].setPatrolBounds(mapLeft + 2 * TILE_SIZE, mapLeft + 10 * TILE_SIZE);

    mGameState.enemies[1].setPosition({mapLeft + 18 * TILE_SIZE, mOrigin.y + TILE_SIZE * 2});
    mGameState.enemies[1].setPatrolBounds(mapLeft + 15 * TILE_SIZE, mapLeft + 20 * TILE_SIZE);

    // ----------- EXIT DOOR -----------
    float mapRight = mOrigin.x + (LEVEL_WIDTH * TILE_SIZE) / 2.0f;
    mGameState.doorTexture = LoadTexture("assets/game/door.png");
    mGameState.exitPosition = {mapRight - 2.5f * TILE_SIZE, mOrigin.y + TILE_SIZE * 0.5f};
    mGameState.hasDoor = true;
}

void LevelA::update(float deltaTime)
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
        mGameState.nextSceneID = 2;
}

void LevelA::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    mGameState.map->render();

    // Draw exit door
    if (mGameState.hasDoor)
    {
        Rectangle src = {0, 0, (float)mGameState.doorTexture.width, (float)mGameState.doorTexture.height};
        Rectangle dst = {mGameState.exitPosition.x, mGameState.exitPosition.y, 50.0f, 150.0f};
        DrawTexturePro(mGameState.doorTexture, src, dst, {25.0f, 75.0f}, 0.0f, WHITE);

        // Hint text above door
        DrawText("EXIT", (int)mGameState.exitPosition.x - 20, (int)mGameState.exitPosition.y - 100, 20, GOLD);
    }

    for (int i = 0; i < mGameState.enemyCount; i++)
        mGameState.enemies[i].render();
    mGameState.player->render();
}

void LevelA::shutdown()
{
    delete mGameState.player;    mGameState.player = nullptr;
    delete[] mGameState.enemies; mGameState.enemies = nullptr;
    delete mGameState.map;       mGameState.map = nullptr;
    if (mGameState.hasDoor) { UnloadTexture(mGameState.doorTexture); mGameState.hasDoor = false; }
}
