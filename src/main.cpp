#include "raylib.h"
#include "Particle.h"

int main() {
    // 1. Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Physics Engine - v0.1");
    SetTargetFPS(60);

    // Spawn the Entity
    Particle p(screenWidth / 2.0f, 100.0f, 1.0f);
    const float particleRadius = 15.0f;

    // 2. The Game Loop
    while (!WindowShouldClose()) { 
        // The Tick
        float deltaTime = GetFrameTime();

        // Update the Math
        p.Update(deltaTime);

        // Floor Collision
        if (p.position.y + particleRadius >= screenHeight) {
            p.position.y = screenHeight - particleRadius;
            p.velocity.y *= -0.8f; // Bounce with restitution
        }

        // 3. Drawing
        BeginDrawing();
        
        // A nice dark-mode/cyberpunk background color
        ClearBackground({ 15, 15, 18, 255 }); 


        // Draw the Entity
        DrawCircle(p.position.x, p.position.y, particleRadius, MAGENTA);

        EndDrawing();
    }

    // 4. De-Initialization
    CloseWindow(); 
    return 0;
}
