#include "raylib.h"
#include "Particle.h"
#include <iostream>

int main() {
    // 1. Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Physics Engine - v0.1");
    SetTargetFPS(60);

    // Spawn the Entity
    Particle p(screenWidth / 2.0f, 100.0f, 15.0f, 1.0f);

    int frameCount = 0;

    // 2. The Game Loop
    while (!WindowShouldClose()) { 
        // The Tick
        float deltaTime = GetFrameTime();

        // Update the Math
        p.Update(deltaTime, screenHeight);

        // 3. Drawing
        BeginDrawing();
        
        // A nice dark-mode/cyberpunk background color
        ClearBackground({ 15, 15, 18, 255 }); 

        // Draw the Entity
        DrawCircle((int)p.position.x, (int)p.position.y, p.radius, MAGENTA);

        // Debug info on screen
        DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, RAYWHITE);
        DrawText(TextFormat("Pos Y: %.2f", p.position.y), 10, 40, 20, RAYWHITE);

        EndDrawing();

        frameCount++;
        if (frameCount < 10) {
            std::cout << "Frame " << frameCount << " rendered at Pos Y: " << p.position.y << std::endl;
        }
    }

    // 4. De-Initialization
    CloseWindow(); 
    return 0;
}
