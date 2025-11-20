#include "raylib.h"
#include "PheromoneGrid.h"

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Myrmidon α – Pheromone Grid MVP");
    SetTargetFPS(60);

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.target = { 512.0f, 512.0f };  // center on world

    PheromoneGrid foodGrid;
    PheromoneGrid homeGrid;

    Texture2D ant = LoadTexture("assets/ant.png");

    bool showFood = true;
    bool showHome = true;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F1)) showFood = !showFood;
        if (IsKeyPressed(KEY_F2)) showHome = !showHome;

        // Mouse painting (hold left = food, right = home)
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
            homeGrid.add_pheromone((int)worldPos.x, (int)worldPos.y, 10.0f);
        }

        // Camera controls (future-proof!)
        if (IsKeyDown(KEY_A)) camera.target.x -= 400 * GetFrameTime() / camera.zoom;
        if (IsKeyDown(KEY_D)) camera.target.x += 400 * GetFrameTime() / camera.zoom;
        if (IsKeyDown(KEY_W)) camera.target.y -= 400 * GetFrameTime() / camera.zoom;
        if (IsKeyDown(KEY_S)) camera.target.y += 400 * GetFrameTime() / camera.zoom;
        camera.zoom += GetMouseWheelMove() * 0.3f * camera.zoom;
        camera.zoom = fmaxf(0.1f, camera.zoom);

        foodGrid.update();
        homeGrid.update();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        constexpr Color SAND_COLOR{194, 154, 94, 255};
        DrawRectangle(0, 0, screenWidth, screenHeight, SAND_COLOR);

        BeginMode2D(camera);   // ← THIS WAS MISSING

            if (showFood) foodGrid.draw_debug(camera, RED);
            if (showHome) homeGrid.draw_debug(camera, BLUE);

            // Placeholder ant in world center (512,512)
            DrawTexturePro(ant,
                Rectangle{0,0,(float)ant.width,(float)ant.height},
                Rectangle{512, 512, 64, 64}, Vector2{32,32}, 0.0f, WHITE);

        EndMode2D();           // ← AND THIS

        // UI overlay is always screen-space
        DrawFPS(10, 10);
        DrawText("Hold LMB/RMB to paint • WASD + Wheel • F1/F2 toggle", 10, 40, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}