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
    struct fallingword
    {
char text[MAX_LEN];
Vector2 position;
float speed;
bool active;
    };
  struct fallingword screenword[MAX_WORD_ONSCREEN];
 int totalword=0;
char word[MAX_LINE][MAX_LEN];

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
return totalword;
}
void spawnword(int index)
{
    int i=rand()%totalword;
    strcpy(screenword[index].text,word[i]);
    screenword[index].position.x=GetRandomValue(100,screenwidth-200);
    screenword[index].position.y=GetRandomValue(-150,-40);
    screenword[index].speed=80.0f;
    screenword[index].active=true;
}

int main()
{
    int load=loadword("word.txt");
    if(load==0)
    {
        printf("error");
        return 0;
    }
    for(int i=0;i<MAX_WORD_ONSCREEN;i++)
    {
        screenword[i].active=false;
    }
    srand(time(NULL));
float time=60.0f;
int score=0;
bool GAME_OVER=false;
bool complete_target=false;
float spawntime=0.0f;
char inputword[100];
int wordlength=0;
        InitWindow(screenwidth,screenheight,"Typing Ninja");
    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        if(!GAME_OVER)
        {
float deltatime=GetFrameTime();
time-=deltatime;
if(time<0)
{
    time=0;
    GAME_OVER=true;
}
if(score>=TARGET)
{
    complete_target=true;
    GAME_OVER=true;
}
spawntime+=deltatime;
if(spawntime>=2.0f)
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
            if(screenword[i].position.y>=800)
            {
                screenword[i].active=false;
                score-=5;
            }
        }
    }



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

else
{
    if(IsKeyPressed(KEY_R))
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
            BeginDrawing();
ClearBackground(RAYWHITE);

if(GAME_OVER)
{
    if(complete_target)
    {
DrawText("LEVEL COMPLETED",screenwidth/3,screenheight/2+20,50,RED);
DrawText(TextFormat("SCORE: %d",score),screenwidth/2-25,screenheight/2-100,50,RED);

    }
    else
    {
DrawText("GAME OVER",screenwidth/2-200,screenheight/2-200,50,RED);
DrawText(TextFormat("SCORE: %d",score),screenwidth/2-25,screenheight/2-100,50,RED);
DrawText("PRESS [R] TO RESTART",screenwidth/3-150,screenheight/2,50,RED);
    }
}
else
{
    DrawText(TextFormat("SCORE: %d",score),750,20,40,BLACK);
    DrawText(TextFormat("TIME: %.0f s",time),400,20,40,BLACK);
    DrawText("TARGET: 100",50,20,40,BLACK);

    for(int i=0;i<MAX_WORD_ONSCREEN;i++)
    {
        if(screenword[i].active)
        {
            DrawText(screenword[i].text,screenword[i].position.x,screenword[i].position.y,40,RED);
            DrawText(TextFormat("INPUT:%s",inputword),50,750,40,BLACK);
        }
    }
}

    EndDrawing();
    }

    
    CloseWindow();
    return 0;
}
