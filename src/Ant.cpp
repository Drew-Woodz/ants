// src/Ant.cpp
#include "Ant.h"
#include "raylib.h"
#include "raymath.h"     // Vector2Add, Vector2Scale

// Constructor – per-ant personality
Ant::Ant()
{
    float variation = (float)GetRandomValue(-100, 100) / 1000.0f;  // ±10%
    turn_rate_bias = 1.0f + variation;
    wander_bias    = 1.0f + variation * 0.5f;
}

// Const-correct sensing (reads only)
float Ant::sense(const PheromoneGrid& grid, float angle_offset, const AntConfig& cfg) const
{
    float dir_x = cosf(angle + angle_offset);
    float dir_y = sinf(angle + angle_offset);
    Vector2 ahead = {
        pos.x + dir_x * cfg.sensor_distance,
        pos.y + dir_y * cfg.sensor_distance
    };

    float sum = 0.0f;
    int hw = cfg.sensor_width / 2;
    for (int dy = -hw; dy <= hw; ++dy)
    {
        for (int dx = -hw; dx <= hw; ++dx)
        {
            int x = (int)(ahead.x + dx);
            int y = (int)(ahead.y + dy);
            sum += grid.get_pheromone(x, y);
        }
    }
    return sum;
}

// Non-const because it mutates the grid
void Ant::lay_pheromone(PheromoneGrid& grid, float rate, float dt, const AntConfig& cfg)
{
    if (rate <= 0.0f) return;
    grid.add_pheromone((int)pos.x, (int)pos.y, rate * dt);
}

// The ONE AND ONLY update implementation
void Ant::update(float dt,
                 const PheromoneGrid& foodGrid,
                 PheromoneGrid& homeGrid,          // ← NON-CONST
                 const AntConfig& cfg)
{
    const PheromoneGrid& activeGrid = (state == AntState::Searching) ? foodGrid : homeGrid;

    float left   = sense(activeGrid, +cfg.sensor_angle, cfg);
    float center = sense(activeGrid,  0.0f,          cfg);
    float right  = sense(activeGrid, -cfg.sensor_angle, cfg);

    float desired_turn = 0.0f;

    if (center > left && center > right)
    {
        desired_turn = ((GetRandomValue(0, 1) == 0) ? -1.0f : 1.0f)
                     * cfg.wander_strength * wander_bias;
    }
    else if (left > right)
    {
        desired_turn = +cfg.turn_rate * turn_rate_bias;
    }
    else if (right > left)
    {
        desired_turn = -cfg.turn_rate * turn_rate_bias;
    }
    else
    {
        desired_turn = ((GetRandomValue(0, 1) == 0) ? -1.0f : 1.0f)
                     * cfg.wander_strength * wander_bias;
    }

    angle += desired_turn * dt;

    Vector2 forward = { cosf(angle), sinf(angle) };
    pos = Vector2Add(pos, Vector2Scale(forward, cfg.speed * dt));

    // World wrapping
    const int WORLD = 1024;
    if (pos.x < 0) pos.x += WORLD;
    if (pos.x >= WORLD) pos.x -= WORLD;
    if (pos.y < 0) pos.y += WORLD;
    if (pos.y >= WORLD) pos.y -= WORLD;

    // Lay trail logic
    float lay_rate = cfg.lay_rate_food;
    if (state == AntState::Returning)
        lay_rate = cfg.lay_rate_home;

    if (lay_rate > 0.0f)
        lay_pheromone(homeGrid, lay_rate, dt, cfg);
}

void Ant::draw(const Camera2D& camera, const Texture2D& tex) const
{
    Rectangle source{0, 0, (float)tex.width, (float)tex.height};
    Rectangle dest{pos.x, pos.y, 64.0f, 64.0f};
    Vector2 origin{32.0f, 32.0f};
    float rotation = angle * RAD2DEG + 90.0f;

    Color tint = (state == AntState::Returning) ? SKYBLUE : WHITE;

    DrawTexturePro(tex, source, dest, origin, rotation, tint);
}
