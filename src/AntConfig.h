// src/AntConfig.h  (new file — header-only struct)
#pragma once
#include "raylib.h"

struct AntConfig {
    float speed                = 80.0f;
    float turn_rate            = 6.5f;
    float sensor_angle         = 0.785f;  // 45°
    float sensor_distance      = 9.0f;
    int   sensor_width         = 3;
    float lay_rate_food        = 0.0f;
    float lay_rate_home        = 80.0f;
    float lay_rate_home_carrying = 120.0f;
    float wander_strength      = 0.2f;

    static AntConfig load(const char* filename = "config/ants.json");
};