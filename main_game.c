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
#define MAX_WORD_ONSCREEN 8
#define bottom_line_y 582




typedef enum{
TITLE = 0,
MAIN_MENU, 
 NAME_ENTRY,
 DIFFICULTY,     
LEVEL_SELECT,
LEVEL_READY,
GAMEPLAY,
GAMEOVER,
RECORDS
} GameScreen;



typedef enum {
    DIFF_EASY,
    DIFF_MEDIUM,
    DIFF_HARD
}Difficultymode;




typedef enum
{
    powerup_none=0,
    powerup_bullet_time,
    powerup_hyper_speed,
    powerup_time_warp,
    powerup_screen_wipe,
    powerup_freeze,
    powerup_rush,
    powerup_shock,
    powerup_shrink,
}poweruptype;
//new
typedef struct {
    char name[30];
    int difficultyUnlocked; 
    int highestLevel;       
} PlayerRecord;

const char* GetNinjaRank(int diff, int level) {
    // Jonin: Reached Hard difficulty and completed level 3
    if (diff == DIFF_HARD && level >= 3) {
        return "Jonin";
    }
    // Chunin: Reached Hard difficulty (meaning Medium level 3 was fully completed)
    if (diff == DIFF_HARD) {
        return "Chunin";
    }
    // Genin: Reached Medium difficulty (meaning Easy level 3 was fully completed)
    if (diff == DIFF_MEDIUM) {
        return "Genin";
    }
    
    return "Not Yet";
}
typedef struct{
char text[MAX_LEN];
Vector2 position;
float speed;
bool active;
poweruptype powerupeffect;
bool issliced;
char lefthalf[MAX_LEN];
char righthalf[MAX_LEN];
Vector2 leftvelocity;
Vector2 rightvelocity;
Vector2 leftposoffset;
Vector2 rightposoffset;
float slashtimer;
Vector2 slashstart;
Vector2 slashend;
}Fallingword;

Sound slicesound;




poweruptype activeglobalpowerup=powerup_none;
float poweruptimer=0.0f;
float speedmodifier=1.0f;
int scoremultiplier=1;
bool iswipingdown=false;

bool canusebullet=false;
bool canusehyper=false;
bool canusewarp=false;
bool canusewipe=false;

GameScreen currentScreen=TITLE;
Difficultymode selecteddiff=DIFF_EASY;
int selectedlevel=1;
int maxdifficultyunlocked=DIFF_EASY;

char playerName[30]="" ;
int nameLength=0;
float speed=0.0f;
float timer=0.0f;
int score=0;
int target=0;
bool GAME_OVER=false;
bool complete_target=false;
bool ismuted=false;
bool isPaused = false;
float spawntime=0.0f;
float spawninterval=1.0f;
int maxlevelunlocked[3] = {1, 1, 1};



 Fallingword screenword[MAX_WORD_ONSCREEN];
 int totalword=0;
char word[MAX_LINE][MAX_LEN];
char inputword[100];
int wordlength=0;

//new
void SaveRecord() {
    PlayerRecord records[50];
    int totalRecords = 0;
    bool found = false;

    FILE *f = fopen("records.txt", "r");
    if (f != NULL) {
        char filePlayer[30];
        int fileDiff, fileLevel;
        while (totalRecords < 50 && fscanf(f, "%29s %d %d", filePlayer, &fileDiff, &fileLevel) == 3) {
            if (strcmp(filePlayer, playerName) == 0) {
                strcpy(records[totalRecords].name, playerName);
                records[totalRecords].difficultyUnlocked = (maxdifficultyunlocked > fileDiff) ? maxdifficultyunlocked : fileDiff;
              int currentMaxLvl = maxlevelunlocked[fileDiff]; 
        records[totalRecords].highestLevel = (currentMaxLvl > fileLevel) ? currentMaxLvl : fileLevel;
                found = true;
            } else {
                strcpy(records[totalRecords].name, filePlayer);
                records[totalRecords].difficultyUnlocked = fileDiff;
                records[totalRecords].highestLevel = fileLevel;
            }
            totalRecords++;
        }
        fclose(f);
    }

    if (!found && strlen(playerName) > 0) {
        strcpy(records[totalRecords].name, playerName);
        records[totalRecords].difficultyUnlocked = maxdifficultyunlocked;
        records[totalRecords].highestLevel = maxlevelunlocked[maxdifficultyunlocked];
        totalRecords++;
    }

    f = fopen("records.txt", "w");
    if (f != NULL) {
        for (int i = 0; i < totalRecords; i++) {
            fprintf(f, "%s %d %d\n", records[i].name, records[i].difficultyUnlocked, records[i].highestLevel);
        }
        fclose(f);
    }
}
void LoadPlayerProgress() {
    FILE *f = fopen("records.txt", "r");
    bool found = false;
    
    if (f != NULL) {
        char filePlayer[30];
        int fileDiff, fileLevel;
        
        while (fscanf(f, "%29s %d %d", filePlayer, &fileDiff, &fileLevel) == 3) {
            if (strcmp(filePlayer, playerName) == 0) {
                maxdifficultyunlocked = fileDiff;
                selecteddiff = fileDiff;
                maxlevelunlocked[fileDiff] = fileLevel; 
                selectedlevel = 1;
                found = true;
                break;
            }
        }
        fclose(f);
    }
    
    if (!found) {
        maxdifficultyunlocked = DIFF_EASY;
        selecteddiff = DIFF_EASY;
        selectedlevel = 1;
        maxlevelunlocked[0] = 1;
        maxlevelunlocked[1] = 1;
        maxlevelunlocked[2] = 1;
    }
}

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
        
