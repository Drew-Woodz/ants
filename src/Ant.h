// src/Ant.h
#pragma once

#include "raylib.h"
#include "PheromoneGrid.h"
#include "AntConfig.h"

enum class AntState { Searching, Returning };

struct Ant
{
    Vector2 pos{512.0f, 512.0f};
    float   angle = 0.0f;

    AntState state = AntState::Searching;

    float turn_rate_bias = 1.0f;
    float wander_bias    = 1.0f;

    Ant();

    void update(float dt,
                const PheromoneGrid& foodGrid,
                PheromoneGrid& homeGrid,           // ← NON-CONST
                const AntConfig& cfg);

    void draw(const Camera2D& camera, const Texture2D& tex) const;

private:
    float sense(const PheromoneGrid& grid, float angle_offset, const AntConfig& cfg) const;
    void lay_pheromone(PheromoneGrid& grid, float rate, float dt, const AntConfig& cfg);
};