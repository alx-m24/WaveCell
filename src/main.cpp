#include <SFML/Graphics.hpp>

#include "Simulation/Simulation.hpp"

int main() {
    sf::ContextSettings windowSettings{};
    windowSettings.antialiasingLevel = 8;
    
    sf::RenderWindow window(sf::VideoMode(1000, 600), "WaveCell", sf::Style::Default, windowSettings);
    window.setVerticalSyncEnabled(true);

    Simulation simulation(window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: window.close(); break;
                default: break;
            }
        }

        simulation.Update();

        window.clear(sf::Color::Blue);

        simulation.Draw(window);

        window.display();
    }

    return EXIT_SUCCESS;
}