score=0;
 GAME_OVER=false;

complete_target=false;
 totalword=0;

 spawntime=0.0f;
 wordlength=0;
   inputword[wordlength]='\0';

 activeglobalpowerup=powerup_none;
 poweruptimer=0.0f;
 speedmodifier=1.0f;
 scoremultiplier=1;
 iswipingdown=false;
 canusebullet=false;
 canusehyper=false;
 canusewarp=false;
 canusewipe=false;



    for(int i=0;i<MAX_WORD_ONSCREEN;i++)
    {
        screenword[i].active=false;
        screenword[i].issliced=false;
        screenword[i].leftposoffset=(Vector2){0.0f,0.0f};
        screenword[i].rightposoffset=(Vector2){0.0f,0.0f};
        screenword[i].slashtimer=0.0f;
        screenword[i].lefthalf[0]='\0';
        screenword[i].righthalf[0]='\0';


        screenword[i].powerupeffect=powerup_none;
    }

    

}



void activatepowerup(poweruptype type)
{
    if(type!=powerup_screen_wipe)
    {
        speedmodifier=1.0f;
        scoremultiplier=1;
    }
    switch(type)
    {
        case powerup_bullet_time:
        {
            speedmodifier=0.5f;
            poweruptimer=10.0f;
            activeglobalpowerup=powerup_bullet_time;
            break;
        }


        case powerup_hyper_speed:
        {
            speedmodifier=2.0f;
            scoremultiplier=2;
            poweruptimer=10.0f;
            activeglobalpowerup=powerup_hyper_speed;
            break;
        }


        case powerup_time_warp:
        {
            timer+=10.0f;
            break;
        }



        case powerup_screen_wipe:
        {
            iswipingdown=true;
            break;
        }




        case powerup_freeze:
        {
            speedmodifier=0.0f;
            poweruptimer=5.0f;
            activeglobalpowerup=powerup_freeze;
            break;
        }



                case powerup_rush:
        {
            speedmodifier=2.5f;
            scoremultiplier=3;
            poweruptimer=5.0f;
            activeglobalpowerup=powerup_rush;
            break;
        }


        case powerup_shock:
        {
            for(int i=0;i<MAX_WORD_ONSCREEN;i++)
            {
                if(screenword[i].active&&screenword[i].position.y>screenheight*0.5f)
                {
                    screenword[i].active=false;

                }
            }
            break;
        }




        case powerup_shrink:
        {
            for(int i=0;i<MAX_WORD_ONSCREEN;i++)
            {
                if(screenword[i].active&&strlen(screenword[i].text)>3)
                {
                    screenword[i].text[3]='\0';
                }
            }
            break;
        }

    }
}


void poweruponspawn(int wordindex)
{
    screenword[wordindex].powerupeffect=powerup_none;

    if(selecteddiff==DIFF_MEDIUM||selecteddiff==DIFF_HARD)
    {
        int chance=GetRandomValue(1,100);
        if(chance<=28)
        {
        int typechoice=GetRandomValue(1,4);

switch(typechoice)

        {
            case 1:screenword[wordindex].powerupeffect=powerup_freeze;break;

           case 2:screenword[wordindex].powerupeffect=powerup_rush;break;
            case 3:screenword[wordindex].powerupeffect=powerup_shock;break;
            case 4:screenword[wordindex].powerupeffect=powerup_shrink;break;


            

        }

    }
}
}


