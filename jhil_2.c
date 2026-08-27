#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#define screenWidth 1000
#define screenHeight 800
#define MAX_HISTORY 5
#define SAVE_FILE "game_records"


typedef struct {
    int score;
} GameRecord;

typedef struct {
    int highScore;
    int lowScore;
    GameRecord history[MAX_HISTORY];
    int historyCount;
} StatsManager;

void SaveGameData(StatsManager *stats) {
    SaveFileData(SAVE_FILE, stats, sizeof(StatsManager));
}

void LoadGameData(StatsManager *stats) {
 if (FileExists(SAVE_FILE)) {
unsigned int fileSize = 0;
unsigned char *fileData = LoadFileData(SAVE_FILE, &fileSize);
        
 if (fileData != NULL && fileSize == sizeof(StatsManager)) {
        *stats = *(StatsManager *)fileData;
        UnloadFileData(fileData); 
        }
    } 
    else {
        stats->highScore = 0;
        stats->lowScore = 0;
        stats->historyCount = 0;
    }
}

void AddNewScore(StatsManager *stats, int newScore) {
    if (stats->historyCount == 0 || newScore > stats->highScore) {
        stats->highScore = newScore;
    }

    if (stats->historyCount == 0 || newScore < stats->lowScore) {
        stats->lowScore = newScore;
    }

    int shiftLimit = (stats->historyCount < MAX_HISTORY) ? stats->historyCount : MAX_HISTORY - 1;
    for (int i = shiftLimit; i > 0; i--) {
        stats->history[i] = stats->history[i - 1];
    }

    stats->history[0].score = newScore;

    if (stats->historyCount < MAX_HISTORY) {
        stats->historyCount++;
    }

    SaveGameData(stats);
}

int main()
 {
    InitWindow(screenWidth, screenHeight, "Typing Ninja Game Records");

    StatsManager myStats = {0};

    LoadGameData(&myStats);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
    
    if (IsKeyPressed(KEY_ENTER)) {
        int simulatedScore = (rand() % 100) + 1;
        AddNewScore(&myStats, simulatedScore); 
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("TYPING NINJA - PERMANENT RECORDS", 110, 40, 40, BLUE);
        DrawText("Press tap ENTER to add a score", 310, 80, 30, BLUE);

        DrawText(TextFormat("High Score: %d", myStats.highScore), 150, 160, 40, BLUE);
        DrawText(TextFormat("Low Score:  %d", myStats.lowScore),  500, 160, 40, BLUE);

        DrawText("Previous Records:", 150, 250, 40, BLUE);
        
        if (myStats.historyCount == 0) {
            DrawText("No games played yet!", 150, 250, 40, BLUE);
        } else {
            for (int i = 0; i < myStats.historyCount; i++) {
          DrawText(TextFormat("Game %d:  %d points", i + 1, myStats.history[i].score), 
                         150, 400 + (i * 30), 30, BLUE);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}