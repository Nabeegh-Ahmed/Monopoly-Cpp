#include <SFML/Graphics.hpp>
#include "Resources/Dice.h"
#include "Resources/GameEngine.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    Dice d(200, 200);
    GPEngine Engine;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (Engine.MouseClick(d.getBlock(), event, window)) std::cout << d.Roll() << std::endl;
        }
        
        window.clear(sf::Color::White);
        d.Draw(window);
        window.display();
    }

    return 0;
}