void spawnword(int index)
{
    int i=rand()%totalword;
    strcpy(screenword[index].text,word[i]);
    screenword[index].position.x=GetRandomValue(200,screenwidth-200);
    screenword[index].position.y=GetRandomValue(-150,-40);
    screenword[index].speed=speed;
    screenword[index].active=true;
    screenword[index].lefthalf[0]='\0';
    screenword[index].righthalf[0]='\0';
    poweruponspawn(index);
int fontSize = 22;
    int textWidth = MeasureText(screenword[index].text, fontSize);
    int scrollWidth = textWidth + 30;
    int ninjaSize = 32;
    int spacing = 6;
    
    
    float totalWidth = (float)(ninjaSize + spacing + scrollWidth);
    float boxHeight = (float)(fontSize + 16); 

    int maxAttempts = 50; 
    int attempts = 0;
    bool hasCollision = true;

    while (hasCollision && attempts < maxAttempts)
    {
        
        
        screenword[index].position.x = (float)GetRandomValue(200, screenwidth - 200);
        screenword[index].position.y = (float)GetRandomValue(-150, -40);

        
        
        Rectangle newWordRec = { 
            screenword[index].position.x - (totalWidth / 2.0f), 
            screenword[index].position.y, 
            totalWidth, 
            boxHeight 
        };

        hasCollision = false;

        
        for (int k = 0; k < MAX_WORD_ONSCREEN; k++)
        {
            
            if (k == index || !screenword[k].active || screenword[k].issliced) continue;

            
            int checkTextWidth = MeasureText(screenword[k].text, fontSize);
            float checkTotalWidth = (float)(ninjaSize + spacing + checkTextWidth + 30);

            Rectangle existingWordRec = {
                screenword[k].position.x - (checkTotalWidth / 2.0f),
                screenword[k].position.y,
                checkTotalWidth,
                boxHeight
            };

            
            if (CheckCollisionRecs(newWordRec, existingWordRec))
            {
                hasCollision = true; 
                break; 
            }
        }
        attempts++;
    }
    
}







