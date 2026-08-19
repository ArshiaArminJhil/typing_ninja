#include "raylib.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#define screenwidth 1000
#define screenheight 800

int main()
{
    srand(time(NULL));
    int max_line=50;
    int max_len=30;
    int i=0;
char word[max_line][max_len];
    FILE *f;
    f=fopen("word.txt","r");
    if(f==NULL)
    {
return 0;
    }
    else
    {
while(fscanf(f,"%29s",word[i])==1)
{
    i++;
}
    }
    //int index=GetRandomValue(0,i-1);
    int index=rand()%i;
    int position_x=GetRandomValue(100,800);

    char targetword[50];
    strcpy(targetword,word[index]);
int wordx=position_x;
int wordy=0;
float fallingspeed=120.0f;
char inputword[100];
int wordlength=0;
        InitWindow(screenwidth,screenheight,"Typing Ninja");
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        BeginDrawing();
ClearBackground(RAYWHITE);
DrawText(targetword,wordx,wordy,40,RED);

wordy+=fallingspeed*GetFrameTime();
if(wordy>=800)
{
       //  index=GetRandomValue(0,i-1);
       index=rand()%i;
           strcpy(targetword,word[index]);

     position_x=GetRandomValue(100,800);

    wordy=0;
    wordx=position_x;
}
int key=GetCharPressed();
if((key>=65)&&(key<=122))
{
inputword[wordlength]=(char)key;
wordlength++;
inputword[wordlength]='\0';
}
if(IsKeyPressed(KEY_SPACE))
{
    if(strcmp(targetword,inputword)==0)
    {
                //index=GetRandomValue(0,i-1);
                index=rand()%i;
                    strcpy(targetword,word[index]);

    int position_x=GetRandomValue(100,800);

    wordy=0;
    wordx=position_x;

        wordlength=0;
        inputword[wordlength]='\0';
    }
    else
    {
        wordlength=0;
        inputword[wordlength]='\0';
    }
}
    EndDrawing();


    }
    CloseWindow();
    return 0;
}
