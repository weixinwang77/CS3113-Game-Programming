/**
* Author: Weixin Wang
* Assignment: Lunar Lander
* Date due: 03/14/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Entity.h"

constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS = 120;
constexpr char BG_COLOUR[] = "#0B0E17";
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f,
                ACCELERATION_OF_GRAVITY = 60.0f,
                THRUST_VERTICAL = 140.0f,
                THRUST_HORIZONTAL = 80.0f,
                HORIZONTAL_DRAG = 1.5f,
                MAX_SAFE_LANDING_SPEED = 80.0f;

constexpr float INITIAL_FUEL = 100.0f,
                FUEL_CONSUMPTION_RATE = 15.0f;

constexpr int   NUMBER_OF_GROUND_TILES = 8,
                NUMBER_OF_OBSTACLES = 2,
                LANDING_PAD_INDEX = NUMBER_OF_GROUND_TILES + NUMBER_OF_OBSTACLES,
                MOVING_PLATFORM_INDEX = LANDING_PAD_INDEX + 1,
                TOTAL_PLATFORMS = MOVING_PLATFORM_INDEX + 1;

constexpr float GROUND_Y = 570.0f,
                TILE_WIDTH = 130.0f,
                TILE_HEIGHT = 40.0f,
                MOVING_SPEED = 50.0f;

constexpr int   SHEET_COLS = 4,
                SHEET_ROWS = 2,
                CELL_W = 48,
                CELL_H = 64;

enum GameState { PLAYING, WON, LOST };

AppStatus  gAppStatus = RUNNING;
GameState  gGameState = PLAYING;

float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f,
      gFuel = INITIAL_FUEL;

float gMovingDirection = 1.0f;

bool gThrustUp = false,
     gThrustLeft = false,
     gThrustRight = false;

Entity *gShip = nullptr;
Entity *gPlatforms = nullptr;

void initialise();
void processInput();
void update();
void render();
void shutdown();

Texture2D generateShipSpriteSheet();
Texture2D generateTileTexture(int width, int height, Color colour);
void checkGameOutcome(float preLandingSpeed);
void drawHUD();

Texture2D generateShipSpriteSheet(){
    int totalW = CELL_W * SHEET_COLS;
    int totalH = CELL_H * SHEET_ROWS;
    Image sheet = GenImageColor(totalW, totalH, BLANK);

    for (int frame = 0; frame < SHEET_COLS * SHEET_ROWS; frame++){
        int col = frame % SHEET_COLS;
        int row = frame / SHEET_COLS;
        int ox  = col * CELL_W;
        int oy  = row * CELL_H;

        ImageDrawRectangle(&sheet, ox + 18, oy + 10, 12, 30, LIGHTGRAY);
        ImageDrawRectangle(&sheet, ox + 20, oy + 4,  8,  10, WHITE);
        ImageDrawRectangle(&sheet, ox + 21, oy + 14, 6,  5,  SKYBLUE);
        ImageDrawRectangle(&sheet, ox + 14, oy + 38, 4,  8,  GRAY);
        ImageDrawRectangle(&sheet, ox + 30, oy + 38, 4,  8,  GRAY);
        ImageDrawRectangle(&sheet, ox + 10, oy + 28, 10, 6,  GRAY);
        ImageDrawRectangle(&sheet, ox + 28, oy + 28, 10, 6,  GRAY);

        if (row == 1){
            int flameH = 6 + col * 4;
            int flameY = oy + 40;
            Color outerColour = (col % 2 == 0) ? ORANGE : RED;
            ImageDrawRectangle(&sheet, ox + 19, flameY, 10, flameH, outerColour);
            int innerH = flameH - 3;
            if (innerH > 0)
                ImageDrawRectangle(&sheet, ox + 21, flameY, 6, innerH, YELLOW);
        }
    }
    Texture2D texture = LoadTextureFromImage(sheet);
    UnloadImage(sheet);
    return texture;
}

Texture2D generateTileTexture(int width, int height, Color colour){
    Image img = GenImageColor(width, height, colour);
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    return tex;
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lunar Lander");
    Texture2D shipTexture = generateShipSpriteSheet();
    std::map<Direction, std::vector<int>> shipAnimationAtlas = {{DOWN, { 0 }}, {LEFT,  { 0 }}, {RIGHT, { 0 }}, {UP, { 4, 5, 6, 7 }},};

    gShip = new Entity({SCREEN_WIDTH / 2, 80.0f}, {60.0f, 80.0f}, shipTexture, ATLAS,{SHEET_ROWS, SHEET_COLS}, shipAnimationAtlas);
    gShip->setColliderDimensions({ 0.0f, 50.0f});
    gShip->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    gShip->setFrameSpeed(10);

    gPlatforms = new Entity[TOTAL_PLATFORMS];

    for (int i = 0; i < NUMBER_OF_GROUND_TILES; i++){
        float x = (TILE_WIDTH / 2.0f) + i * (SCREEN_WIDTH / (float) NUMBER_OF_GROUND_TILES);
        gPlatforms[i].setPosition({ x, GROUND_Y });
        gPlatforms[i].setScale({ TILE_WIDTH, TILE_HEIGHT });
        gPlatforms[i].setColliderDimensions({ TILE_WIDTH, TILE_HEIGHT });
        gPlatforms[i].setTexture(generateTileTexture((int) TILE_WIDTH, (int) TILE_HEIGHT, ColorFromHex("#4A4A4A")));
        gPlatforms[i].setTextureType(SINGLE);
    }

    gPlatforms[NUMBER_OF_GROUND_TILES].setPosition({200.0f, 380.0f});
    gPlatforms[NUMBER_OF_GROUND_TILES].setScale({100.0f, 25.0f});
    gPlatforms[NUMBER_OF_GROUND_TILES].setColliderDimensions({100.0f, 25.0f});
    gPlatforms[NUMBER_OF_GROUND_TILES].setTexture(generateTileTexture(100, 25, ColorFromHex("#5C3A2E")));
    gPlatforms[NUMBER_OF_GROUND_TILES].setTextureType(SINGLE);

    gPlatforms[NUMBER_OF_GROUND_TILES + 1].setPosition({780.0f, 300.0f});
    gPlatforms[NUMBER_OF_GROUND_TILES + 1].setScale({90.0f, 25.0f});
    gPlatforms[NUMBER_OF_GROUND_TILES + 1].setColliderDimensions({ 90.0f, 25.0f });
    gPlatforms[NUMBER_OF_GROUND_TILES + 1].setTexture(generateTileTexture(90, 25, ColorFromHex("#5C3A2E")));
    gPlatforms[NUMBER_OF_GROUND_TILES + 1].setTextureType(SINGLE);

    gPlatforms[LANDING_PAD_INDEX].setPosition({500.0f, 540.0f});
    gPlatforms[LANDING_PAD_INDEX].setScale({110.0f, 20.0f});
    gPlatforms[LANDING_PAD_INDEX].setColliderDimensions({110.0f, 20.0f});
    gPlatforms[LANDING_PAD_INDEX].setTexture(generateTileTexture(110, 20, ColorFromHex("#2ECC71")));
    gPlatforms[LANDING_PAD_INDEX].setTextureType(SINGLE);

    gPlatforms[MOVING_PLATFORM_INDEX].setPosition({350.0f, 430.0f});
    gPlatforms[MOVING_PLATFORM_INDEX].setScale({100.0f, 20.0f});
    gPlatforms[MOVING_PLATFORM_INDEX].setColliderDimensions({100.0f, 20.0f});
    gPlatforms[MOVING_PLATFORM_INDEX].setTexture(generateTileTexture(100, 20, ColorFromHex("#F39C12")));
    gPlatforms[MOVING_PLATFORM_INDEX].setTextureType(SINGLE);

    SetTargetFPS(FPS);
}

void processInput()
{
    gThrustUp = false;
    gThrustLeft = false;
    gThrustRight = false;

    if (gGameState != PLAYING){
        if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
        return;
    }

    if (gFuel > 0.0f){
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    gThrustUp    = true;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))   gThrustLeft  = true;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))  gThrustRight = true;
    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void checkGameOutcome(float preLandingSpeed)
{
    constexpr float TOLERANCE = 2.0f;

    for (int i = 0; i < TOTAL_PLATFORMS; i++)
    {
        float xDist = fabs(gShip->getPosition().x - gPlatforms[i].getPosition().x) - ((gShip->getColliderDimensions().x + gPlatforms[i].getColliderDimensions().x) / 2.0f);
        float yDist = fabs(gShip->getPosition().y - gPlatforms[i].getPosition().y) - ((gShip->getColliderDimensions().y + gPlatforms[i].getColliderDimensions().y) / 2.0f);

        if (xDist < TOLERANCE && yDist < TOLERANCE)
        {
            bool isLandingPad = (i == LANDING_PAD_INDEX || i == MOVING_PLATFORM_INDEX);
            if (isLandingPad && fabs(preLandingSpeed) <= MAX_SAFE_LANDING_SPEED)
            {
                gGameState = WON;
            }
            else
            {
                gGameState = LOST;
            }
            return;
        }
    }
}

void update()
{
    if (gGameState != PLAYING) return;

    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        float accX = 0.0f;
        float accY = ACCELERATION_OF_GRAVITY;

        bool isThrusting = false;

        if (gThrustUp) {accY -= THRUST_VERTICAL; isThrusting = true; }
        if (gThrustLeft)  { accX -= THRUST_HORIZONTAL; isThrusting = true; }
        if (gThrustRight) { accX += THRUST_HORIZONTAL; isThrusting = true; }

        accX += -gShip->getVelocity().x * HORIZONTAL_DRAG;

        gShip->setAcceleration({ accX, accY });

        if (isThrusting)
        {
            gFuel -= FUEL_CONSUMPTION_RATE * FIXED_TIMESTEP;
            if (gFuel < 0.0f) gFuel = 0.0f;
        }

        gShip->setDirection(isThrusting ? UP : DOWN);

        float preLandingSpeed = gShip->getVelocity().y;

        gShip->update(FIXED_TIMESTEP, gPlatforms, TOTAL_PLATFORMS);

        Vector2 movPos = gPlatforms[MOVING_PLATFORM_INDEX].getPosition();
        movPos.x += MOVING_SPEED * gMovingDirection * FIXED_TIMESTEP;

        if (movPos.x > SCREEN_WIDTH - 60.0f || movPos.x < 60.0f)
            gMovingDirection *= -1.0f;

        gPlatforms[MOVING_PLATFORM_INDEX].setPosition(movPos);

        if (gShip->isCollidingBottom() || gShip->isCollidingTop())
        {
            checkGameOutcome(preLandingSpeed);
        }

        Vector2 shipPos = gShip->getPosition();

        if (shipPos.y > SCREEN_HEIGHT + 50.0f)
        {
            gGameState = LOST;
        }

        if (shipPos.x < -20.0f)
            gShip->setPosition({ SCREEN_WIDTH + 20.0f, shipPos.y });
        else if (shipPos.x > SCREEN_WIDTH + 20.0f)
            gShip->setPosition({ -20.0f, shipPos.y });

        deltaTime -= FIXED_TIMESTEP;
    }

    gTimeAccumulator = deltaTime;
}

void drawHUD()
{
    int barX = 20, barY = 20, barW = 200, barH = 20;
    float fuelRatio = gFuel / INITIAL_FUEL;

    DrawRectangle(barX, barY, barW, barH, ColorFromHex("#333333"));

    Color fuelColour = (fuelRatio > 0.3f) ? GREEN : RED;
    DrawRectangle(barX, barY, (int)(barW * fuelRatio), barH, fuelColour);

    DrawRectangleLines(barX, barY, barW, barH, WHITE);
    DrawText("FUEL", barX + barW + 10, barY + 2, 18, WHITE);

    Vector2 vel = gShip->getVelocity();
    char velText[64];
    sprintf(velText, "VEL  X:%.0f  Y:%.0f", vel.x, vel.y);
    DrawText(velText, 20, 50, 16, LIGHTGRAY);

    if (gGameState == WON){
        const char *msg = "MISSION ACCOMPLISHED";
        int textW = MeasureText(msg, 40);
        DrawText(msg, (SCREEN_WIDTH - textW) / 2, SCREEN_HEIGHT / 2 - 40, 40, GREEN);

        const char *sub = "Press Q to quit";
        int subW = MeasureText(sub, 20);
        DrawText(sub, (SCREEN_WIDTH - subW) / 2, SCREEN_HEIGHT / 2 + 10, 20, WHITE);
    }
    else if (gGameState == LOST){
        const char *msg = "MISSION FAILED";
        int textW = MeasureText(msg, 40);
        DrawText(msg, (SCREEN_WIDTH - textW) / 2, SCREEN_HEIGHT / 2 - 40, 40, RED);

        const char *sub = "Press Q to quit";
        int subW = MeasureText(sub, 20);
        DrawText(sub, (SCREEN_WIDTH - subW) / 2, SCREEN_HEIGHT / 2 + 10, 20, WHITE);
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    for (int i = 0; i < 80; i++)
    {
        int sx = (i * 127 + 53) % SCREEN_WIDTH;
        int sy = (i * 311 + 17) % SCREEN_HEIGHT;
        int brightness = 100 + (i * 73) % 155;
        DrawPixel(sx, sy, (Color){ (unsigned char)brightness, (unsigned char)brightness, (unsigned char)brightness, 255 });
    }

    Vector2 padPos = gPlatforms[LANDING_PAD_INDEX].getPosition();
    DrawText("v", (int)padPos.x - 4, (int)padPos.y - 40, 20, GREEN);

    Vector2 movPos = gPlatforms[MOVING_PLATFORM_INDEX].getPosition();
    DrawText("v", (int)movPos.x - 4, (int)movPos.y - 40, 20, ORANGE);

    for (int i = 0; i < TOTAL_PLATFORMS; i++)
        gPlatforms[i].render();
    gShip->render();
    drawHUD();
    EndDrawing();
}

void shutdown()
{
    delete gShip;
    delete[] gPlatforms;
    CloseWindow();
}

int main(void)
{
    initialise();
    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }
    shutdown();
    return 0;
}