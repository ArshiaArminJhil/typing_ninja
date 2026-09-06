#include "raylib.h"

typedef enum GameScreen {
TITLE = 0,
MAIN_MENU,
NAME_ENTRY,      
DIFFICULTY,     
LEVEL_SELECT,
GAMEPLAY
} GameScreen;
#define screenWidth 1000
#define screenHeight 800
void DrawButton(const char *text, int centerX, int y,int fontSize, int paddingX, int paddingY)
{
    int textWidth = MeasureText(text, fontSize);

    int rectWidth = textWidth + paddingX * 2;
    int rectHeight = fontSize + paddingY * 2;

    int rectX = centerX - rectWidth / 2;
    DrawRectangleRounded(
        (Rectangle){ rectX, y, rectWidth, rectHeight },0.2f,10, WHITE );
    DrawText( text,centerX - textWidth / 2,y + paddingY,fontSize,BLACK);
}
int main()
{
InitWindow(screenWidth, screenHeight, "Typing Ninja-Framework");
SetExitKey(0);
InitAudioDevice();
SetTargetFPS(60);

Texture2D ninjaLogo = LoadTexture("ninja_logo.png");
Music backgroundMusic = LoadMusicStream("background_music.mp3");
PlayMusicStream(backgroundMusic);
GameScreen currentScreen = TITLE;
char playerName[30]="" ;
int nameLength=0;

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
    nameLength = 0;
    playerName[0] = '\0';
    currentScreen = NAME_ENTRY;
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
    
case NAME_ENTRY:
{
    int charPressed = GetCharPressed();

    while (charPressed > 0)
    {
        if (charPressed >= 32 &&
            charPressed <= 125 &&
            nameLength < 29)
        {
            playerName[nameLength] = (char)charPressed;
            nameLength++;

            playerName[nameLength] = '\0';
        }

        charPressed = GetCharPressed();
    }

    if (key == KEY_BACKSPACE && nameLength > 0)
    {
        nameLength--;
        playerName[nameLength] = '\0';
    }

    if (key == KEY_ENTER && nameLength > 0)
    {
        currentScreen = DIFFICULTY;
    }

    if (key == KEY_ESCAPE)
    {
        currentScreen = MAIN_MENU;
    }

    break;
}

    case DIFFICULTY:
    if (key == KEY_E)
    {
    currentScreen = LEVEL_SELECT;
    }
    else if (key == KEY_M)
    {
     currentScreen = LEVEL_SELECT;
     }
     else if (key == KEY_H)
     {
    currentScreen = LEVEL_SELECT;
     }

    if (key == KEY_BACKSPACE)
    currentScreen = MAIN_MENU;
    break;

    case LEVEL_SELECT:
    if (key == KEY_ONE || key == KEY_TWO|| key==KEY_THREE)
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
        DrawRectangle(0,0,screenWidth,screenHeight,(Color){ 0, 0, 0, 80 });
        DrawButton("Press ENTER to Start",screenWidth/2,420,30,25,12);
        break;
        }

        case MAIN_MENU:
        {
        DrawRectangle(0, 0, screenWidth,screenHeight,(Color){0,0,0,130});
        DrawButton("MAIN MENU",screenWidth/2,120,28,25,10);
        DrawButton("[P] Play Game",screenWidth/2,210,28,25,10);
        DrawButton("[Practice Mode(Coming Soon)]",screenWidth/2,270,24,25,10);
        DrawButton("[X] Exit",screenWidth/2,340,18,20,8);
        DrawButton("BACKSPACE : Go Back",screenWidth/2,420,18,20,8);
break;
        }

case NAME_ENTRY:
{
    DrawRectangle(
        0, 0,
        screenWidth,
        screenHeight,
        (Color){ 0, 0, 0, 130 }
    );

    DrawButton("ENTER YOUR NAME",screenWidth / 2,120,40,25,12);

    DrawRectangleRounded((Rectangle){screenWidth / 2 - 250,240,500,70}, 0.2f, 10,WHITE);

    DrawText(
        playerName,
        screenWidth / 2 - MeasureText(playerName, 30) / 2,
        260,
        30,
        BLACK
    );

    DrawButton("Press ENTER to Continue",screenWidth / 2,360,20,20,8);

    DrawButton("ESC : Go Back", screenWidth / 2, 430, 18, 20, 8);
    break;
}

        case DIFFICULTY:
        {
        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){ 0, 0, 0, 130 });
        DrawButton("SELECT DIFFICULTY",screenWidth/2,120,40,25,12);
        DrawButton("[E]EASY",screenWidth/2,210,28,25,10);
        DrawButton("[M]MEDIUM",screenWidth/2,280,28,25,10);
        DrawButton("[H]HARD",screenWidth/2,350,28,25,10);
        DrawButton("BACKSPACE : Go back",screenWidth/2,450,18,20,8);

        break;
            }

        case LEVEL_SELECT:
        {
        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){ 0, 0, 0, 130 });

       DrawButton("[1] Level 1",screenWidth/2,220,28,25,10);
       DrawButton("[2] Level 2",screenWidth/2,300,28,25,10);
       DrawButton("[3] Level 3",screenWidth/2,380,28,25,10);
        break;
            }

        case GAMEPLAY:
         {
        DrawRectangle(0,0,screenWidth,screenHeight,(Color){0,0,0,100})  ;
        DrawButton("Press [ESC] for menu",screenWidth/2,500,40,20,8);
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
