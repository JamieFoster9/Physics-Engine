#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "ParticleSystem.h"
using namespace std;
using namespace glm;

int main() {
    sf::RenderWindow window(sf::VideoMode(1400, 800), "SFML Particle System");
    sf::Clock clock; //starts clock
    float timePerFrame = 1/120; //time value of 60 fps
    //sf::Time micro = sf::microseconds(10000);
    //sf::Time milli = sf::milliseconds(10);
    //sf::Time second = sf::seconds(0.01f);
    window.setFramerateLimit(120);
    const float Speed = 50.f;
    float Left = 0.f;
    float Top = 0.f;
    sf::Text text;
    sf::Font font;
    font.loadFromFile("Fonts/Montserrat.otf");
    text.setFont(font);
    text.setString("0");
    text.setCharacterSize(50);
    text.setPosition(20.f, 10.f);
    text.setFillColor(sf::Color::Black);

    ParticleSystem particleSystem;

    particleSystem.addRectangle(sf::Vector2f(400.f, 300.f), 30, 10);

    //particleSystem.addParticle(20.f, sf::Vector2f(500.f, 350.f), sf::Vector2f(10.f, 50.f), sf::Color::Blue, false);
    //particleSystem.addParticle(20.f, sf::Vector2f(600.f, 350.f), sf::Vector2f(0.f, 0.f), sf::Color::Blue, true);
    //particleSystem.addParticle(50.f, sf::Vector2f(600.f, 450.f), sf::Vector2f(500.f, 100.f), sf::Color::Blue);
    //particleSystem.addParticle(50.f, sf::Vector2f(500.f, 450.f), sf::Vector2f(0.f, 0.f), sf::Color::Blue);
    //particleSystem.addParticle(50.f, sf::Vector2f(100.f, 200.f), sf::Vector2f(450.f, 50.f), sf::Color::Blue);
    //particleSystem.addParticle(20.f, sf::Vector2f(600.f, 450.f), sf::Vector2f(150.f, 500.f), sf::Color::Blue);
    //particleSystem.addParticle(10.f, sf::Vector2f(500.f, 450.f), sf::Vector2f(100.f, 600.f), sf::Color::Blue);
    //particleSystem.addParticle(10.f, sf::Vector2f(600.f, 450.f), sf::Vector2f(50.f, 450.f), sf::Color::Blue);
   
    /*
    particleSystem.addSpring(particleSystem.particles[1], particleSystem.particles[2], 1000, 0, 200);
    particleSystem.addSpring(particleSystem.particles[2], particleSystem.particles[3], 1000, 0, 200);
    particleSystem.addSpring(particleSystem.particles[3], particleSystem.particles[0], 1000, 0, 200);
    particleSystem.addSpring(particleSystem.particles[0], particleSystem.particles[2], 1000, 0, 200);
    particleSystem.addSpring(particleSystem.particles[1], particleSystem.particles[3], 1000, 0, 200);
    */
    
    /*
    particleSystem.addSpring(particleSystem.particles[4], particleSystem.particles[5], 10000, 0, 10); //created a spring between particle 2 and 3
    particleSystem.addSpring(particleSystem.particles[5], particleSystem.particles[6], 10000, 0, 10); //created a spring between particle 3 and 1
    particleSystem.addSpring(particleSystem.particles[6], particleSystem.particles[7], 10000, 0, 10); //created a spring between particle 1 and 2
    particleSystem.addSpring(particleSystem.particles[7], particleSystem.particles[8], 10000, 0, 10); //created a spring between particle 2 and 3
    particleSystem.addSpring(particleSystem.particles[8], particleSystem.particles[9], 10000, 0, 10); //created a spring between particle 3 and 1
    particleSystem.addSpring(particleSystem.particles[9], particleSystem.particles[0], 10000, 0, 10); //created a spring between particle 3 and 1
    */

    //run the program as long as the window is open
    while (window.isOpen()) {
        //check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            //"close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.getElapsedTime().asSeconds();
        if (dt < timePerFrame) {
            sf::sleep(sf::seconds(timePerFrame - dt));
            dt += timePerFrame - dt;
        }
        clock.restart();

        //if (window.GetInput().IsKeyDown(sf::Key::Left))  Left -= Speed * dt;
        //if (window.GetInput().IsKeyDown(sf::Key::Right)) Left += Speed * dt;
        //if (window.GetInput().IsKeyDown(sf::Key::Up))    Top -= Speed * dt;
        //if (window.GetInput().IsKeyDown(sf::Key::Down))  Top += Speed * dt;

        float framerate = 1 / dt; //Framerate in Hz
        string framesText = to_string(framerate);
        text.setString(framesText + " fps");
        window.draw(text);
        particleSystem.update(dt, window.getSize());
        window.clear(sf::Color::White);
        particleSystem.draw(window);
        window.draw(text);
        window.display();
    }

    return 0;
}