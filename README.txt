==================================================
PROJECT: Typing Ninja
COURSE / LAB: Raylib Project Submission
==================================================

1. ABOUT THE PROJECT:
Typing Ninja is a fast-paced typing arcade game built in C using the Raylib library. 
Players must type words matching falling scrolls to score points, trigger power-ups, 
and survive against a countdown timer.

2. REQUIRED DEPENDENCIES & LIBRARIES:
- Raylib 
- C Compiler (GCC / MinGW / Clang)

3. INSTRUCTIONS FOR COMPILING AND RUNNING:
- The main entry point source file for this project is "main_game.c". Please ensure you are compiling this specific file.
- If using a standard Raylib template setup via terminal:
  Open your terminal in the project directory and run your build command referencing main_game.c (e.g., gcc main_game.c -lraylib -lopengl32 -lgdi32 -lwinmm).
- If using VS Code:
  Open the project folder, open "main_game.c" and use your configured build task or press F5 to compile and run.

4. SPECIAL CONFIGURATION OR SETUP STEPS:
- Ensure all asset files (images, audio files, and records.txt) remain in the exact same directory structure relative to main_game.c so the game can load textures, sounds, and leaderboard data properly.

