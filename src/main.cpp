#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "PhysicsWorld.hpp"
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

void LoadScene(int sceneID, PhysicsWorld& world) {
    world.particles.clear();
    world.springs.clear();
    world.pulleys.clear();

    int screenWidth = 1280;

    if (sceneID == 0) {
        // Sandbox
        world.particles.push_back(Particle(screenWidth / 2.0f, 100.0f, 1.0f, 15.0f, 0xFFFFFFFF)); // Anchor
        world.particles[0].isStatic = true;
        world.particles.push_back(Particle(screenWidth / 2.0f + 100.0f, 300.0f, 1.0f, 15.0f, 0xFFFFFFFF)); // Bob
        world.springs.push_back(Spring(0, 1, 200.0f, 250.0f, 15.0f));
    } else if (sceneID == 1) {
        // Atwood Machine
        world.particles.push_back(Particle(screenWidth / 2.0f, 100.0f, 0.0f, 20.0f, 0x888888FF)); // Anchor
        world.particles[0].isStatic = true;
        world.particles.push_back(Particle(screenWidth / 2.0f - 150.0f, 250.0f, 10.0f, 15.0f, 0xFF0000FF)); // Heavy
        world.particles.push_back(Particle(screenWidth / 2.0f + 150.0f, 250.0f, 2.0f, 15.0f, 0x0000FFFF)); // Light
        float distA = glm::distance(world.particles[1].position, world.particles[0].position);
        float distB = glm::distance(world.particles[2].position, world.particles[0].position);
        world.pulleys.push_back(Pulley(1, 2, 0, distA + distB, 500.0f, 20.0f));
    } else if (sceneID == 2) {
        // Block Tower
        for (int i = 0; i < 10; i++) {
            uint32_t randomColor = ((GetRandomValue(50, 255) << 24) | (GetRandomValue(50, 255) << 16) | (GetRandomValue(50, 255) << 8) | 0xFF);
            world.particles.push_back(Particle(screenWidth / 2.0f + GetRandomValue(-2, 2), 600.0f - i * 42.0f, 2.0f, 40.0f, 40.0f, randomColor));
        }
    }
}

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Physics Engine - v0.2");
    SetTargetFPS(60);

    PhysicsWorld world((float)screenHeight, 0.0f, (float)screenWidth);
    EngineState state;
    
    LoadScene(state.currentScene, world);

    int grabbedParticleIndex = -1;
    float accumulator = 0.0f;
    const float FIXED_TIME_STEP = 1.0f / 120.0f;

    while (!WindowShouldClose()) { 
        float mouseX = (float)GetMouseX();
        float mouseY = (float)GetMouseY();
        glm::vec2 mousePos(mouseX, mouseY);
        
        Rectangle uiBounds = { 10, 10, 240, 360 }; // UI Panel Area

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !state.sceneMenuOpen && !CheckCollisionPointRec(GetMousePosition(), uiBounds)) {
            bool grabbed = false;
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

            if (!grabbed) {
                uint32_t randomColor = ((GetRandomValue(50, 255) << 24) | (GetRandomValue(50, 255) << 16) | (GetRandomValue(50, 255) << 8)  | 0xFF);
                if (state.spawnShape == 0) {
                    world.particles.push_back(Particle(mouseX, mouseY, 1.0f, 15.0f, randomColor));
                } else {
                    world.particles.push_back(Particle(mouseX, mouseY, 1.0f, 30.0f, 30.0f, randomColor));
                }
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && grabbedParticleIndex != -1) {
            world.particles[grabbedParticleIndex].position = mousePos;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && grabbedParticleIndex != -1) {
            // If it's a scene anchor, don't un-static it unless we want to
            if (grabbedParticleIndex != 0 || state.currentScene == 2) {
                world.particles[grabbedParticleIndex].isStatic = false;
            }
            grabbedParticleIndex = -1;
        }

        // The Tick
        float frameTime = GetFrameTime();
        if (frameTime > 0.25f) frameTime = 0.25f;
        accumulator += frameTime;

        while (accumulator >= FIXED_TIME_STEP) {
            world.Update(FIXED_TIME_STEP, state);
            accumulator -= FIXED_TIME_STEP;
        }

        // 4. Drawing
        BeginDrawing();
        ClearBackground({ 15, 15, 18, 255 }); 

        for (const auto& p : world.particles) {
            if (p.shape == CIRCLE) {
                DrawCircle((int)p.position.x, (int)p.position.y, p.radius, GetRaylibColor(p.color));
            } else if (p.shape == BOX) {
                DrawRectangle((int)(p.position.x - p.width / 2.0f), (int)(p.position.y - p.height / 2.0f), (int)p.width, (int)p.height, GetRaylibColor(p.color));
            }
        }

        if (state.showSprings) {
            for (const auto& spring : world.springs) {
                Vector2 p1 = { world.particles[spring.particleA].position.x, world.particles[spring.particleA].position.y };
                Vector2 p2 = { world.particles[spring.particleB].position.x, world.particles[spring.particleB].position.y };
                DrawLineEx(p1, p2, 3.0f, GREEN);
            }

            for (const auto& pulley : world.pulleys) {
                Vector2 pA = { world.particles[pulley.particleA].position.x, world.particles[pulley.particleA].position.y };
                Vector2 pB = { world.particles[pulley.particleB].position.x, world.particles[pulley.particleB].position.y };
                Vector2 pAnc = { world.particles[pulley.anchor].position.x, world.particles[pulley.anchor].position.y };
                
                DrawCircleV(pAnc, 25.0f, GRAY);
                DrawLineEx(pA, pAnc, 3.0f, WHITE);
                DrawLineEx(pB, pAnc, 3.0f, WHITE);
            }
        }

        // --- UI PANEL ---
        DrawRectangleRec(uiBounds, Fade(BLACK, 0.8f));
        DrawText("ENGINE CONTROLS", 30, 20, 20, WHITE);

        GuiCheckBox({ 30, 60, 20, 20 }, "Enable Gravity", &state.enableGravity);
        GuiCheckBox({ 30, 100, 20, 20 }, "Enable Collisions", &state.enableCollisions);
        GuiCheckBox({ 30, 140, 20, 20 }, "Show Strings/Springs", &state.showSprings);

        DrawText(TextFormat("Gravity: %.1f", state.gravityValue), 30, 180, 10, LIGHTGRAY);
        GuiSlider({ 30, 200, 120, 20 }, "0", "2000", &state.gravityValue, 0.0f, 2000.0f);

        DrawText("Spawn Shape:", 30, 240, 10, LIGHTGRAY);
        GuiToggleGroup({ 30, 260, 60, 25 }, "CIRCLE;BOX", &state.spawnShape);

        if (GuiButton({ 30, 300, 180, 30 }, "CLEAR WORLD")) {
            world.particles.clear();
            world.springs.clear();
            world.pulleys.clear();
        }

        // Scene Dropdown (Draw Last so it overlaps everything)
        if (GuiDropdownBox({ 30, 340, 180, 25 }, "Sandbox;Atwood Machine;Block Tower", &state.currentScene, state.sceneMenuOpen)) {
            state.sceneMenuOpen = !state.sceneMenuOpen;
            if (!state.sceneMenuOpen) { // Selection made
                LoadScene(state.currentScene, world);
            }
        }

        DrawText(TextFormat("Particles: %i", (int)world.particles.size()), 10, screenHeight - 40, 20, RAYWHITE);
        DrawText(TextFormat("FPS: %i", GetFPS()), 10, screenHeight - 20, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}
