// src/main.cpp
#include "raylib.h"
#include "PheromoneGrid.h"
#include "Ant.h"
#include "AntConfig.h"

int main()
{
    // ------------------------------------------------------------------------------------
    // Fixed timestep – perfect reproducibility + capped CPU
    // ------------------------------------------------------------------------------------
    float fixedTimestep = 1.0f / 60.0f;
    float accumulator = 0.0f;

    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Myrmidon α – Pheromone Grid MVP");
    SetTargetFPS(60);  // We drive the frame rate ourselves for smoothness

    // ------------------------------------------------------------------------------------
    // Camera – world space centered on 1024×1024 grid
    // ------------------------------------------------------------------------------------
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.target = { 512.0f, 512.0f };  // center of the world

    // ------------------------------------------------------------------------------------
    // Systems
    // ------------------------------------------------------------------------------------
    PheromoneGrid foodGrid;   // red  – ants follow when searching
    PheromoneGrid homeGrid;   // blue – ants follow when returning

    AntConfig antCfg = AntConfig::load();                 // data-driven from config/ants.json
    std::vector<Ant> ants(10);                            // start with 10 → instant highways
    Texture2D antTex = LoadTexture("assets/ant.png");    // 64×64 sprite

    bool showFood = true;
    bool showHome = true;

    // ------------------------------------------------------------------------------------
    // Main loop
    // ------------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {
        float frameTime = GetFrameTime();
        accumulator += frameTime;

        // -------------------------- Input & Camera --------------------------
        if (IsKeyPressed(KEY_F1)) showFood = !showFood;
        if (IsKeyPressed(KEY_F2)) showHome = !showHome;

        // Mouse painting (world space!)
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            int wx = (int)worldPos.x;
            int wy = (int)worldPos.y;
            for (int y = -20; y <= 20; ++y)
                for (int x = -20; x <= 20; ++x)
                    if (x*x + y*y < 400)
                        foodGrid.add_pheromone(wx + x, wy + y, 8.0f);
        }
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
        {
            Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            int wx = (int)worldPos.x;
            int wy = (int)worldPos.y;
            for (int y = -20; y <= 20; ++y)
                for (int x = -20; x <= 20; ++x)
                    if (x*x + y*y < 400)
                        homeGrid.add_pheromone(wx + x, wy + y, 8.0f);
        }

        // Camera controls
        if (IsKeyDown(KEY_A)) camera.target.x -= 400 * frameTime / camera.zoom;
        if (IsKeyDown(KEY_D)) camera.target.x += 400 * frameTime / camera.zoom;
        if (IsKeyDown(KEY_W)) camera.target.y -= 400 * frameTime / camera.zoom;
        if (IsKeyDown(KEY_S)) camera.target.y += 400 * frameTime / camera.zoom;
        camera.zoom += GetMouseWheelMove() * 0.3f * camera.zoom;
        camera.zoom = fmaxf(0.1f, camera.zoom);

        // -------------------------- Fixed-timestep Update --------------------------
        while (accumulator >= fixedTimestep)
        {
            for (auto& ant : ants)
                ant.update(fixedTimestep, foodGrid, homeGrid, antCfg);

            accumulator -= fixedTimestep;
        }
        foodGrid.update();
        homeGrid.update();

        // -------------------------- Rendering --------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Sand background (screen space)
        DrawRectangle(0, 0, screenWidth, screenHeight, Color{194, 154, 94, 255});

        BeginMode2D(camera);   // ← THIS IS CRUCIAL – everything below is world space

        // Draw ants
        for (const auto& ant : ants)
            ant.draw(camera, antTex);

        // Debug pheromone overlays
        if (showFood)  foodGrid.draw_debug(camera, RED);
        if (showHome)  homeGrid.draw_debug(camera, BLUE);

        EndMode2D();

        // Screen-space UI
        DrawFPS(10, 10);
        DrawText("LMB = Food (red) • RMB = Home (blue) • WASD + Wheel • F1/F2 toggle • 10 ants active", 10, 40, 20, DARKGRAY);
        DrawText(TextFormat("Ants: %d | Logic ticks this frame: %.1f | FPS: %d", 
                    (int)ants.size(), 
                    frameTime / fixedTimestep, 
                    GetFPS()), 10, 70, 20, DARKGRAY);
        EndDrawing();
    }

    UnloadTexture(antTex);
    CloseWindow();
    return 0;
}