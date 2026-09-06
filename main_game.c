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





