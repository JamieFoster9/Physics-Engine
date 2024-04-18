#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "ParticleSystem.h"
using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode(1400, 800), "SFML Particle System");
    sf::Clock clock;
    ParticleSystem particleSystem;

    sf::Text text;
    sf::Text text2;
    sf::Font font;
    string stats;

    window.setFramerateLimit(120);
    font.loadFromFile("Fonts/Montserrat.otf");
    text.setFont(font);
    text.setCharacterSize(20);
    text.setPosition(20.f, 10.f);
    text.setFillColor(sf::Color::Black);
    text2.setFont(font);
    text2.setCharacterSize(20);
    text2.setPosition(20.f, 50.f);
    text2.setFillColor(sf::Color::Black);

    //Test 1

    //Mass: 1
    //Gravity: 500
    //Damping: 20
    //Stiffness: 3000
    //Break Length: 15

    particleSystem.addRectangle(sf::Vector2f(400.f, 400.f), 20, 5, 30, 5);
    particleSystem.addParticle(50.f, sf::Vector2f(700.f, 100.f), sf::Vector2f(0.f, 500.f), sf::Color::Blue, false, 10000);

    /*
    //Test 2

    //Mass: 1
    //Gravity: 0
    //Damping: 7
    //Stiffness: 1000
    //Break Length: 15

    particleSystem.addRectangle(sf::Vector2f(400.f, 400.f), 30, 10, 20, 2);
    particleSystem.addParticle(50.f, sf::Vector2f(700.f, 100.f), sf::Vector2f(0.f, 500.f), sf::Color::Blue, false, 10000);
    */

    /*
    //Test 3
    
    //Mass: 1
    //Gravity: 500
    //Damping: 20
    //Stiffness: 3000
    //Break Length: 15

    particleSystem.addFibres(sf::Vector2f(400.f, 400.f), 20, 10, 30, 5);
    particleSystem.addParticle(50.f, sf::Vector2f(700.f, 100.f), sf::Vector2f(0.f, 0.f), sf::Color::Blue, false, 10000);
    */
    
    /*
    //Test 4

    //Mass: 1
    //Gravity: 0
    //Damping: 7
    //Stiffness: 1000
    //Break Length: 15

    particleSystem.addFibres(sf::Vector2f(400.f, 400.f), 30, 10, 20, 2);
    particleSystem.addParticle(50.f, sf::Vector2f(700.f, 100.f), sf::Vector2f(0.f, 500.f), sf::Color::Blue, false, 10000);
    */

    //Code adapted from SFML, n.d-f.
    //run the program as long as the window is open
    while (window.isOpen()) {
        sf::Event event;
        //check all the window's events that were triggered since the last iteration of the loop
        while (window.pollEvent(event)) {
            //"close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        float dt = clock.getElapsedTime().asSeconds();
        clock.restart();
        int framerate = 1 / dt;

        int mass = particleSystem.particles[0].getMass();
        int gravity = particleSystem.particles[0].getGravity();
        int damping = particleSystem.springs.front().getDamping();
        int stiffness = particleSystem.springs.front().getStiffness();

        string framesText = to_string(framerate);
        string massText = to_string(mass);
        string gravityText = to_string(gravity);
        string dampingText = to_string(damping);
        string stiffnessText = to_string(stiffness);
        string stats = "mass: " + massText + "\ngravity: " + gravityText + "\ndamping: " + dampingText + "\nstiffness: " + stiffnessText;
        
        text.setString("framerate: " + framesText + " fps");
        text2.setString(stats);
        
        particleSystem.update(dt, window.getSize());
        
        window.clear(sf::Color::White);
        particleSystem.draw(window);
        window.draw(text);
        window.draw(text2);
        window.display();
    }
    //End of adapted code
    return 0;
}