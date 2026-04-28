#include "raylib.h"
#include "Particle.h"
#include "Spring.h"
#include <vector>
#include <cstdint>
#include <iostream>

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
    std::vector<Spring> springs;

    // The Anchor Setup (The Sandbox)
    particles.push_back(Particle(screenWidth / 2.0f, 100.0f, 1.0f, 15.0f, 0xFFFFFFFF)); // Particle 0: Anchor
    particles[0].isStatic = true;
    particles.push_back(Particle(screenWidth / 2.0f + 100.0f, 300.0f, 1.0f, 15.0f, 0xFFFFFFFF)); // Particle 1: Bob
    springs.push_back(Spring(0, 1, 200.0f, 250.0f, 15.0f));

    int grabbedParticleIndex = -1;

    // 2. The Game Loop
    while (!WindowShouldClose()) { 
        // 3. The God Hand (Input)
        float mouseX = (float)GetMouseX();
        float mouseY = (float)GetMouseY();
        glm::vec2 mousePos(mouseX, mouseY);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            bool grabbed = false;
            // Check if clicking on an existing particle
            for (size_t i = 0; i < particles.size(); i++) {
                if (glm::distance(particles[i].position, mousePos) <= particles[i].radius) {
                    grabbedParticleIndex = (int)i;
                    particles[i].isStatic = true;
                    particles[i].velocity = glm::vec2(0.0f, 0.0f);
                    grabbed = true;
                    break;
                }
            }

            // Spawn on Click if hitting empty space
            if (!grabbed) {
                uint32_t randomColor = ((GetRandomValue(50, 255) << 24) | 
                                        (GetRandomValue(50, 255) << 16) | 
                                        (GetRandomValue(50, 255) << 8)  | 0xFF);
                
                particles.push_back(Particle(mouseX, mouseY, 1.0f, (float)GetRandomValue(10, 25), randomColor));
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && grabbedParticleIndex != -1) {
            particles[grabbedParticleIndex].position = mousePos;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && grabbedParticleIndex != -1) {
            // Particle 0 is the anchor, so it stays static
            if (grabbedParticleIndex != 0) {
                particles[grabbedParticleIndex].isStatic = false;
            }
            grabbedParticleIndex = -1;
        }

        // The Tick
        float deltaTime = GetFrameTime();

        // Apply Spring Forces
        for (auto& s : springs) {
            s.Update(particles);
        }

        // Update positions
        for (auto& p : particles) {
            p.Update(deltaTime, screenWidth, screenHeight);
        }

        // Then check collisions
        for (size_t i = 0; i < particles.size(); i++) {
            for (size_t j = i + 1; j < particles.size(); j++) {
                particles[i].ResolveCollision(particles[j]);
            }
        }

        // 4. Drawing
        BeginDrawing();
        
        // A nice dark-mode/cyberpunk background color
        ClearBackground({ 15, 15, 18, 255 }); 

        // Draw every particle
        for (const auto& p : particles) {
            DrawCircle((int)p.position.x, (int)p.position.y, p.radius, GetRaylibColor(p.color));
        }

        // Draw every spring
        for (const auto& spring : springs) {
            Vector2 p1 = { particles[spring.particleA].position.x, particles[spring.particleA].position.y };
            Vector2 p2 = { particles[spring.particleB].position.x, particles[spring.particleB].position.y };
            DrawLineEx(p1, p2, 3.0f, GREEN); // Neon green line
        }

        // Debug info on screen
        DrawText(TextFormat("Particles: %i", (int)particles.size()), 10, 10, 20, RAYWHITE);
        DrawText(TextFormat("FPS: %i", GetFPS()), 10, 40, 20, RAYWHITE);

        EndDrawing();
    }

    // 5. De-Initialization
    CloseWindow(); 
    return 0;
}