void updatefallingword(float dt)
{
   timer-=dt;
 spawntime+=dt;



 if(activeglobalpowerup!=powerup_none)
 {
    poweruptimer-=dt;
    if(poweruptimer<=0.0f)
    {
        speedmodifier=1.0f;
        scoremultiplier=1;
        activeglobalpowerup=powerup_none;
    }
 }


 if(canusebullet && IsKeyPressed(KEY_ONE))
 {  
    activatepowerup(powerup_bullet_time);
    canusebullet=false;
 }

  if(canusehyper && IsKeyPressed(KEY_TWO))
 {  
    activatepowerup(powerup_hyper_speed);
    canusehyper=false;
 }
 if(canusewarp && IsKeyPressed(KEY_THREE))
 {  
    activatepowerup(powerup_time_warp);
    canusewarp=false;
 }
 if(canusewipe && IsKeyPressed(KEY_FOUR))
 {  
    activatepowerup(powerup_screen_wipe);
    canusewipe=false;
 }

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

bool activewordleft=false;
    for(int i=0;i<MAX_WORD_ONSCREEN;i++)
    {
        if(!screenword[i].active)
        {
        continue;
        }
        activewordleft=true;
        if(screenword[i].issliced)
        {

            if(screenword[i].slashtimer>=0.0f)
            {
                screenword[i].slashtimer-=dt;
                screenword[i].leftvelocity.y+=600.0f*dt;
                screenword[i].rightvelocity.y+=600.0f*dt;
                screenword[i].leftposoffset.x+=screenword[i].leftvelocity.x*dt;
                screenword[i].leftposoffset.y+=screenword[i].leftvelocity.y*dt;
                screenword[i].rightposoffset.x+=screenword[i].rightvelocity.x*dt;
                screenword[i].rightposoffset.y+=screenword[i].rightvelocity.y*dt;

                float currenty=screenword[i].position.y+screenword[i].leftposoffset.y;
                float currentx=screenword[i].position.x+screenword[i].leftposoffset.x;
                if(currenty>=bottom_line_y||currenty<=-200||currentx<=-200||currentx>screenwidth+200)
                {
                     screenword[i].active=false;
                     screenword[i].issliced=false;

                }
            }
            else
            {
                 screenword[i].active=false;
                     screenword[i].issliced=false;

            }
        }

            
            else
            {

                if(iswipingdown)
                {
                    screenword[i].position.y+=800.0f*dt;
                }

                else
                {
                    screenword[i].position.y+=screenword[i].speed*dt*speedmodifier;

                }
            if(screenword[i].position.y>=bottom_line_y)
            {

                if(iswipingdown)
                {
                    score+=10*scoremultiplier;
                }
                else
                {
                     score-=5;

                }
                screenword[i].active=false;
            }
        }
    }
        if(iswipingdown&&!activewordleft)
        {
            iswipingdown=false;
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
        if(screenword[i].active&&!screenword[i].issliced)
        {
                if(strcmp(screenword[i].text,inputword)==0)
                {


                    if(screenword[i].powerupeffect!=powerup_none)
                    {
                        activatepowerup(screenword[i].powerupeffect);
                    }
                   // screenword[i].active=false;
                   screenword[i].issliced=true;
                    score+=10;
                    PlaySound(slicesound);
                    int wordwidth=MeasureText(screenword[i].text,22);
                    int wordheight=22;
                    screenword[i].slashstart=(Vector2){screenword[i].position.x+wordwidth+10,screenword[i].position.y-5};
                    screenword[i].slashend=(Vector2){screenword[i].position.x-10,screenword[i].position.y+wordheight+5};
                    screenword[i].slashtimer=0.15f;
                    int len=(int)strlen(screenword[i].text);
                    int mid=len/2;
                    strncpy(screenword[i].lefthalf,screenword[i].text,mid);
                    screenword[i].lefthalf[mid]='\0';
                    strcpy(screenword[i].righthalf,screenword[i].text+mid);
                    screenword[i].leftvelocity=(Vector2){-250.0f,-200.0f};
                      screenword[i].rightvelocity=(Vector2){250.0f,-200.0f};
                     screenword[i].leftposoffset=(Vector2){0.0f,0.0f};
                    screenword[i].rightposoffset=(Vector2){0.0f,0.0f};



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


void levelsetting(Difficultymode diff, int level)
{
    initgameplay();
    switch(diff)
    {
        case DIFF_EASY:
        {
            speed = 100.0f;
            canusebullet=true;
            canusehyper=true;
            canusewarp=true;
            canusewipe=true;
            if(level == 1)      { timer = 60.0f; target = 100; }
            else if(level == 2) { timer = 50.0f; target = 150; }
            else                { timer = 40.0f; target = 200; }
            
            break;
        }
        case DIFF_MEDIUM:
        {
            speed = 110.0f;
            if(level == 1)      { timer = 60.0f; target = 150; }
            else if(level == 2) { timer = 50.0f; target = 200; }
            else                { timer = 40.0f; target = 250; }
            break;
        }
        case DIFF_HARD:
        {
            speed = 200.0f;
            if(level == 1)      { timer = 60.0f; target = 200; }
            else if(level == 2) { timer = 50.0f; target = 300; }
            else                { timer = 40.0f; target = 400; }
            break;
        }
    }
    loadword("word.txt");
}





void DrawNinjaHoldingScroll(const char *wordText, int centerX, int y, int fontSize, const char *currentInput,Color scrollcolor)
{
    int inputLen = strlen(currentInput);
    int wordLen = strlen(wordText);
    

    int textWidth = MeasureText(wordText, fontSize);
    int scrollWidth = textWidth + 30;
    int scrollHeight = fontSize + 16;
    
    
    int ninjaSize = 32;
    int spacing = 6;
    int totalWidth = ninjaSize + spacing + scrollWidth;
    
    int startX = centerX - totalWidth / 2;
    int ninjaX = startX;
    int scrollX = startX + ninjaSize + spacing;
    int textY = y + 8;

    //  DRAW THE TINY NINJA HELPER
    int headRadius = 15;
    int headCenterX = ninjaX + headRadius + 3;
    int headCenterY = y + (scrollHeight / 2) - 2;
    
    // A. Draw black outfit head/mask circle
    DrawCircle(headCenterX, headCenterY, headRadius, BLACK);
    
    // B. Draw small skin-toned eyes cutout slot rectangle
    Color ninjaSkin = (Color){ 255, 220, 180, 255 };
    DrawRectangle(headCenterX - 9, headCenterY - 4, 18, 8, ninjaSkin);
    
    // C. Draw two tiny glaring black ninja pupils
    DrawCircle(headCenterX - 4, headCenterY - 1, 2, BLACK);
    DrawCircle(headCenterX + 4, headCenterY - 1, 2, BLACK);
    
    // D. Draw cute little hands overlapping the left edge of the scroll plate
    DrawCircle(scrollX, y +12, 6, BLACK);
    DrawCircle(scrollX, y + scrollHeight - 12, 6, BLACK);

    // DRAW BACKGROUND PAPYRUS CANVAS SHEET 
    Color parchmentColor = (Color){ 242, 222, 179, 255 }; 
    DrawRectangle(scrollX, y, scrollWidth, scrollHeight, scrollcolor);
    DrawRectangleLines(scrollX, y, scrollWidth, scrollHeight, BLACK);

    // DRAW ROLLED WOODEN SCROLL EDGES 
    int handleWidth = 6;
    int handleHeight = scrollHeight + 8;
    Color scrollWoodColor = (Color){ 139, 69, 19, 255 };
    
    Rectangle rightHandle = { scrollX + scrollWidth, y - 4, handleWidth, handleHeight };
    DrawRectangleRounded(rightHandle, 0.4f, 4, scrollWoodColor);
    DrawRectangleRoundedLines(rightHandle, 0.4f, 4, BLACK);

    // DRAW TYPING HIGHLIGHT TEXT ON TOP 
    int textStartX = scrollX + 15;

    bool isMatchingPartially = false;
    if (inputLen > 0 && inputLen <= wordLen) 
    {
        if (strncmp(wordText, currentInput, inputLen) == 0) 
        {
            isMatchingPartially = true;
        }
    }

    if (isMatchingPartially) 
    {
        char matchedPart[MAX_LEN] = { 0 };
        strncpy(matchedPart, wordText, inputLen);
        matchedPart[inputLen]='\0';
        
        const char *remainingPart = wordText + inputLen;

        // Correctly typed letters glow in Maroon Ninja Red
        DrawText(matchedPart, textStartX, textY, fontSize, MAROON);

        int offset = MeasureText(matchedPart, fontSize);
        DrawText(remainingPart, textStartX + offset, textY, fontSize, BLACK);
    }
    else 
    {
        DrawText(wordText, textStartX, textY, fontSize, BLACK);
    }
}




















//Jhil's code
//this is a funtion for drawing rectangle behind every text
/*void DrawButton(const char *text, int centerX, int y,int fontSize, int paddingX, int paddingY)
{
    int textWidth = MeasureText(text, fontSize);

    int rectWidth = textWidth + paddingX * 2;
    int rectHeight = fontSize + paddingY * 2;

    int rectX = centerX - rectWidth / 2;
    //to make the corners round shape
    DrawRectangleRounded((Rectangle){ rectX, y, rectWidth, rectHeight },0.2f,10, WHITE );
    DrawText( text,centerX - textWidth / 2,y + paddingY,fontSize,BLACK);
}
    */
void DrawButton(const char *text, int posX, int posY, int fontSize, int paddingX, int paddingY)
{
    int textWidth = MeasureText(text, fontSize);
    
    // Calculate box dimensions using your padding style
    int boxX = posX - textWidth / 2 - paddingX;
    int boxY = posY - paddingY;
    int boxWidth = textWidth + (paddingX * 2);
    int boxHeight = fontSize + (paddingY * 2);

    // Pick your neon theme color (Electric Cyan)
    Color neonColor = (Color){ 0, 230, 255, 255 }; 

    // 1. Outer Glow Layers (Larger rectangles with low alpha transparency)
    DrawRectangleRounded((Rectangle){boxX - 6, boxY - 6, boxWidth + 12, boxHeight + 12}, 0.4f, 4, (Color){neonColor.r, neonColor.g, neonColor.b, 35});
    DrawRectangleRounded((Rectangle){boxX - 3, boxY - 3, boxWidth + 6, boxHeight + 6}, 0.4f, 4, (Color){neonColor.r, neonColor.g, neonColor.b, 70});

    // 2. Button Body (Dark semi-transparent inner core with a crisp glowing border)
    DrawRectangleRounded((Rectangle){boxX, boxY, boxWidth, boxHeight}, 0.4f, 4, (Color){10, 12, 20, 220});
    
    // Corrected: Removed the thickness argument (2.0f) so it fits Raylib's 4-parameter function signature
    DrawRectangleRoundedLines((Rectangle){boxX, boxY, boxWidth, boxHeight}, 0.4f, 4, neonColor);

    // 3. Crisp text drawn centered on top
    DrawText(text, posX - textWidth / 2, posY, fontSize, WHITE);
}


int main()
{
InitWindow(screenwidth, screenheight, "Typing Ninja");
srand(time(NULL));
SetExitKey(0);
InitAudioDevice();
SetTargetFPS(60);

Texture2D ninjaLogo = LoadTexture("ninja_logo.png");
Texture2D ninjaLogo2 =LoadTexture("ninjalogo2.png");
Texture2D easy = LoadTexture("coverphoto_easy.png");
Texture2D records =LoadTexture("coverphoto_hard.png");
//if(easy.id<=0)
//{
//TraceLog(LOG_WARNING,"EMAGE FAILED TO LOAD");
//}
//printf("Easy image: %d x %d\n", easy.width, easy.height);
slicesound=LoadSound("daviddumaisaudio-sword-slash-with-metallic-impact-185435.mp3");
SetSoundVolume(slicesound,0.8f);
Music backgroundMusic = LoadMusicStream("background_music.mp3");
PlayMusicStream(backgroundMusic);
GameScreen currentScreen = TITLE;

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
    }
    break;
    }
    case MAIN_MENU:
    {Rectangle mutebuttonrec={(float)screenwidth-150,20,120,40};
    if(CheckCollisionPointRec(GetMousePosition(),mutebuttonrec)&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        ismuted = !ismuted;
        if(ismuted)
        {
            SetMusicVolume(backgroundMusic,0.0f);
        }
        else
        {
            SetMusicVolume(backgroundMusic,1.0f);
        }
        //SetMasterVolume(ismuted?0.0f:1.0f);
    }
    if (key == KEY_P)
    {
    nameLength = 0;
   playerName[0] = '\0';
   while(GetCharPressed()>0);
    currentScreen = NAME_ENTRY;
    }
    //new
    else if (key == KEY_R)
    {
        currentScreen = RECORDS;
    }
    else if (key == KEY_H)
    {
    currentScreen = TITLE;
    }
                
    if (key == KEY_BACKSPACE)
    {
    currentScreen = TITLE;
    break;}
    }
    
    //new

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
            LoadPlayerProgress(); 
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
    if (key == KEY_ONE)
    {
        selectedlevel = 1;
        levelsetting(selecteddiff, selectedlevel);
        currentScreen = LEVEL_READY;
    }
    else if (key == KEY_TWO && maxlevelunlocked[selecteddiff] >= 2)
    {
        selectedlevel = 2;
        levelsetting(selecteddiff, selectedlevel);
        currentScreen = LEVEL_READY;
    }
    else if (key == KEY_THREE && maxlevelunlocked[selecteddiff] >= 3)
    {
        selectedlevel = 3;
        levelsetting(selecteddiff, selectedlevel);
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
    if(key==KEY_ENTER)
    {
        currentScreen=GAMEPLAY;
    }
    else if(key==KEY_BACKSPACE)
{
    currentScreen=LEVEL_SELECT;
}
break;
}
    
   case GAMEPLAY:
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        isPaused = !isPaused;
    }
    if (!isPaused)
{
    updatefallingword(deltatime);
    handleplayertyping();
    
    if(timer <= 0.0f && score < target)
    {
        complete_target = false; 
        SaveRecord();            
        currentScreen = GAMEOVER;
        break;                   
    }

    if(score >= target)
    {
        complete_target = true;
        
    
        int nextLevel = selectedlevel + 1;
        if (nextLevel > 3) nextLevel = 3;
        
        if (nextLevel > maxlevelunlocked[selecteddiff]) {
            maxlevelunlocked[selecteddiff] = nextLevel;
        }
        
        if(selecteddiff == DIFF_EASY && selectedlevel == 3)
        {
            if(maxdifficultyunlocked < DIFF_MEDIUM)
            {
                maxdifficultyunlocked = DIFF_MEDIUM;
            }
        }
        else if(selecteddiff == DIFF_MEDIUM && selectedlevel == 3)
        {
            if(maxdifficultyunlocked < DIFF_HARD)
            {
                maxdifficultyunlocked = DIFF_HARD;
            }
        }
        
        SaveRecord();
        currentScreen = GAMEOVER;
        break;
    }
}
    break;
}

case GAMEOVER:
    {
        if (key == KEY_ENTER || key == KEY_ESCAPE)
        {
            currentScreen = LEVEL_SELECT;
        }
        break;
    }

    }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        

        switch (currentScreen)
        {
        case TITLE:
        {//Logo bg for title screen
            DrawTexturePro(
                ninjaLogo2,
        (Rectangle){ 0, 0, (float)ninjaLogo.width, (float)ninjaLogo.height },
         (Rectangle){ 0, 0, (float)screenwidth, (float)screenheight },
        (Vector2){ 0, 0 },0.0f,WHITE
        );
        DrawRectangle(0,0,screenwidth,screenheight,(Color){ 0, 0, 0, 80 });
        break;
        }

        case MAIN_MENU:
        {
            DrawTexturePro(
                ninjaLogo,
                (Rectangle){ 0, 0, (float)ninjaLogo.width, (float)ninjaLogo.height },
                (Rectangle){ 0, 0, (float)screenwidth, (float)screenheight },
                (Vector2){ 0, 0 }, 0.0f, WHITE
            );
            DrawRectangle(0, 0, screenwidth, screenheight, (Color){ 0, 0, 0, 130 });
            DrawButton(ismuted ? "Unmute" : "Mute", screenwidth - 90, 40, 20, 15, 8);
            DrawButton("MAIN MENU", screenwidth / 2, 120, 28, 25, 10);
            DrawButton("[P] Play Game", screenwidth / 2, 210, 28, 25, 10);
        
            DrawButton("[R] View Records", screenwidth / 2, 270, 24, 25, 10);
            
            DrawButton("[X] Exit", screenwidth / 2, 340, 18, 20, 8);
            DrawButton("BACKSPACE : Go Back", screenwidth / 2, 420, 18, 20, 8);
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
                (Vector2){ 0, 0 }, 0.0f, WHITE);
            DrawRectangle(0, 0, screenwidth, screenheight, (Color){ 0, 0, 0, 130 });
            DrawButton("[1] Level 1", screenwidth / 2, 220, 28, 25, 10);
            if (maxlevelunlocked[selecteddiff] >= 2) {
                DrawButton("[2] Level 2", screenwidth / 2, 300, 28, 25, 10);
            } else {
                DrawButton("[2] Level 2 - LOCKED", screenwidth / 2, 300, 28, 25, 10);
            }

            if (maxlevelunlocked[selecteddiff] >= 3) {
                DrawButton("[3] Level 3", screenwidth / 2, 380, 28, 25, 10);
            } else {
                DrawButton("[3] Level 3 - LOCKED", screenwidth / 2, 380, 28, 25, 10);
            }

            DrawButton("press [Backspace] to return", screenwidth / 2, 540, 28, 25, 10);
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
           // DrawLine(0,bottom_line_y,screenwidth,bottom_line_y,RED);
                DrawButton(TextFormat("SCORE: %d",score),80,20,20,10,6);
    DrawButton(TextFormat("TIME: %.1f s",timer),80,60,20,10,6);
    DrawButton(TextFormat("TARGET: %d",target),80,100,20,10,6);
    DrawText("PRESS [ESC] TO PAUSE", screenwidth / 2 - MeasureText("PRESS [ESC] TO PAUSE", 20) / 2, screenheight - 40, 20, DARKGRAY);
    if (isPaused)
    { 
        // Dim the background
        DrawRectangle(0, 0, screenwidth, screenheight, (Color){ 0, 0, 0, 180 });

        // Draw your neon pause menu using your upgraded button function!
        DrawButton("GAME PAUSED", screenwidth / 2, screenheight / 2 - 50, 30, 20, 10);
        DrawButton("PRESS [ESC] TO RESUME", screenwidth / 2, screenheight / 2 + 20, 20, 15, 8);
    }
        for(int i=0;i<MAX_WORD_ONSCREEN;i++)
        {
            if(screenword[i].active)
            {
                if(screenword[i].issliced)
                {

                    int scrolloffsetx=15;
                    int scrolloffsety=8;
                    int leftx=(int)(screenword[i].position.x+scrolloffsetx+screenword[i].leftposoffset.x);
                      int lefty=(int)(screenword[i].position.y+scrolloffsety+screenword[i].leftposoffset.y);
                      DrawText(screenword[i].lefthalf,leftx,lefty,22,RED);
                      int leftwidth=MeasureText(screenword[i].lefthalf,22);
                      int rightx=(screenword[i].position.x+scrolloffsetx+screenword[i].rightposoffset.x);
                    int righty=(screenword[i].position.y+scrolloffsety+screenword[i].rightposoffset.y);
                      DrawText(screenword[i].righthalf,rightx,righty,22,RED);

if (screenword[i].slashtimer>0.0f)
{
    DrawLineEx(screenword[i].slashstart,screenword[i].slashend,6.0f,WHITE);
        DrawLineEx(screenword[i].slashstart,screenword[i].slashend,2.0f,SKYBLUE);

}

  
                }
                else{



                    Color scrollparchmentcolor=(Color){242,222,179,255};

                    if(activeglobalpowerup!=powerup_none)
                    {
                      Color scrollparchmentcolor=(Color){242,222,179,255};

                       // scrollparchmentcolor=WHITE;
                    }

                    else
                    {
                        switch(screenword[i].powerupeffect)
                        {
                            case powerup_freeze:scrollparchmentcolor=BLUE;break;
                            case powerup_rush:scrollparchmentcolor=RED;break;
                            case powerup_shock:scrollparchmentcolor=PURPLE;break;
                            case powerup_shrink:scrollparchmentcolor=GREEN;break;
                        }
                    }
             //DrawButton(screenword[i].text,screenword[i].position.x,screenword[i].position.y,20,15,9);

             DrawNinjaHoldingScroll(screenword[i].text,screenword[i].position.x,screenword[i].position.y,22,inputword,scrollparchmentcolor);
            }

            }
        }


        if(activeglobalpowerup==powerup_bullet_time)
        {
            DrawButton(TextFormat("SLOW MOTION ACTIVE:%.1fs",poweruptimer),screenwidth/2-70,screenheight-100,20,10,5);
            
        }

        else if(activeglobalpowerup==powerup_freeze)
        {
            DrawButton(TextFormat("TIME FROZEN:%.1fs",poweruptimer),screenwidth/2-70,screenheight-100,20,10,5);

        }
          else if(activeglobalpowerup==powerup_hyper_speed||activeglobalpowerup==powerup_rush)
          {
            DrawButton(TextFormat("SCORE RUSH:%.1fs",poweruptimer),screenwidth/2-70,screenheight-100,20,10,5);

          }


          if(selecteddiff==DIFF_EASY)
          {
            DrawButton("ITEMS(1-4:)",100,screenheight-150,20,10,5);
             DrawButton(canusebullet?"1:slow":"used",300,screenheight-150,20,10,5);
            DrawButton(canusehyper?"2:rush":"used",500,screenheight-150,20,10,5);
            DrawButton(canusewarp?"3:warp":"used",700,screenheight-150,20,10,5);
            DrawButton(canusewipe?"4:wipe":"used",900,screenheight-150,20,10,5);

          }


          else
          {
           // DrawText("powerup codes:",50,screenheight-50,20,DARKGRAY);
          DrawText("BLUE:FREEZE",200,screenheight-150,20,BLUE);
            DrawText("RED:RUSH",400,screenheight-150,20,RED);
            DrawText("PURPLE:SHOCK",550,screenheight-150,20,PURPLE);
            DrawText("GREEN:SHRINK",750,screenheight-150,20,GREEN);


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
 DrawText("PRESS [ESCAPE] TO RETURN",screenwidth/2-240,screenheight/2+60,30,RED);

                }
                else
                {
                    DrawText("GAME OVER",screenwidth/2-150,screenheight/2-100,50,RED);
DrawText(TextFormat("SCORE: %d",score),screenwidth/2-100,screenheight/2-20,40,RED);
DrawText("PRESS [ESCAPE] TO RETURN",screenwidth/2-220,screenheight/2+60,30,RED);

                }
             break;  
            }

            //new
           case RECORDS:
        {
            DrawTexturePro(
                records,
                (Rectangle){ 0, 0, (float)ninjaLogo.width, (float)ninjaLogo.height },
                (Rectangle){ 0, 0, (float)screenwidth, (float)screenheight },
                (Vector2){ 0, 0 }, 0.0f, WHITE
            );
            DrawRectangle(0, 0, screenwidth, screenheight, (Color){ 0, 0, 0, 150 });

            // Title & Table Headers matching your sketch layout
            DrawButton("NINJA LEADERBOARD", screenwidth / 2, 70, 30, 25, 10);
            
            DrawText("NAME", 130, 140, 18, LIGHTGRAY);
            DrawText("RANK", 310, 140, 18, LIGHTGRAY);
            DrawText("DIFFICULTY", 500, 140, 18, LIGHTGRAY);
            DrawText("LEVEL", 720, 140, 18, LIGHTGRAY);
            DrawLine(110, 170, 890, 170, LIGHTGRAY);

            PlayerRecord tableRecords[50];
            int totalRecords = 0;

            // Read records from file
            FILE *f = fopen("records.txt", "r");
            if (f != NULL) {
                while (totalRecords < 50 && fscanf(f, "%29s %d %d", tableRecords[totalRecords].name, &tableRecords[totalRecords].difficultyUnlocked, &tableRecords[totalRecords].highestLevel) == 3) {
                    totalRecords++;
                }
                fclose(f);
            }

        
            for (int i = 0; i < totalRecords - 1; i++) {
                for (int j = 0; j < totalRecords - i - 1; j++) {
                    int scoreA = (tableRecords[j].difficultyUnlocked * 3) + tableRecords[j].highestLevel;
                    int scoreB = (tableRecords[j + 1].difficultyUnlocked * 3) + tableRecords[j + 1].highestLevel;
                    if (scoreA < scoreB) {
                        PlayerRecord temp = tableRecords[j];
                        tableRecords[j] = tableRecords[j + 1];
                        tableRecords[j + 1] = temp;
                    }
                }
            }

    
            int yOffset = 190;
            for (int i = 0; i < totalRecords && i < 10; i++) {
                const char* rankStr = GetNinjaRank(tableRecords[i].difficultyUnlocked, tableRecords[i].highestLevel);
                
                const char* diffStr = "EASY";
         if (tableRecords[i].difficultyUnlocked == DIFF_MEDIUM) diffStr = "MEDIUM";
                if (tableRecords[i].difficultyUnlocked == DIFF_HARD) diffStr = "HARD";

                char lvlStr[10];
                sprintf(lvlStr, "%d", tableRecords[i].highestLevel-1);

            
                DrawText(tableRecords[i].name, 130, yOffset, 20, WHITE);
                DrawText(rankStr, 310, yOffset, 20, YELLOW);
                DrawText(diffStr, 500, yOffset, 20, WHITE);
                DrawText(lvlStr, 720, yOffset, 20, WHITE);

                yOffset += 40;
            }

            if (totalRecords == 0) {
                DrawText("No records found yet. Play a game!", screenwidth / 2 - MeasureText("No records found yet. Play a game!", 20) / 2, 300, 20, LIGHTGRAY);
            }

            DrawButton("BACKSPACE : Go Back", screenwidth / 2, 630, 18, 20, 8);

            if (key == KEY_BACKSPACE || key == KEY_ESCAPE)
            {
                currentScreen = MAIN_MENU;
            }
            break;
        }
        }
        
    
        EndDrawing();

    }
    UnloadTexture(easy);
     UnloadTexture(ninjaLogo);
    StopMusicStream(backgroundMusic); 
    UnloadMusicStream(backgroundMusic);
    UnloadSound(slicesound);
    CloseAudioDevice();

    CloseWindow();   

    return 0;

    }