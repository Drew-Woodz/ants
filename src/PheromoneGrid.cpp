#include "PheromoneGrid.h"
#include "json/single_include/nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

PheromoneGrid::PheromoneGrid(int width, int height)
    : m_width(width), m_height(height)
{
    m_data.assign(m_width * m_height, 0.0f);
    load_config();  // Best-effort load on construction
}

void PheromoneGrid::add_pheromone(int x, int y, float strength)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) return;
    m_data[y * m_width + x] = std::min(m_config.max_value,
                                       m_data[y * m_width + x] + strength);
}

float PheromoneGrid::get_pheromone(int x, int y) const
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) return 0.0f;
    return m_data[y * m_width + x];
}

void PheromoneGrid::update(float /*dt*/)
{
    evaporate();
    diffuse();
}

void PheromoneGrid::evaporate()
{
    for (auto& cell : m_data) {
        cell *= m_config.evaporation_rate;
    }
}

void PheromoneGrid::diffuse()
{
    std::vector<float> next = m_data;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            float sum = m_data[y * m_width + x] * (1.0f - 4 * m_config.diffusion_rate);
            if (x > 0)     sum += m_data[y * m_width + (x-1)] * m_config.diffusion_rate;
            if (x < m_width-1) sum += m_data[y * m_width + (x+1)] * m_config.diffusion_rate;
            if (y > 0)     sum += m_data[(y-1) * m_width + x] * m_config.diffusion_rate;
            if (y < m_height-1) sum += m_data[(y+1) * m_width + x] * m_config.diffusion_rate;

            next[y * m_width + x] = sum;
        }
    }

    m_data.swap(next);
}

void PheromoneGrid::draw_debug(const Camera2D& camera, Color tint) const
{
    // Simple heat-map rendering — fully camera-aware (future zoom/pan ready!)
    const float cell_size_screen = 4.0f;  // Adjust for performance vs detail

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            float value = get_pheromone(x, y) / m_config.max_value;
            if (value < 0.01f) continue;

            Vector2 world_pos = { (float)x, (float)y };
            Vector2 screen_pos = GetWorldToScreen2D(world_pos, camera);

            Color c = tint;
            c.a = (unsigned char)(value * 255);
            DrawRectangle((int)screen_pos.x, (int)screen_pos.y, (int)cell_size_screen, (int)cell_size_screen, c);
        }
    }
}

bool PheromoneGrid::load_config(const char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open " << filename << " – using defaults\n";
        return false;
    }

    nlohmann::json j;
    try {
        file >> j;
        m_config.evaporation_rate = j.value("evaporation_rate", m_config.evaporation_rate);
        m_config.diffusion_rate   = j.value("diffusion_rate",   m_config.diffusion_rate);
        m_config.max_value        = j.value("max_value",        m_config.max_value);
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error in " << filename << ": " << e.what() << "\n";
        return false;
    }
    return true;
}