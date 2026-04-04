#include "WinScreen.h"

WinScreen::WinScreen() : Scene{{0.0f}, nullptr} { mIsGameplay = false; }
WinScreen::WinScreen(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode}
{ mIsGameplay = false; }
WinScreen::~WinScreen() {}

void WinScreen::initialise()
{
    mGameState.nextSceneID = -1;
    mGameState.player = nullptr;
    mGameState.enemies = nullptr;
    mGameState.enemyCount = 0;
    mGameState.map = nullptr;
}

void WinScreen::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
        mGameState.nextSceneID = 0; // Back to menu
}

void WinScreen::render()
{
    ClearBackground(ColorFromHex("#0f3d0f"));

    const char *text = "YOU WIN!";
    int w = MeasureText(text, 80);
    DrawText(text, (int)mOrigin.x - w / 2, (int)mOrigin.y - 80, 80, GOLD);

    const char *sub = "Press ENTER to return to Menu";
    int sw = MeasureText(sub, 24);
    DrawText(sub, (int)mOrigin.x - sw / 2, (int)mOrigin.y + 40, 24, LIGHTGRAY);
}

void WinScreen::shutdown() {}
