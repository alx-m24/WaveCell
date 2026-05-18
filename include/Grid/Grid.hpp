#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

template<typename T>
class Grid {
    private:
        sf::Vector2f m_offset{};

        sf::Vector2f m_size{};
        sf::Vector2u m_resolution{};

        std::vector<T> m_raw{};

        const sf::Vector2f outlineThickness = sf::Vector2f{ 2.0f, 2.0f };
    public:
        Grid() = default;

    public:
        void Init(sf::Vector2f size, sf::Vector2u resolution, sf::Vector2f offset = { 0.0f, 0.0f }) {
            this->m_size = size;
            this->m_resolution = resolution;
            this->m_offset = offset;

            m_raw.resize(resolution.x * resolution.y);
            forEach([](T& t, size_t, size_t) { t = T{}; });
        }

        void Draw(sf::RenderTarget& target) const {
            sf::RectangleShape drawableCell{};

            drawableCell.setOutlineThickness(outlineThickness.x);
            drawableCell.setFillColor(sf::Color::White);
            drawableCell.setOutlineColor(sf::Color::Black);

            const sf::Vector2 cellSize = sf::Vector2((m_size.x - outlineThickness.x * 2.0f) / m_resolution.x, (m_size.y - outlineThickness.y * 2.0f) / m_resolution.y);
            drawableCell.setSize(cellSize);

            forEach(
                [&drawableCell, cellSize, offset = outlineThickness + m_offset, &target] (const T&, size_t i, size_t j) {
                    float x = static_cast<float>(i);
                    float y = static_cast<float>(j);
                    sf::Vector2f position = sf::Vector2f(cellSize.x * x, cellSize.y * y);
                    drawableCell.setPosition(position + offset);
                    target.draw(drawableCell);
                }
            );
        }

    public:
        sf::Vector2f getCellSize() const {
            return sf::Vector2(m_size.x / m_resolution.x, m_size.y / m_resolution.y);
        }

    public:
        template<typename Func>
        void forEach(Func&& func) {
            for (size_t y = 0; y < m_resolution.y; y++) {
                for (size_t x = 0; x < m_resolution.x; x++) {
                    func(m_raw[getIndex(x, y)], x, y);
                }
            }
        }

        template<typename Func>
        void forEach(Func&& func) const {
            for (size_t y = 0; y < m_resolution.y; y++) {
                for (size_t x = 0; x < m_resolution.x; x++) {
                    func(static_cast<const T&>(m_raw[getIndex(x, y)]), x, y);
                }
            }
        }

    public:
        size_t getIndex(size_t x, size_t y) const {
            return x + y * m_resolution.x;
        }

    public:
        T& operator()(size_t x, size_t y) {
            assert(inBounds(x, y));
            return m_raw[getIndex(x, y)];
        }
        
        const T& operator()(size_t x, size_t y) const {
            assert(inBounds(x, y));
            return m_raw[getIndex(x, y)];
        }

    public:
        bool inBounds(int x, int y) const {
            return x >= 0 &&
                   y >= 0 &&
                   x < m_resolution.x &&
                   y < m_resolution.y;
        }

        bool inBounds(size_t x, size_t y) const {
            return x >= 0 &&
                   y >= 0 &&
                   x < m_resolution.x &&
                   y < m_resolution.y;
        }
};
