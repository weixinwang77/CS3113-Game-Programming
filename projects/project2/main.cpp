/**
* Author: Weixin Wang
* Assignment: Pong Variations
* Date due: 02/28/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/cs3113.h"
#include <math.h>
#include <stdio.h>

constexpr int SCREEN_WIDTH  = 1280,
              SCREEN_HEIGHT = 720,
              FPS = 60,
              MAX_BALLS = 3,
              WIN_SCORE = 15;

constexpr float RACQUET_SPEED = 300.0f,
                BALL_SPEED = 260.0f,
                RACQUET_W = 100.0f,
                RACQUET_H = 100.0f,
                BALL_SIZE = 60.0f;
constexpr float P1_X = RACQUET_W / 2;
constexpr float P2_X = SCREEN_WIDTH - RACQUET_W / 2;
constexpr char BG_COLOUR[] = "#829498";
constexpr char RACQUET_LEFT_TEX_FP[] = "assets/racquet_left.png";
constexpr char RACQUET_RIGHT_TEX_FP[] = "assets/racquet_right.png";
constexpr char BADMINITON_TEX_FP[] = "assets/badminiton.png";
constexpr char P1_WIN_FP[] = "assets/player1_win.png";
constexpr char P2_WIN_FP[] = "assets/player2_win.png";

struct Ball {
    Vector2 position;
    Vector2 velocity;
    bool active;
};

AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f;

Vector2 gP1Position = {P1_X, SCREEN_HEIGHT / 2};
Vector2 gP2Position = {P2_X, SCREEN_HEIGHT / 2};
Vector2 gRacquetSize = {RACQUET_W, RACQUET_H};

Ball gBalls[MAX_BALLS];
int gBadminitonCount = 1;
int gP1Score = 0;
int gP2Score = 0;
bool gSinglePlayer = false;
bool gGameOver = false;
int  gWinner = 0;

Texture2D gP1Texture, gP2Texture, gBallTexture, gP1WinTexture, gP2WinTexture;

void initialise();
void processInput();
void update();
void render();
void shutdown();
bool isColliding(const Vector2 *posA, const Vector2 *scaleA, const Vector2 *posB, const Vector2 *scaleB);
void renderObject(const Texture2D *texture, const Vector2 *position, const Vector2 *scale);
void resetBall(int index, float velX, float velY);
void resetAllBalls();

bool isColliding(const Vector2 *posA, const Vector2 *scaleA, const Vector2 *posB, const Vector2 *scaleB){
    float xDist = fabs(posA->x - posB->x) - ((scaleA->x + scaleB->x) / 2.0f);
    float yDist = fabs(posA->y - posB->y) - ((scaleA->y + scaleB->y) / 2.0f);

    if (xDist < 0.0f && yDist < 0.0f){
        return true;
    }
    return false;
}

void renderObject(const Texture2D *texture, const Vector2 *position, const Vector2 *scale){
    Rectangle textureArea = {
        0.0f, 0.0f,
        static_cast<float> (texture->width),
        static_cast<float> (texture->height)
    };
    Rectangle destinationArea = {
        position->x,
        position->y,
        static_cast<float> (scale->x),
        static_cast<float> (scale->y)
    };

    Vector2 originOffset = {static_cast<float> (scale->x) / 2.0f, static_cast<float> (scale->y) / 2.0f};
    DrawTexturePro(*texture, textureArea, destinationArea, originOffset, 0.0f, WHITE);
}

void resetBall(int index, float velX, float velY){
    gBalls[index].position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    float len = sqrtf(velX * velX + velY * velY);
    gBalls[index].velocity = { (velX / len) * BALL_SPEED, (velY / len) * BALL_SPEED };
    gBalls[index].active   = (index < gBadminitonCount);
}

void resetAllBalls(){
    resetBall(0,1.0f,0.5f);
    resetBall(1,-1.0f,-0.5f);
    resetBall(2,1.0f,0.3f);
}

void initialise(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong Variations");
    gP1Texture = LoadTexture(RACQUET_LEFT_TEX_FP);
    gP2Texture = LoadTexture(RACQUET_RIGHT_TEX_FP);
    gBallTexture = LoadTexture(BADMINITON_TEX_FP);
    gP1WinTexture = LoadTexture(P1_WIN_FP);
    gP2WinTexture = LoadTexture(P2_WIN_FP);
    resetAllBalls();
    SetTargetFPS(FPS);
}

void processInput(){
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;

    if (gGameOver) return;

    if (IsKeyPressed(KEY_T)){
        gSinglePlayer = !gSinglePlayer;
    }
    if (IsKeyPressed(KEY_ONE)) {
        gBadminitonCount = 1;
    }
    if (IsKeyPressed(KEY_TWO)){
        gBadminitonCount = 2;
    } 
    if (IsKeyPressed(KEY_THREE)) {
        gBadminitonCount = 3;
    }
    if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_THREE)) {
    resetAllBalls();
    }
}

void update(){
    if (gGameOver) return;
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;
    if (IsKeyDown(KEY_W)){
        gP1Position.y -= RACQUET_SPEED * deltaTime;
    }
    if (IsKeyDown(KEY_S)){
        gP1Position.y += RACQUET_SPEED * deltaTime;
    }
    
    if (gSinglePlayer){
        for (int i = 0; i < MAX_BALLS; i++) {
            if (!gBalls[i].active) continue;
            if (gP2Position.y < gBalls[i].position.y - 4.0f){
                gP2Position.y += RACQUET_SPEED * deltaTime;
            }
            if (gP2Position.y > gBalls[i].position.y + 4.0f) {
                gP2Position.y -= RACQUET_SPEED * deltaTime;
            }
            break;
        }
    }
    else{
        if (IsKeyDown(KEY_UP)){
            gP2Position.y -= RACQUET_SPEED * deltaTime;
        }
        if (IsKeyDown(KEY_DOWN)){
            gP2Position.y += RACQUET_SPEED * deltaTime;
        }
    }

    float half = RACQUET_H / 2;
    if (gP1Position.y < half){
        gP1Position.y = half;
    } 
    if (gP1Position.y > SCREEN_HEIGHT - half) {
        gP1Position.y = SCREEN_HEIGHT - half;
    }
    if (gP2Position.y < half){
        gP2Position.y = half;
    }
    if (gP2Position.y > SCREEN_HEIGHT - half){
        gP2Position.y = SCREEN_HEIGHT - half;
    }
    Vector2 ballSize = { BALL_SIZE, BALL_SIZE };
    for (int i = 0; i < MAX_BALLS; i++){
        if (!gBalls[i].active) continue;

        gBalls[i].position.x += gBalls[i].velocity.x * deltaTime;
        gBalls[i].position.y += gBalls[i].velocity.y * deltaTime;
        if (gBalls[i].position.y - BALL_SIZE / 2.0f <= 0.0f) {
            gBalls[i].position.y = BALL_SIZE / 2;
            gBalls[i].velocity.y = fabs(gBalls[i].velocity.y);
        }
        if (gBalls[i].position.y + BALL_SIZE / 2.0f >= SCREEN_HEIGHT) {
            gBalls[i].position.y = SCREEN_HEIGHT - BALL_SIZE / 2;
            gBalls[i].velocity.y = -fabs(gBalls[i].velocity.y);
        }

        if (isColliding(&gBalls[i].position, &ballSize, &gP1Position, &gRacquetSize)) {
            gBalls[i].position.x = gP1Position.x + RACQUET_W / 2 + BALL_SIZE / 2;
            gBalls[i].velocity.x = fabs(gBalls[i].velocity.x);
        }
        if (isColliding(&gBalls[i].position, &ballSize, &gP2Position, &gRacquetSize)) {
            gBalls[i].position.x = gP2Position.x - RACQUET_W / 2 - BALL_SIZE / 2;
            gBalls[i].velocity.x = -fabs(gBalls[i].velocity.x);
        }

        if (gBalls[i].position.x + BALL_SIZE / 2.0f < 0.0f) {
            gP2Score++;
            if (gP2Score >= WIN_SCORE) { gGameOver = true; gWinner = 2; return; }
            resetAllBalls();
            return;
        }
        if (gBalls[i].position.x - BALL_SIZE / 2.0f > SCREEN_WIDTH) {
            gP1Score++;
            if (gP1Score >= WIN_SCORE) { gGameOver = true; gWinner = 1; return; }
            resetAllBalls();
            return;
        }
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    if (gGameOver)
    {
        Texture2D *winTex = (gWinner == 1) ? &gP1WinTexture : &gP2WinTexture;
        Vector2 centre = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
        float drawW = SCREEN_WIDTH * 0.6f;
        float drawH = drawW * ((float) winTex->height / (float) winTex->width);
        Vector2 size = {drawW, drawH};
        renderObject(winTex, &centre, &size);
    }
    else
    {
        for (int y = 0; y < SCREEN_HEIGHT; y += 20)
            DrawRectangle(SCREEN_WIDTH / 2 - 2, y, 4, 10, ColorFromHex("#FFFFFF33"));

        renderObject(&gP1Texture, &gP1Position, &gRacquetSize);
        renderObject(&gP2Texture, &gP2Position, &gRacquetSize);

        Vector2 ballSize = {BALL_SIZE, BALL_SIZE};
        for (int i = 0; i < MAX_BALLS; i++)
            if (gBalls[i].active)
                renderObject(&gBallTexture, &gBalls[i].position, &ballSize);

        char p1Str[8], p2Str[8];
        snprintf(p1Str, sizeof(p1Str), "%d", gP1Score);
        snprintf(p2Str, sizeof(p2Str), "%d", gP2Score);
        DrawText(p1Str, SCREEN_WIDTH / 2 - 80 - MeasureText(p1Str, 48), 20, 48, WHITE);
        DrawText(p2Str, SCREEN_WIDTH / 2 + 80, 20, 48, WHITE);

        const char *modeStr = gSinglePlayer ? "1P  [T]" : "2P  [T]";
        DrawText(modeStr, SCREEN_WIDTH / 2 - MeasureText(modeStr, 18) / 2, SCREEN_HEIGHT - 50, 18, LIGHTGRAY);

        char ballStr[32];
        snprintf(ballStr, sizeof(ballStr), "Balls: %d  [1][2][3]", gBadminitonCount);
        DrawText(ballStr, SCREEN_WIDTH / 2 - MeasureText(ballStr, 18) / 2, SCREEN_HEIGHT - 28, 18, LIGHTGRAY);
    }

    EndDrawing();
}

void shutdown()
{
    UnloadTexture(gP1Texture);
    UnloadTexture(gP2Texture);
    UnloadTexture(gBallTexture);
    UnloadTexture(gP1WinTexture);
    UnloadTexture(gP2WinTexture);
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
















