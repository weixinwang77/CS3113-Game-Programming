#include "LoseScreen.h"

LoseScreen::LoseScreen() : Scene{{0.0f}, nullptr} { mIsGameplay = false; }
LoseScreen::LoseScreen(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode}
{ mIsGameplay = false; }
LoseScreen::~LoseScreen() {}

void LoseScreen::initialise()
{
    mGameState.nextSceneID = -1;
    mGameState.player = nullptr;
    mGameState.enemies = nullptr;
    mGameState.enemyCount = 0;
    mGameState.map = nullptr;
}

void LoseScreen::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
        mGameState.nextSceneID = 0; // Back to menu
}

void LoseScreen::render()
{
    ClearBackground(ColorFromHex("#3d0f0f"));

    const char *text = "YOU LOSE";
    int w = MeasureText(text, 80);
    DrawText(text, (int)mOrigin.x - w / 2, (int)mOrigin.y - 80, 80, RED);

    const char *sub = "Press ENTER to return to Menu";
    int sw = MeasureText(sub, 24);
    DrawText(sub, (int)mOrigin.x - sw / 2, (int)mOrigin.y + 40, 24, LIGHTGRAY);
}

void LoseScreen::shutdown() {}
