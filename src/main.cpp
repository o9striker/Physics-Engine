#include "raylib.h"
#include "Particle.h"
#include <vector>
#include <cstdint>

// Helper function to keep physics fully decoupled from Raylib
Color GetRaylibColor(uint32_t hexColor) {
    Color color;
    color.r = (hexColor >> 24) & 0xFF;
    color.g = (hexColor >> 16) & 0xFF;
    color.b = (hexColor >> 8) & 0xFF;
    color.a = hexColor & 0xFF;
    return color;
}

int main() {
    // 1. Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Physics Engine - v0.1");
    SetTargetFPS(60);

    // The dynamic list of particles
    std::vector<Particle> particles;

    // 2. The Game Loop
    while (!WindowShouldClose()) { 
        // 3. The God Hand (Input)
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Spawn on Click
            float mouseX = (float)GetMouseX();
            float mouseY = (float)GetMouseY();
            
            // Generate a random hex color (RRGGBBAA format, with alpha = FF)
            uint32_t randomColor = ((GetRandomValue(50, 255) << 24) | 
                                    (GetRandomValue(50, 255) << 16) | 
                                    (GetRandomValue(50, 255) << 8)  | 0xFF);
            
            particles.push_back(Particle(mouseX, mouseY, 1.0f, GetRandomValue(10, 25), randomColor));
        }

        // The Tick
        float deltaTime = GetFrameTime();

        // Update the Math for every particle
        for (auto& p : particles) {
            p.Update(deltaTime);

            // Floor Collision
            if (p.position.y + p.radius >= screenHeight) {
                p.position.y = screenHeight - p.radius;
                p.velocity.y *= -0.8f; // Bounce with restitution
            }
        }

        // 4. Drawing
        BeginDrawing();
        
        // A nice dark-mode/cyberpunk background color
        ClearBackground({ 15, 15, 18, 255 }); 

        // Draw every particle
        for (const auto& p : particles) {
            DrawCircle(p.position.x, p.position.y, p.radius, GetRaylibColor(p.color));
        }

        EndDrawing();
    }

    // 5. De-Initialization
    CloseWindow(); 
    return 0;
}
