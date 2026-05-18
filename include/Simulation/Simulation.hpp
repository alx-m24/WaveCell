#pragma once

#include <cmath>
#include <cstdint>
#include <SFML/Graphics.hpp>

#include "Grid/Grid.hpp"

// Essentially a PV = NKT system
class Simulation {
    private:
        static constexpr uint32_t RESOLUTION_X = 50;
        static constexpr uint32_t RESOLUTION_Y = 25;

        float V{};
        static constexpr double K = 1.38e-23;

    private:
        struct Cell {
            float P;
            uint32_t N; // particle Num
            float T;
        };
        Grid<Cell> grid{};

    public:
        Simulation() = default;
        Simulation(sf::RenderTarget& target) {
            grid.Init(sf::Vector2f(target.getSize()), sf::Vector2u(RESOLUTION_X, RESOLUTION_Y));
            V = grid.getCellSize().x * grid.getCellSize().y;

            grid.forEach(
                [](Cell& cell, size_t, size_t) {
                    cell.N = static_cast<float>((rand() % 1000));
                    cell.T = 1.0f;
                }
            );
        }

    public:
        void Update() {
            grid.forEach(
                [this](Cell& cell, size_t, size_t) {
                    cell.P = static_cast<float>(cell.N) * K * cell.T / this->V;
                }
            );
        }

        enum class OutputMode : uint8_t {
            GRAY_SCALE_PRESSURE
        };
        void Draw(sf::RenderTarget& target, OutputMode mode = OutputMode::GRAY_SCALE_PRESSURE) const {
            switch (mode) {
                case OutputMode::GRAY_SCALE_PRESSURE:
                    VisualizePressure(target); 
                    break;
                default:
                    grid.Draw(target);
                    break;
            }
        }

    private:
        void VisualizePressure(sf::RenderTarget& target) const {
            float max = 0.0f;
            grid.forEach(
                [&max](const Cell& cell, size_t, size_t) {
                    max = std::max(max, cell.P);
                }
            );
            
            grid.forEach(
                [max, cellSize = grid.getCellSize(), &target](const Cell& cell, size_t i, size_t j) {
                    sf::RectangleShape drawableCell{};

                    float color = cell.P / max;
                    uint8_t colorARGB = static_cast<uint8_t>(color * 255);

                    drawableCell.setFillColor(sf::Color(colorARGB, colorARGB, colorARGB));
                    drawableCell.setSize(cellSize);
                    float x = static_cast<float>(i);
                    float y = static_cast<float>(j);
                    sf::Vector2f position = sf::Vector2f(cellSize.x * x, cellSize.y * y);
                    drawableCell.setPosition(position);
                    target.draw(drawableCell);

                }
            );

        }
};
