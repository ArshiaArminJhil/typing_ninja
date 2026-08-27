#include "raylib.h"

typedef enum GameScreen {
TITLE = 0,
MAIN_MENU,      
 DIFFICULTY,     
LEVEL_SELECT,
GAMEPLAY
} GameScreen;
#define screenWidth 800
#define screenHeight 600
int main(void)
{
InitWindow(screenWidth, screenHeight, "Typing Ninja-Framework");
SetExitKey(0);
InitAudioDevice();
SetTargetFPS(60);

Texture2D ninjaLogo = LoadTexture("ninja_logo.png");
Music backgroundMusic = LoadMusicStream("background_music.mp3");
PlayMusicStream(backgroundMusic);
GameScreen currentScreen = TITLE;
int lives ;

    while (!WindowShouldClose())
    {
    UpdateMusicStream(backgroundMusic);

     int key = GetKeyPressed();

    switch (currentScreen)
    {
    case TITLE:
    if (key == KEY_ENTER)
    currentScreen = MAIN_MENU;
     break;

    case MAIN_MENU:
    if (key == KEY_P)
    {
     currentScreen = DIFFICULTY;
    }
    else if (key == KEY_R)
    {
     // we will do it later
     }
    else if (key == KEY_X)
    {
    goto cleanup;
    }
                
    if (key == KEY_BACKSPACE)
    currentScreen = TITLE;
    break;

    case DIFFICULTY:
    if (key == KEY_E)
    {
    lives = 4;
    currentScreen = LEVEL_SELECT;
    }
    else if (key == KEY_M)
    {
     lives = 3;
     currentScreen = LEVEL_SELECT;
     }
     else if (key == KEY_H)
     {
    lives = 2;
    currentScreen = LEVEL_SELECT;
     }

    if (key == KEY_BACKSPACE)
    currentScreen = MAIN_MENU;
    break;

    case LEVEL_SELECT:
    if (key == KEY_ONE || key == KEY_TWO)
    currentScreen = GAMEPLAY;

    if (key == KEY_BACKSPACE)
    currentScreen = DIFFICULTY;
    break;

    case GAMEPLAY:
     if (key == KEY_ESCAPE)
    currentScreen = MAIN_MENU;
    break;
    }
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexturePro(
         ninjaLogo,
        (Rectangle){ 0, 0, (float)ninjaLogo.width, (float)ninjaLogo.height },
         (Rectangle){ 0, 0, (float)screenWidth, (float)screenHeight },
        (Vector2){ 0, 0 },0.0f,WHITE
        );

        switch (currentScreen)
        {
        case TITLE:
        {
        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){ 0, 0, 0, 80 });

        const char *startText = "Press ENTER to Start";
        DrawText(startText, screenWidth / 2 - MeasureText(startText, 24) / 2, 420, 24, WHITE);
        break;
            }

        case MAIN_MENU:
         {
        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){ 0, 0, 0, 130 });

        const char *title = "MAIN MENU";
        const char *play = "[P] Play Game";
        const char *practice = "[R] Practice Mode (Coming Soon)";
        const char *exit = "[X] Exit";
        const char *back = "Press BACKSPACE to go back";

        DrawText(title, screenWidth / 2 - MeasureText(title, 30) / 2, 130, 30, WHITE);
        DrawText(play, screenWidth / 2 - MeasureText(play, 20) / 2, 210, 20, WHITE);
        DrawText(practice, screenWidth / 2 - MeasureText(practice, 20) / 2, 250, 20, WHITE);
        DrawText(exit, screenWidth / 2 - MeasureText(exit, 20) / 2, 290, 20, WHITE);
        DrawText(back, screenWidth / 2 - MeasureText(back, 14) / 2, 380, 15, WHITE);
        break;
            }

        case DIFFICULTY:
        {
        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){ 0, 0, 0, 130 });

        const char *title = "SELECT DIFFICULTY";
        const char *easy = "[E] Easy (4 Lives)";
        const char *medium = "[M] Medium (3 Lives)";
        const char *hard = "[H] Hard (2 Lives)";
        const char *back = "Press BACKSPACE to go back";

        DrawText(title, screenWidth / 2 - MeasureText(title, 30) / 2, 130, 30, WHITE);
        DrawText(easy, screenWidth / 2 - MeasureText(easy, 20) / 2, 210, 20, WHITE);
        DrawText(medium, screenWidth / 2 - MeasureText(medium, 20) / 2, 250, 20, WHITE);
        DrawText(hard, screenWidth / 2 - MeasureText(hard, 20) / 2, 290, 20, WHITE);
        DrawText(back, screenWidth / 2 - MeasureText(back, 15) / 2, 380, 14, WHITE);
        break;
            }

        case LEVEL_SELECT:
        {
        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){ 0, 0, 0, 130 });

        const char *title = "SELECT LEVEL";
        const char *letters = "[1] Level 1: Letters";
        const char *words = "[2] Level 2: Words";
        const char *back = "Press BACKSPACE to go back";

        DrawText(title, screenWidth / 2 - MeasureText(title, 30) / 2, 150, 30, WHITE);
        DrawText(letters, screenWidth / 2 - MeasureText(letters, 20) / 2, 230, 20, WHITE);
        DrawText(words, screenWidth / 2 - MeasureText(words, 20) / 2, 270, 20, WHITE);
        DrawText(back, screenWidth / 2 - MeasureText(back, 14) / 2, 380, 14, LIGHTGRAY);
        break;
            }

        case GAMEPLAY:
         {
        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){ 0, 0, 0, 100 });

        DrawText(TextFormat("Lives: %i", lives), screenWidth - 120, 20, 20, RED);

        const char *gameTitle = "GAMEPLAY SCREEN";
        DrawText(gameTitle, screenWidth / 2 - MeasureText(gameTitle, 20) / 2, 200, 20, WHITE);

        const char *controls = "Press [ESC] for Menu";
        DrawText(controls, screenWidth / 2 - MeasureText(controls, 12) / 2, 320, 12, WHITE);
        break;
            }
        }

        EndDrawing();
    }

cleanup:
    UnloadTexture(ninjaLogo);
    StopMusicStream(backgroundMusic);
    UnloadMusicStream(backgroundMusic);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
