#include "raylib.h"

int main() {
    // 1. Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Physics Engine - v0.1");
    SetTargetFPS(60);

    // 2. The Game Loop
    while (!WindowShouldClose()) { 
        // 3. Drawing
        BeginDrawing();
        
        // A nice dark-mode/cyberpunk background color
        ClearBackground({ 15, 15, 18, 255 }); 

        DrawText("We are cooking.", 500, 350, 30, GREEN);

        EndDrawing();
    }

    // 4. De-Initialization
    CloseWindow(); 
    return 0;
}
