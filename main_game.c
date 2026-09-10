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
#define bottom_line_y 750




typedef enum{
TITLE = 0,
MAIN_MENU, 
 NAME_ENTRY,
 DIFFICULTY,     
LEVEL_SELECT,
LEVEL_READY,
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



GameScreen currentScreen=TITLE;
Difficultymode selecteddiff=DIFF_EASY;
int selectedlevel=1;
int maxdifficultyunlocked=DIFF_EASY;


float speed=0.0f;
float timer=0.0f;
int score=0;
int target=0;
bool GAME_OVER=false;
bool complete_target=false;
bool ismuted=false;
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
   timer-=dt;
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
InitWindow(screenwidth, screenheight, "Typing Ninja");
srand(time(NULL));
SetExitKey(0);
InitAudioDevice();
SetTargetFPS(60);

Texture2D ninjaLogo = LoadTexture("ninja_logo.png");
Texture2D easy = LoadTexture("coverphoto_easy.png");
//if(easy.id<=0)
//{
//TraceLog(LOG_WARNING,"EMAGE FAILED TO LOAD");
//}
//printf("Easy image: %d x %d\n", easy.width, easy.height);
Music backgroundMusic = LoadMusicStream("background_music.mp3");
PlayMusicStream(backgroundMusic);
GameScreen currentScreen = TITLE;
char playerName[30]="" ;
int nameLength=0;

    while (!WindowShouldClose())
    {
    UpdateMusicStream(backgroundMusic);

     int key = GetKeyPressed();
     float deltatime=GetFrameTime();

    switch (currentScreen)
    {
    case TITLE:
    {
    if (key == KEY_ENTER)
    {
    currentScreen = MAIN_MENU;
     break;
    }
    }
    case MAIN_MENU:
    {Rectangle mutebuttonrec={(float)screenwidth-150,20,120,40};
    if(CheckCollisionPointRec(GetMousePosition(),mutebuttonrec)&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        ismuted = !ismuted;
        SetMasterVolume(ismuted?0.0f:1.0f);
    }
    if (key == KEY_P)
    {
    nameLength = 0;
   playerName[0] = '\0';
   while(GetCharPressed()>0);
    currentScreen = NAME_ENTRY;
    }
    else if (key == KEY_R)
    {
     // we will do it later
     }
    else if (key == KEY_X)
    {
    //later;
    }
                
    if (key == KEY_BACKSPACE)
    {
    currentScreen = TITLE;
    break;}
    }
    
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
    {
    if (key == KEY_E)
    {
        selecteddiff=DIFF_EASY;
    currentScreen = LEVEL_SELECT;
    }
    else if (key == KEY_M)
    {if(maxdifficultyunlocked>=DIFF_MEDIUM)
        {
                    selecteddiff=DIFF_MEDIUM;

     currentScreen = LEVEL_SELECT;
        }
     }
     else if (key == KEY_H)
     {if(maxdifficultyunlocked>=DIFF_HARD)
        {

                selecteddiff=DIFF_HARD;

    currentScreen = LEVEL_SELECT;
     }
    }
    if (key == KEY_BACKSPACE)
    {
    currentScreen = MAIN_MENU;
    }
    break;
    }
    case LEVEL_SELECT:
    {
    if(key==KEY_ONE)
    {
        selectedlevel=1;
    }
    else if(key==KEY_TWO)
    {
        selectedlevel=2;
    }
    else if(key==KEY_THREE)
    {
        selectedlevel=3;
    }
    if(key==KEY_ONE||key==KEY_TWO||key==KEY_THREE)
    {
        initgameplay();
        if(selecteddiff==DIFF_EASY)
        {
            speed=80.0f;
            if(selectedlevel==1)
            {
                timer=60.0f;
                target=100;
                loadword("word.txt");
                
            }
            else if(selectedlevel==2)
            {
                timer=50.0f;
                target=120;
                loadword("word.txt");
            }
            else
            {
                timer=40.0f;
                target=150;
                loadword("word.txt");
   
            }
        }
        else if(selecteddiff==DIFF_MEDIUM)
        {
                        speed=120.0f;
            if(selectedlevel==1)
            {
                timer=40.0f;
                target=100;
                loadword("word.txt");
                
            }
            else if(selectedlevel==2)
            {
                timer=35.0f;
                target=200;
                loadword("word.txt");
            }
            else
            {
                timer=30.0f;
                target=300;
                loadword("word.txt");
   
            }

        }
        else
        {
                        speed=160.0f;
            if(selectedlevel==1)
            {
                timer=30.0f;
                target=100;
                loadword("word.txt");
                
            }
            else if(selectedlevel==2)
            {
                timer=25.0f;
                target=200;
                loadword("word.txt");
            }
            else
            {
                timer=20.0f;
                target=300;
                loadword("word.txt");
   
            }

        }
            currentScreen = LEVEL_READY;


    }


    if (key == KEY_BACKSPACE)
    {
    currentScreen = DIFFICULTY;
    }
    break;
    }
    case LEVEL_READY:
    {
        if (key == KEY_ENTER)
        {
            currentScreen = GAMEPLAY; //Actually start the game here
        }
        if (key == KEY_BACKSPACE)
        {
            currentScreen = LEVEL_SELECT;
        }
        break;
    }
    case GAMEPLAY:
    {
        updatefallingword(deltatime);
        handleplayertyping();
        if(score>=target)
        {
            complete_target=true;
            if(selecteddiff==DIFF_EASY&&selectedlevel==3)
            {
                if(maxdifficultyunlocked<DIFF_MEDIUM)
                {
                    maxdifficultyunlocked=DIFF_MEDIUM;
                }
            }
                        if(selecteddiff==DIFF_MEDIUM&&selectedlevel==3)
            {
                if(maxdifficultyunlocked<DIFF_HARD)
                {
                    maxdifficultyunlocked=DIFF_HARD;
                }
            }
currentScreen=GAMEOVER;
        }
       else if(timer<=0.0f)
        {
            timer=0.0f;
            complete_target=false;
            currentScreen=GAMEOVER;

        }
     if (key == KEY_ESCAPE)
    currentScreen = LEVEL_SELECT;
    break;
    }
    case GAMEOVER:
    {
        if(key==KEY_ENTER)
        {
            currentScreen=LEVEL_SELECT;
            break;
        }
    }
}



//drawing logic
        BeginDrawing();

        ClearBackground(RAYWHITE);

        

        switch (currentScreen)
        {
        case TITLE:
        {//Logo bg for title screen
            DrawTexturePro(
                ninjaLogo,
        (Rectangle){ 0, 0, (float)ninjaLogo.width, (float)ninjaLogo.height },
         (Rectangle){ 0, 0, (float)screenwidth, (float)screenheight },
        (Vector2){ 0, 0 },0.0f,WHITE
        );
        DrawRectangle(0,0,screenwidth,screenheight,(Color){ 0, 0, 0, 80 });
        DrawButton("Press ENTER to Start",screenwidth/2,420,30,25,12);
        break;
        }

        case MAIN_MENU:
        {
            DrawTexturePro(
                ninjaLogo,
        (Rectangle){ 0, 0, (float)ninjaLogo.width, (float)ninjaLogo.height },
         (Rectangle){ 0, 0, (float)screenwidth, (float)screenheight },
        (Vector2){ 0, 0 },0.0f,WHITE
        );
        DrawRectangle(0, 0, screenwidth,screenheight,(Color){0,0,0,130});
        DrawButton(ismuted ?"Unmute" : "Mute",screenwidth-90,40,20,15,8);
        DrawButton("MAIN MENU",screenwidth/2,120,28,25,10);
        DrawButton("[P] Play Game",screenwidth/2,210,28,25,10);
        DrawButton("[Practice Mode(Coming Soon)]",screenwidth/2,270,24,25,10);
        DrawButton("[X] Exit",screenwidth/2,340,18,20,8);
        DrawButton("BACKSPACE : Go Back",screenwidth/2,420,18,20,8);
break;
        }

case NAME_ENTRY:
{DrawTexturePro(
                ninjaLogo,
        (Rectangle){ 0, 0, (float)ninjaLogo.width, (float)ninjaLogo.height },
         (Rectangle){ 0, 0, (float)screenwidth, (float)screenheight },
        (Vector2){ 0, 0 },0.0f,WHITE
        );
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
            DrawTexturePro(
                ninjaLogo,
        (Rectangle){ 0, 0, (float)ninjaLogo.width, (float)ninjaLogo.height },
         (Rectangle){ 0, 0, (float)screenwidth, (float)screenheight },
        (Vector2){ 0, 0 },0.0f,WHITE
        );
        DrawRectangle(0, 0, screenwidth, screenheight, (Color){ 0, 0, 0, 130 });
        DrawButton("SELECT DIFFICULTY",screenwidth/2,120,40,25,12);
        DrawButton("[E]EASY",screenwidth/2,210,28,25,10);
        if(maxdifficultyunlocked>=DIFF_MEDIUM)
        {
        DrawButton("[M]MEDIUM",screenwidth/2,280,28,25,10);
        }
        else
        {
            DrawButton("[M]MEDIUM-LOCKED",screenwidth/2,280,28,25,10);
        }
        if(maxdifficultyunlocked>=DIFF_HARD)
        {
        DrawButton("[H]HARD",screenwidth/2,350,28,25,10);
        }
        else
        {
            DrawButton("[H]HARD-LOCKED",screenwidth/2,350,28,25,10);
        }


        DrawButton("BACKSPACE : Go back",screenwidth/2,450,18,20,8);

        break;
            }

        case LEVEL_SELECT:
        {
         DrawTexturePro( 
                ninjaLogo,
        (Rectangle){ 0, 0, (float)ninjaLogo.width, (float)ninjaLogo.height },
        (Rectangle){ 0, 0, (float)screenwidth, (float)screenheight },
        (Vector2){ 0, 0 },0.0f,WHITE
        );
       DrawRectangle(0, 0, screenwidth, screenheight, (Color){ 0, 0, 0, 130 });
       DrawButton("[1] Level 1",screenwidth/2,220,28,25,10);
       DrawButton("[2] Level 2",screenwidth/2,300,28,25,10);
       DrawButton("[3] Level 3",screenwidth/2,380,28,25,10);
       DrawButton("press [Backspace] to return",screenwidth/2,540,28,25,10);
        break;
            }

            case LEVEL_READY:
        {
            DrawTexturePro(
                ninjaLogo,
                (Rectangle){ 0, 0, (float)ninjaLogo.width, (float)ninjaLogo.height },
                (Rectangle){ 0, 0, (float)screenwidth, (float)screenheight },
                (Vector2){ 0, 0 }, 0.0f, WHITE
            );
            DrawRectangle(0, 0, screenwidth, screenheight, (Color){ 0, 0, 0, 130 });

            DrawButton(TextFormat("LEVEL %d SELECTED", selectedlevel), screenwidth / 2, 200, 35, 25, 12);
            DrawButton(TextFormat("Target: %d | Time: %.0fs", target, timer), screenwidth / 2, 280, 25, 20, 8);
            DrawButton("Press [ENTER] to Start Game", screenwidth / 2, 360, 25, 20, 8);
            DrawButton("[Backspace] Go Back", screenwidth / 2, 440, 18, 20, 8);
            break;
        }

        case GAMEPLAY:
         {
            DrawTexturePro(
                easy,
                (Rectangle){ 0, 0, (float)easy.width, (float)easy.height },
                (Rectangle){ 0, 0, (float)screenwidth, (float)screenheight },
                (Vector2){ 0, 0 }, 0.0f, WHITE
            );
             DrawRectangle(0, 0, screenwidth, screenheight, (Color){ 0, 0, 0, 0 });
            DrawLine(0,bottom_line_y,screenwidth,bottom_line_y,RED);
                DrawText(TextFormat("SCORE: %d",score),750,20,40,RED);
    DrawText(TextFormat("TIME: %.1f s",timer),400,20,40,RED);
    DrawText(TextFormat("TARGET: %d",target),50,20,40,RED);

        for(int i=0;i<MAX_WORD_ONSCREEN;i++)
        {
            if(screenword[i].active)
            {
             DrawText(screenword[i].text,screenword[i].position.x,screenword[i].position.y,40,RED);


            }
        }
                     DrawText(TextFormat("INPUT:%s",inputword),50,750,40,RED);


        //DrawButton("Press [ESC] for menu",screenwidth/2,750,40,20,8);
        break;
            }
            case GAMEOVER:
            {
                if(complete_target)
                {
                    DrawText("LEVEL COMPLETED",screenwidth/2-200,screenheight/2-100,45,RED);
DrawText(TextFormat("SCORE: %d",score),screenwidth/2-100,screenheight/2-20,40,RED);
 DrawText("PRESS [ENTER] TO RETURN",screenwidth/2-240,screenheight/2+60,30,RED);

                }
                else
                {
                    DrawText("GAME OVER",screenwidth/2-150,screenheight/2-100,50,RED);
DrawText(TextFormat("SCORE: %d",score),screenwidth/2-100,screenheight/2-20,40,RED);
DrawText("PRESS [ENTER] TO RETURN",screenwidth/2-220,screenheight/2+60,30,RED);

                }
               
            }
        }
    
        EndDrawing();

    }
    UnloadTexture(easy);
     UnloadTexture(ninjaLogo);
    StopMusicStream(backgroundMusic); 
    UnloadMusicStream(backgroundMusic);
    CloseAudioDevice();

    CloseWindow();   

    return 0;
}