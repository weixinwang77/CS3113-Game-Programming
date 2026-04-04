#include "Menu.h"

Menu::Menu() : Scene{{0.0f}, nullptr} { mIsGameplay = false; }
Menu::Menu(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode}
{
    mIsGameplay = false;
}
Menu::~Menu() { }

void Menu::initialise()
{
    mGameState.nextSceneID = -1;
    mGameState.player = nullptr;
    mGameState.enemies = nullptr;
    mGameState.enemyCount = 0;
    mGameState.map = nullptr;
}

void Menu::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
        mGameState.nextSceneID = 1; // Go to Level A
}

void Menu::render()
{
    ClearBackground(ColorFromHex("#1a1a2e"));

    const char *title = "KNIGHT'S ADVENTURE";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, (int)mOrigin.x - titleWidth / 2, (int)mOrigin.y - 100, 60, GOLD);

    const char *subtitle = "Press ENTER to Start";
    int subWidth = MeasureText(subtitle, 24);
    DrawText(subtitle, (int)mOrigin.x - subWidth / 2, (int)mOrigin.y + 20, 24, LIGHTGRAY);

    const char *controls = "A/D = Move | W = Jump | Q = Quit";
    int ctrlWidth = MeasureText(controls, 18);
    DrawText(controls, (int)mOrigin.x - ctrlWidth / 2, (int)mOrigin.y + 80, 18, GRAY);
}

void Menu::shutdown() { }
