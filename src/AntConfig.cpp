// src/AntConfig.cpp
#include "AntConfig.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

AntConfig AntConfig::load(const char* filename) {
    AntConfig cfg;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Warning: " << filename << " missing — using defaults\n";
        return cfg;
    }
    try {
        nlohmann::json j;
        file >> j;
        cfg.speed                  = j.value("speed", cfg.speed);
        cfg.turn_rate              = j.value("turn_rate", cfg.turn_rate);
        cfg.sensor_angle           = j.value("sensor_angle", cfg.sensor_angle);
        cfg.sensor_distance        = j.value("sensor_distance", cfg.sensor_distance);
        cfg.sensor_width           = j.value("sensor_width", cfg.sensor_width);
        cfg.lay_rate_food          = j.value("lay_rate_food", cfg.lay_rate_food);
        cfg.lay_rate_home          = j.value("lay_rate_home", cfg.lay_rate_home);
        cfg.lay_rate_home_carrying = j.value("lay_rate_home_carrying", cfg.lay_rate_home_carrying);
        cfg.wander_strength        = j.value("random_wander_strength", cfg.wander_strength);
    } catch (const std::exception& e) {
        std::cerr << "ants.json parse error: " << e.what() << "\n";
    }
    return cfg;
}