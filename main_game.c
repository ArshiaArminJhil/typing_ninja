<<<<<<< Updated upstream
#include "raylib.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>



#define screenwidth 1000
#define screenheight 800
#define MAX_LINE 50
#define MAX_LEN 30
#define MAX_WORD_ONSCREEN 5
#define TARGET 100
#define bottom_line_y 350




typedef enum{
TITLE = 0,
MAIN_MENU, 
 NAME_ENTRY,
 DIFFICULTY,     
LEVEL_SELECT,
GAMEPLAY,
GAMEOVER
} GameScreen;



typedef enum {
    DIFF_EASY,
    DIFF_MEDIUM,
    DIFF_HARD
}Difficultymode;



typedef struct{
char text[MAX_LEN];
Vector2 position;
float speed;
bool active;
}Fallingword;



GameScreen currentscreen=TITLE;
Difficultymode selecteddiff=DIFF_EASY;
int selectedlevel=1;
int maxdifficultyunlocked=DIFF_EASY;


float speed=120.0f;
float time=60.0f;
int score=0;
bool GAME_OVER=false;
bool complete_target=false;
float spawntime=0.0f;
float spawninterval=2.0f;



 Fallingword screenword[MAX_WORD_ONSCREEN];
 int totalword=0;
char word[MAX_LINE][MAX_LEN];
char inputword[100];
int wordlength=0;



int loadword(char* filename)
{
    FILE *f;
    f=fopen("word.txt","r");
    if(f==NULL)
    {
return 0;
    }
    else
    {
while(fscanf(f,"%29s",word[totalword])==1)
{
    totalword++;
}
    }
    fclose(f);
return totalword;
}





void initgameplay()
{
        {
 time=60.0f;
score=0;
 GAME_OVER=false;
 bool complete_target=false;

 spawntime=0.0f;
 wordlength=0;
   inputword[wordlength]='\0';
    for(int i=0;i<MAX_WORD_ONSCREEN;i++)
    {
        screenword[i].active=false;
    }

    }

}



void spawnword(int index)
{
    int i=rand()%totalword;
    strcpy(screenword[index].text,word[i]);
    screenword[index].position.x=GetRandomValue(100,screenwidth-200);
    screenword[index].position.y=GetRandomValue(-150,-40);
    screenword[index].speed=speed;
    screenword[index].active=true;
}




void updatefallingword(float dt)
{
   time-=dt;
 spawntime+=dt;
if(spawntime>=spawninterval)
{
    for(int i=0;i<MAX_WORD_ONSCREEN;i++)
    {
        if(!screenword[i].active)
        {
            spawnword(i);
            break;
        }
    }
    spawntime=0.0f;
}
    for(int i=0;i<MAX_WORD_ONSCREEN;i++)
    {
        if(screenword[i].active)
        {
            screenword[i].position.y+=screenword[i].speed*GetFrameTime();
            if(screenword[i].position.y>=bottom_line_y)
            {
                screenword[i].active=false;
                score-=5;
            }
        }
    }


}

void handleplayertyping()
{
    int key=GetCharPressed();
while(key>0)
{
if((key>=65)&&(key<=122))
{
inputword[wordlength]=(char)key;
wordlength++;
inputword[wordlength]='\0';
}
key=GetCharPressed();
}
if(IsKeyPressed(KEY_BACKSPACE))
{
    wordlength--;
    if(wordlength<0)
    {
        wordlength=0;
    }
    inputword[wordlength]='\0';
}
if(IsKeyPressed(KEY_ENTER))
{
    bool matchfound=false;
    for(int i=0;i<MAX_WORD_ONSCREEN;i++)
    {
        if(screenword[i].active)
        {
                if(strcmp(screenword[i].text,inputword)==0)
                {
                    screenword[i].active=false;
                    score+=10;
                    matchfound=true;
                    break;
                }
            
        

        }
    }
            wordlength=0;
           inputword[wordlength]='\0';
                           if(!matchfound)
                {
                    score-=5;
                }


    }
}






















