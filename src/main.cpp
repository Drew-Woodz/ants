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
        // Mouse painting (hold left = food, right = home)
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            foodGrid.add_pheromone((int)worldPos.x, (int)worldPos.y, 10.0f);
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

        // Sand background (still there!)
        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){194, 154, 94, 255});

        BeginMode2D(camera);

        if (showFood) foodGrid.draw_debug(camera, RED);
        if (showHome) homeGrid.draw_debug(camera, BLUE);

        // Placeholder ant in center
        DrawTexturePro(ant, Rectangle{0,0,(float)ant.width,(float)ant.height},
                       Rectangle{512, 512, 64, 64}, Vector2{32,32}, 0.0f, WHITE);

        EndMode2D();

        DrawFPS(10, 10);
        DrawText("Hold LMB/RMB to paint • WASD + Wheel • F1/F2 toggle", 10, 40, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}