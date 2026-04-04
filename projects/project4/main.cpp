/**
* Author: Weixin Wang
* Assignment: Rise of the AI
* Date due: 2026-04-04, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Menu.h"
#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include "CS3113/WinScreen.h"
#include "CS3113/LoseScreen.h"

constexpr int SCREEN_WIDTH = 1000,
              SCREEN_HEIGHT = 600,
              FPS = 120,
              NUMBER_OF_SCENES = 6,
              STARTING_LIVES = 3;

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;


AppStatus gAppStatus = RUNNING;
float     gPreviousTicks = 0.0f,
          gTimeAccumulator = 0.0f;

Camera2D  gCamera = {0};

int gLives = STARTING_LIVES;
int gCurrentSceneID = 0;

Scene *gCurrentScene = nullptr;
Scene *gScenes[NUMBER_OF_SCENES];


Music gBGM;
Sound gJumpSound;
Sound gHurtSound;
Sound gLoseSound;
Sound gWinSound;


void switchToScene(int sceneID);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(int sceneID){
    if (gCurrentScene != nullptr && gCurrentScene->isGameplay()){
        gCurrentScene->shutdown();
    }

    gCurrentSceneID = sceneID;
    gCurrentScene = gScenes[sceneID];
    gCurrentScene->initialise();

    if (gCurrentScene->isGameplay() && gCurrentScene->getState().player != nullptr){
        gCamera.target = gCurrentScene->getState().player->getPosition();
    }
        
}

void initialise(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rise of the AI");
    InitAudioDevice();

    gBGM = LoadMusicStream("assets/game/bgm.mp3");
    SetMusicVolume(gBGM, 0.3f);
    PlayMusicStream(gBGM);
    gJumpSound = LoadSound("assets/game/jump.wav");
    gHurtSound = LoadSound("assets/game/hurt.wav");
    gLoseSound = LoadSound("assets/game/lose.wav");
    gWinSound  = LoadSound("assets/game/win.wav");
    gScenes[0] = new Menu(ORIGIN, "#1a1a2e");
    gScenes[1] = new LevelA(ORIGIN, "#16213e");
    gScenes[2] = new LevelB(ORIGIN, "#1a1a2e");
    gScenes[3] = new LevelC(ORIGIN, "#0f0f23");
    gScenes[4] = new WinScreen(ORIGIN, "#0f3d0f");
    gScenes[5] = new LoseScreen(ORIGIN, "#3d0f0f");
    switchToScene(0);

    gCamera.offset = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom = 1.0f;
    SetTargetFPS(FPS);
}

void processInput(){
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()){
        gAppStatus = TERMINATED;
        return;
    }

    if (IsKeyPressed(KEY_ONE)){ 
        switchToScene(1); 
        return;
    }
    if (IsKeyPressed(KEY_TWO)){
        switchToScene(2); 
        return;
    }
    if (IsKeyPressed(KEY_THREE)) {
        switchToScene(3); 
        return; 
    }

    if (!gCurrentScene->isGameplay()) return;
    if (gCurrentScene->getState().player == nullptr) return;

    Entity *player = gCurrentScene->getState().player;
    player->resetMovement();

    if (IsKeyDown(KEY_A)){
        player->moveLeft();
    }
    else if (IsKeyDown(KEY_D)){
        player->moveRight();
    }

    if (IsKeyPressed(KEY_W) && player->isCollidingBottom()){
        player->jump();
        PlaySound(gJumpSound);
    }

    if (GetLength(player->getMovement()) > 1.0f){
        player->normaliseMovement();
    }
}

void update(){
    UpdateMusicStream(gBGM);
    float ticks = (float)GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP){
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP){
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
    gTimeAccumulator = deltaTime;

    int nextID = gCurrentScene->getState().nextSceneID;

    if (nextID == -2) {
        gLives--;
        PlaySound(gHurtSound);

        if (gLives <= 0){
            PlaySound(gLoseSound);
            gLives = 0;
            switchToScene(5); 
        }
        else{
            switchToScene(gCurrentSceneID);
        }
    }
    else if (nextID == 0) {
        gLives = STARTING_LIVES; 
        switchToScene(0);
    }
    else if (nextID == 4) {
        PlaySound(gWinSound);
        switchToScene(4);
    }
    else if (nextID > 0 && nextID < NUMBER_OF_SCENES){
        switchToScene(nextID);
    }

    if (gCurrentScene->isGameplay() && gCurrentScene->getState().player != nullptr){
        Vector2 playerPos = gCurrentScene->getState().player->getPosition();
        Vector2 camTarget = { playerPos.x, ORIGIN.y };
        panCamera(&gCamera, &camTarget);
    }
}

void render(){
    BeginDrawing();

    if (gCurrentScene->isGameplay()){
        BeginMode2D(gCamera);
        gCurrentScene->render();
        EndMode2D();
        char livesText[32];
        snprintf(livesText, sizeof(livesText), "Lives: %d", gLives);
        DrawText(livesText, 20, 20, 28, WHITE);

        const char *levelName = "";
        if (gCurrentSceneID == 1){
            levelName = "Level 1";
        }
        else if (gCurrentSceneID == 2){
            levelName = "Level 2";
        }
        else if (gCurrentSceneID == 3){
            levelName = "Level 3";
        }
        DrawText(levelName, SCREEN_WIDTH - 150, 20, 24, LIGHTGRAY);
    }
    else{
        gCurrentScene->render();
    }
    EndDrawing();
}

void shutdown(){
    for (int i = 0; i < NUMBER_OF_SCENES; i++){
        delete gScenes[i];
        gScenes[i] = nullptr;
    }

    UnloadMusicStream(gBGM);
    UnloadSound(gJumpSound);
    UnloadSound(gHurtSound);
    UnloadSound(gLoseSound);
    UnloadSound(gWinSound);

    CloseAudioDevice();
    CloseWindow();
}

int main(void){
    initialise();

    while (gAppStatus == RUNNING){
        processInput();
        update();
        render();
    }

    shutdown();
    return 0;
}
