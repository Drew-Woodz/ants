// src/main.cpp
#include "raylib.h"
#include "raymath.h"
#include "PheromoneGrid.h"
#include "Ant.h"
#include "AntConfig.h"

int main()
{
    // ------------------------------------------------------------------------------------
    // Fixed timestep – perfect reproducibility + capped CPU
    // ------------------------------------------------------------------------------------
    const float fixedTimestep = 1.0f / 60.0f;
    float accumulator = 0.0f;

    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Myrmidon α – Pheromone Grid MVP");
    SetWindowState(FLAG_WINDOW_HIGHDPI);   // ← Critical for correct mouse on high-DPI displays
    SetTargetFPS(144);                     // Smooth but not insane

    // ------------------------------------------------------------------------------------
    // Camera – Option A: zero-offset, top-left = world (0,0)
    // ------------------------------------------------------------------------------------
    Camera2D camera = { 0 };
    camera.offset   = { 0.0f, 0.0f };                                    // ← Top-left of screen = world origin
    camera.target   = { 512.0f - (screenWidth / 2.0f), 512.0f - (screenHeight / 2.0f) };  // Initial center view
    camera.rotation = 0.0f;
    camera.zoom     = 1.0f;

    // ------------------------------------------------------------------------------------
    // Systems
    // ------------------------------------------------------------------------------------
    PheromoneGrid foodGrid;
    PheromoneGrid homeGrid;

    AntConfig antCfg = AntConfig::load();
    std::vector<Ant> ants(10);
    Texture2D antTex = LoadTexture("assets/ant.png");

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

        // WASD panning
        float panSpeed = 400.0f * frameTime / camera.zoom;
        if (IsKeyDown(KEY_A)) camera.target.x -= panSpeed;
        if (IsKeyDown(KEY_D)) camera.target.x += panSpeed;
        if (IsKeyDown(KEY_W)) camera.target.y -= panSpeed;
        if (IsKeyDown(KEY_S)) camera.target.y += panSpeed;

        // Mouse-wheel zoom centered on cursor
        if (GetMouseWheelMove() != 0.0f)
        {
            Vector2 mouseWorldBefore = GetScreenToWorld2D(GetMousePosition(), camera);

            camera.zoom += GetMouseWheelMove() * 0.3f * camera.zoom;
            camera.zoom = fmaxf(0.1f, camera.zoom);

            Vector2 mouseWorldAfter = GetScreenToWorld2D(GetMousePosition(), camera);
            camera.target = Vector2Add(camera.target, Vector2Subtract(mouseWorldBefore, mouseWorldAfter));
        }

        // -------------------------- Fixed-timestep Update --------------------------
        while (accumulator >= fixedTimestep)
        {
            for (auto& ant : ants)
                ant.update(fixedTimestep, foodGrid, homeGrid, antCfg);

            accumulator -= fixedTimestep;
        }

        // Visual effects (diffusion) – once per rendered frame
        foodGrid.update();
        homeGrid.update();

        // -------------------------- Rendering --------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Sand background
        DrawRectangle(0, 0, screenWidth, screenHeight, Color{194, 154, 94, 255});

        BeginMode2D(camera);

        // ────────────────────── PHEROMONE PAINTING (WORLD SPACE) ──────────────────────
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
        {
            Vector2 mouse    = GetMousePosition();
            Vector2 worldPos = GetScreenToWorld2D(mouse, camera);

            int wx = (int)worldPos.x;
            int wy = (int)worldPos.y;

            bool left  = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
            bool right = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);

            PheromoneGrid& target   = left ? foodGrid : homeGrid;
            float          strength = left ? 12.0f : 10.0f;

            for (int y = -20; y <= 20; ++y)
                for (int x = -20; x <= 20; ++x)
                    if (x*x + y*y < 400)
                        target.add_pheromone(wx + x, wy + y, strength);
        }

        // Draw ants
        for (const auto& ant : ants)
            ant.draw(camera, antTex);

        // Debug overlays
        if (showFood) foodGrid.draw_debug(camera, RED);
        if (showHome) homeGrid.draw_debug(camera, BLUE);

        EndMode2D();

        // UI overlay
        DrawFPS(10, 10);
        DrawText("LMB = Food (red) • RMB = Home (blue) • WASD + Wheel • F1/F2 toggle • 10 ants", 10, 40, 20, DARKGRAY);
        DrawText(TextFormat("Ants: %d | Logic ticks/frame: %.1f | FPS: %d", 
                            (int)ants.size(), frameTime / fixedTimestep, GetFPS()), 10, 70, 20, DARKGRAY);

        EndDrawing();
    }

    UnloadTexture(antTex);
    CloseWindow();
    return 0;
}