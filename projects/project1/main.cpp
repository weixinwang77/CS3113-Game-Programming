/**
* Author: Weixin Wang
* Assignment: Simple 2D Scene
* Date due: 02/14/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/cs3113.h"
#include <math.h>

constexpr int SCREEN_WIDTH  = 1600,
              SCREEN_HEIGHT = 900,
              FPS = 60;

constexpr float MARIO_SIZE = 200.0f,
                YOSHI_SIZE = 150.0f,
                MUSHROOM_SIZE = 100.0f;

constexpr float MARIO_ORBIT_X = 200.0f,    
                MARIO_ORBIT_Y = 100.0f,    
                MARIO_ORBIT_SPD = 1.0f;      

constexpr float PULSE_SPEED = 2.0f,
                PULSE_AMP = 20.0f;

constexpr float YOSHI_ORBIT_R = 180.0f,  
                YOSHI_ORBIT_SPD = 2.0f,      
                YOSHI_SPIN_SPD = 90.0f;     

constexpr float MUSHROOM_SPEED = 120.0f,    
                MUSHROOM_WAVE_AMP = 120.0f,    
                MUSHROOM_WAVE_SPD = 2.0f,      
                MUSHROOM_SPIN_SPD = 120.0f;    

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

constexpr char LINK_MARIO_FP[] = "assets/mario.png";
constexpr char LINK_YOSHI_FP[] = "assets/yoshi.png";
constexpr char LINK_MUSHROOM_FP[] = "assets/mushroom.png";

AppStatus gAppStatus = RUNNING;

float gPreviousTicks = 0.0f,
      gMarioOrbitTime  = 0.0f,   
      gMarioPulseTime  = 0.0f;  
Vector2 gMarioPos = ORIGIN;
Vector2 gMarioScale = { MARIO_SIZE, MARIO_SIZE };

float   gYoshiOrbitAngle = 0.0f,  
        gYoshiRotation = 0.0f;  
Vector2 gYoshiPos = { 0.0f, 0.0f };

float   gMushroomTime = 0.0f,
        gMushroomRotation = 0.0f;   
Vector2 gMushroomPos = { 0.0f, SCREEN_HEIGHT / 2 };

Texture2D gMarioTexture;
Texture2D gYoshiTexture;
Texture2D gMushroomTexture;

float gBgTime = 0.0f;

void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 1 - Simple 2D Scene");
    gMarioTexture = LoadTexture(LINK_MARIO_FP);
    gYoshiTexture = LoadTexture(LINK_YOSHI_FP);
    gMushroomTexture = LoadTexture(LINK_MUSHROOM_FP);
    SetTargetFPS(FPS);
}
void processInput()
{
    if (WindowShouldClose()){
        gAppStatus = TERMINATED;
    }
}
void update()
{
    float ticks = (float)GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    gMarioOrbitTime += MARIO_ORBIT_SPD * deltaTime;
    gMarioPos = {ORIGIN.x + MARIO_ORBIT_X * cosf(gMarioOrbitTime), ORIGIN.y + MARIO_ORBIT_Y * sinf(gMarioOrbitTime)};

    gMarioPulseTime += PULSE_SPEED * deltaTime;
    gMarioScale = { MARIO_SIZE + PULSE_AMP * cosf(gMarioPulseTime), MARIO_SIZE + PULSE_AMP * cosf(gMarioPulseTime)};

    gYoshiOrbitAngle += YOSHI_ORBIT_SPD * deltaTime;
    gYoshiPos = {gMarioPos.x + YOSHI_ORBIT_R * cosf(gYoshiOrbitAngle), gMarioPos.y + YOSHI_ORBIT_R * sinf(gYoshiOrbitAngle)};

    gYoshiRotation += YOSHI_SPIN_SPD * deltaTime;
    gMushroomTime += deltaTime;

    gMushroomPos.x += MUSHROOM_SPEED * deltaTime;
    if (gMushroomPos.x > SCREEN_WIDTH + MUSHROOM_SIZE){
        gMushroomPos.x = -MUSHROOM_SIZE;
    }

    gMushroomPos.y = (SCREEN_HEIGHT / 2) + MUSHROOM_WAVE_AMP * sinf(gMushroomTime * MUSHROOM_WAVE_SPD);
    gMushroomRotation += MUSHROOM_SPIN_SPD * deltaTime;

    gBgTime += deltaTime;
}
void render()
{
    BeginDrawing();
    unsigned char bgR = static_cast<unsigned char>(90  + 40 * sinf(gBgTime * 0.5f));
    unsigned char bgG = static_cast<unsigned char>(140 + 40 * sinf(gBgTime * 0.3f));
    unsigned char bgB = static_cast<unsigned char>(200 + 40 * sinf(gBgTime * 0.7f));
    ClearBackground((Color){ bgR, bgG, bgB, 255 });
    //Draw Mario
    {
        Rectangle textureArea = {
            0.0f, 0.0f,
            static_cast<float>(gMarioTexture.width),
            static_cast<float>(gMarioTexture.height)
        };
        Rectangle destinationArea = {gMarioPos.x, gMarioPos.y, gMarioScale.x, gMarioScale.y};
        Vector2 origin = {gMarioScale.x / 2, gMarioScale.y / 2};
        DrawTexturePro(gMarioTexture, textureArea, destinationArea, origin, 0.0f, WHITE);
    }
    //Draw Yoshi
    {
        Rectangle textureArea = {
            0.0f, 0.0f,
            static_cast<float>(gYoshiTexture.width),
            static_cast<float>(gYoshiTexture.height)
        };
        Rectangle destinationArea = {gYoshiPos.x, gYoshiPos.y, YOSHI_SIZE, YOSHI_SIZE};
        Vector2 origin = {YOSHI_SIZE / 2, YOSHI_SIZE / 2};
        DrawTexturePro(gYoshiTexture, textureArea, destinationArea, origin, gYoshiRotation, WHITE);
    }

    //Draw Mushroom
    {
        Rectangle textureArea = {
            0.0f, 0.0f,
            static_cast<float>(gMushroomTexture.width),
            static_cast<float>(gMushroomTexture.height)
        };
        Rectangle destinationArea = {gMushroomPos.x, gMushroomPos.y, MUSHROOM_SIZE, MUSHROOM_SIZE};
        Vector2 origin = {MUSHROOM_SIZE / 2, MUSHROOM_SIZE / 2};
        DrawTexturePro(gMushroomTexture, textureArea, destinationArea,origin, gMushroomRotation, WHITE);
    }

    EndDrawing();
}

void shutdown()
{
    UnloadTexture(gMarioTexture);
    UnloadTexture(gYoshiTexture);
    UnloadTexture(gMushroomTexture);
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