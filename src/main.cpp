#include "raylib.h"

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Myrmidon α – Ant Colony Prototype");
    SetTargetFPS(60);

    Texture2D ant = LoadTexture("assets/ant.png");  // we'll add a real sprite later
    Vector2 antPos = { (float)screenWidth/2, (float)screenHeight/2 };

    while (!WindowShouldClose())
    {
        float rotation = GetTime() * 90.0f;  // 90 deg/sec

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Sand-colored background – no magic numbers, data-driven ready
        constexpr Color SAND_COLOR{ 194, 154, 94, 255 };
        DrawRectangle(0, 0, screenWidth, screenHeight, SAND_COLOR);

        // Rotating ant in the center
        DrawTexturePro(ant,
                       Rectangle{0, 0, (float)ant.width, (float)ant.height},
                       Rectangle{antPos.x, antPos.y, 128, 128},
                       Vector2{64, 64},  // rotation origin
                       rotation,
                       WHITE);

        DrawFPS(10, 10);
        DrawText("Myrmidon α – Sprint 0 Complete", 10, 40, 20, DARKGRAY);

        EndDrawing();
    }

    UnloadTexture(ant);
    CloseWindow();
    return 0;
}