//Jhil's code
//this is a funtion for drawing rectangle behind every text
void DrawButton(const char *text, int centerX, int y,int fontSize, int paddingX, int paddingY)
{
    int textWidth = MeasureText(text, fontSize);

    int rectWidth = textWidth + paddingX * 2;
    int rectHeight = fontSize + paddingY * 2;

    int rectX = centerX - rectWidth / 2;
    //to make the corners round shape
    DrawRectangleRounded((Rectangle){ rectX, y, rectWidth, rectHeight },0.2f,10, WHITE );
    DrawText( text,centerX - textWidth / 2,y + paddingY,fontSize,BLACK);
}
int main()
{
InitWindow(screenwidth, screenheight, "Typing Ninja-Framework");
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
    {if(maxDifficultyUnlocked>=DIFF_MEDIUM)
     currentScreen = LEVEL_SELECT;
     }
     else if (key == KEY_H)
     {if(maxDifficultyUnlocked>=DIFF_HARD)
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
         (Rectangle){ 0, 0, (float)screenwidth, (float)screenheight },
        (Vector2){ 0, 0 },0.0f,WHITE
        );

        switch (currentScreen)
        {
        case TITLE:
        {
        DrawRectangle(0,0,screenwidth,screenheight,(Color){ 0, 0, 0, 80 });
        DrawButton("Press ENTER to Start",screenwidth/2,420,30,25,12);
        break;
        }

        case MAIN_MENU:
        {
        DrawRectangle(0, 0, screenwidth,screenheight,(Color){0,0,0,130});
        DrawButton("MAIN MENU",screenwidth/2,120,28,25,10);
        DrawButton("[P] Play Game",screenwidth/2,210,28,25,10);
        DrawButton("[Practice Mode(Coming Soon)]",screenwidth/2,270,24,25,10);
        DrawButton("[X] Exit",screenwidth/2,340,18,20,8);
        DrawButton("BACKSPACE : Go Back",screenwidth/2,420,18,20,8);
break;
        }

case NAME_ENTRY:
{
    DrawRectangle(0, 0, screenwidth, screenheight, (Color){ 0, 0, 0, 130 });

    DrawButton("ENTER YOUR NAME",screenwidth / 2,120,40,25,12);

    DrawRectangleRounded((Rectangle){screenwidth / 2 - 250,240,500,70}, 0.2f, 10,WHITE);

    DrawText( playerName,screenwidth / 2 - MeasureText(playerName, 30) / 2,260,30,BLACK);

    DrawButton("Press ENTER to Continue",screenwidth / 2,360,20,20,8);

    DrawButton("ESC : Go Back", screenwidth / 2, 430, 18, 20, 8);
    break;
}

        case DIFFICULTY:
        {
        DrawRectangle(0, 0, screenwidth, screenheight, (Color){ 0, 0, 0, 130 });
        DrawButton("SELECT DIFFICULTY",screenwidth/2,120,40,25,12);
        DrawButton("[E]EASY",screenwidth/2,210,28,25,10);
        DrawButton("[M]MEDIUM",screenwidth/2,280,28,25,10);
        DrawButton("[H]HARD",screenwidth/2,350,28,25,10);
        DrawButton("BACKSPACE : Go back",screenwidth/2,450,18,20,8);

        break;
            }

        case LEVEL_SELECT:
        {
        DrawRectangle(0, 0, screenwidth, screenheight, (Color){ 0, 0, 0, 130 });

       DrawButton("[1] Level 1",screenwidth/2,220,28,25,10);
       DrawButton("[2] Level 2",screenwidth/2,300,28,25,10);
       DrawButton("[3] Level 3",screenwidth/2,380,28,25,10);
        break;
            }

        case GAMEPLAY:
         {
        DrawRectangle(0,0,screenwidth,screenheight,(Color){0,0,0,100})  ;
        DrawButton("Press [ESC] for menu",screenwidth/2,500,40,20,8);
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


>>>>>>> Stashed changes
