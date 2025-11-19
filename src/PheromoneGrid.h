#pragma once

#include <vector>
#include "raylib.h"

// Forward declare for config (we'll create config/pheromones.json next)
struct PheromoneConfig {
    float evaporation_rate = 0.995f;   // 0.5% loss per frame → ~10s half-life at 60fps
    float diffusion_rate   = 0.1f;     // How much bleeds to neighbors
    float default_value    = 0.0f;
    float max_value        = 1000.0f;
};

class PheromoneGrid
{
public:
    explicit PheromoneGrid(int width = 1024, int height = 1024);
    ~PheromoneGrid() = default;

    // Core API
    void update(float dt = 1.0f);                    // Evaporate + diffuse
    void add_pheromone(int x, int y, float strength);
    float get_pheromone(int x, int y) const;

    // Debug / future camera support
    void draw_debug(const Camera2D& camera, Color tint = WHITE) const;

    // Data-driven reload
    bool load_config(const char* filename = "config/pheromones.json");

    [[nodiscard]] int width() const  { return m_width; }
    [[nodiscard]] int height() const { return m_height; }
    [[nodiscard]] Vector2 world_size() const { return { (float)m_width, (float)m_height }; }

private:
    int m_width{};
    int m_height{};
    std::vector<float> m_data;

    PheromoneConfig m_config;

    void evaporate();
    void diffuse();
};