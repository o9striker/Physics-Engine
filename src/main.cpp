#include "raylib.h"
#include "PhysicsWorld.h"
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

    // The PhysicsWorld owns all particles, springs, and boundary constraints
    PhysicsWorld world((float)screenHeight, 0.0f, (float)screenWidth);

    // The Pulley Setup (Atwood Machine Demo)
    world.particles.push_back(Particle(screenWidth / 2.0f, 100.0f, 1.0f, 15.0f, 0xFFFFFFFF)); // Particle 0: Anchor
    world.particles[0].isStatic = true;
    
    world.particles.push_back(Particle(screenWidth / 2.0f - 100.0f, 300.0f, 5.0f, 25.0f, 0xEE4444FF)); // Particle 1: Heavy Weight (Red)
    world.particles.push_back(Particle(screenWidth / 2.0f + 100.0f, 300.0f, 1.0f, 15.0f, 0x44EE44FF)); // Particle 2: Light Weight (Green)
    
    // Pulley: pA=1, pB=2, anchor=0, length=600, stiffness=500, damping=20
    world.pulleys.push_back(Pulley(1, 2, 0, 600.0f, 500.0f, 20.0f));


    int grabbedParticleIndex = -1;

    // Time Step variables
    float accumulator = 0.0f;
    const float FIXED_TIME_STEP = 1.0f / 120.0f;

    // 2. The Game Loop
    while (!WindowShouldClose()) { 
        // 3. The God Hand (Input)
        float mouseX = (float)GetMouseX();
        float mouseY = (float)GetMouseY();
        glm::vec2 mousePos(mouseX, mouseY);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            bool grabbed = false;
            // Check if clicking on an existing particle
            for (size_t i = 0; i < world.particles.size(); i++) {
                bool hit = false;
                if (world.particles[i].shape == CIRCLE) {
                    if (glm::distance(world.particles[i].position, mousePos) <= world.particles[i].radius) hit = true;
                } else if (world.particles[i].shape == BOX) {
                    if (mouseX >= world.particles[i].position.x - world.particles[i].width / 2.0f &&
                        mouseX <= world.particles[i].position.x + world.particles[i].width / 2.0f &&
                        mouseY >= world.particles[i].position.y - world.particles[i].height / 2.0f &&
                        mouseY <= world.particles[i].position.y + world.particles[i].height / 2.0f) {
                        hit = true;
                    }
                }
                
                if (hit) {
                    grabbedParticleIndex = (int)i;
                    world.particles[i].isStatic = true;
                    world.particles[i].velocity = glm::vec2(0.0f, 0.0f);
                    grabbed = true;
                    break;
                }
            }

            // Spawn on Click if hitting empty space
            if (!grabbed) {
                uint32_t randomColor = ((GetRandomValue(50, 255) << 24) | 
                                        (GetRandomValue(50, 255) << 16) | 
                                        (GetRandomValue(50, 255) << 8)  | 0xFF);
                
                if (GetRandomValue(0, 1) == 0) {
                    world.particles.push_back(Particle(mouseX, mouseY, 1.0f, (float)GetRandomValue(10, 25), randomColor));
                } else {
                    world.particles.push_back(Particle(mouseX, mouseY, 1.0f, (float)GetRandomValue(20, 50), (float)GetRandomValue(20, 50), randomColor));
                }
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && grabbedParticleIndex != -1) {
            world.particles[grabbedParticleIndex].position = mousePos;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && grabbedParticleIndex != -1) {
            // Particle 0 is the anchor, so it stays static
            if (grabbedParticleIndex != 0) {
                world.particles[grabbedParticleIndex].isStatic = false;
            }
            grabbedParticleIndex = -1;
        }

        // The Tick
        float frameTime = GetFrameTime();
        if (frameTime > 0.25f) frameTime = 0.25f; // Prevent spiral of death
        accumulator += frameTime;

        while (accumulator >= FIXED_TIME_STEP) {
            world.Update(FIXED_TIME_STEP);
            accumulator -= FIXED_TIME_STEP;
        }

        // 4. Drawing
        BeginDrawing();
        
        // A nice dark-mode/cyberpunk background color
        ClearBackground({ 15, 15, 18, 255 }); 

        // Draw every particle
        for (const auto& p : world.particles) {
            if (p.shape == CIRCLE) {
                DrawCircle((int)p.position.x, (int)p.position.y, p.radius, GetRaylibColor(p.color));
            } else if (p.shape == BOX) {
                DrawRectangle((int)(p.position.x - p.width / 2.0f), (int)(p.position.y - p.height / 2.0f), (int)p.width, (int)p.height, GetRaylibColor(p.color));
            }
        }

        // Draw every spring
        for (const auto& spring : world.springs) {
            Vector2 p1 = { world.particles[spring.particleA].position.x, world.particles[spring.particleA].position.y };
            Vector2 p2 = { world.particles[spring.particleB].position.x, world.particles[spring.particleB].position.y };
            DrawLineEx(p1, p2, 3.0f, GREEN); // Neon green line
        }

        // Draw every pulley
        for (const auto& pulley : world.pulleys) {
            Vector2 pA = { world.particles[pulley.particleA].position.x, world.particles[pulley.particleA].position.y };
            Vector2 pB = { world.particles[pulley.particleB].position.x, world.particles[pulley.particleB].position.y };
            Vector2 pAnc = { world.particles[pulley.anchor].position.x, world.particles[pulley.anchor].position.y };
            
            DrawLineEx(pA, pAnc, 2.0f, GRAY);
            DrawLineEx(pB, pAnc, 2.0f, GRAY);
            DrawCircleV(pAnc, 20.0f, DARKGRAY); // The Wheel
            DrawCircleLines((int)pAnc.x, (int)pAnc.y, 20.0f, LIGHTGRAY);
        }


        // Debug info on screen
        DrawText(TextFormat("Particles: %i", (int)world.particles.size()), 10, 10, 20, RAYWHITE);
        DrawText(TextFormat("FPS: %i", GetFPS()), 10, 40, 20, RAYWHITE);

        EndDrawing();
    }

    // 5. De-Initialization
    CloseWindow(); 
    return 0;
}
