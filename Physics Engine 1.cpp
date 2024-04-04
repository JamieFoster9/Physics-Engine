#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <list>
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
    sf::Text text2;
    sf::Font font;
    font.loadFromFile("Fonts/Montserrat.otf");
    text.setFont(font);
    text.setCharacterSize(30);
    text.setPosition(20.f, 10.f);
    text.setFillColor(sf::Color::Black);
    text2.setFont(font);
    text2.setCharacterSize(30);
    text2.setPosition(20.f, 50.f);
    text2.setFillColor(sf::Color::Black);

    ParticleSystem particleSystem;

    particleSystem.addRectangle(sf::Vector2f(400.f, 400.f), 20, 5, 30, 5); //adds particles and spring to 

    particleSystem.addParticle(50.f, sf::Vector2f(700.f, 50.f), sf::Vector2f(0.f, 500.f), sf::Color::Blue, false, 1000);
    //particleSystem.addParticle(20.f, sf::Vector2f(600.f, 50.f), sf::Vector2f(0.f, 2000.f), sf::Color::Blue, false, 1);
    //particleSystem.addParticle(20.f, sf::Vector2f(800.f, 50.f), sf::Vector2f(0.f, 2000.f), sf::Color::Blue, false, 1);
    
    //particleSystem.addSpring(particleSystem.particles[4], particleSystem.particles[5], 10); //created a spring between particle 2 and 3
    //particleSystem.addSpring(particleSystem.particles[5], particleSystem.particles[6], 10); //created a spring between particle 3 and 1
    
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

        int framerate = 1 / dt; //Framerate in Hz
        string framesText = to_string(framerate);
        int mass = particleSystem.particles[0].getMass();
        int gravity = particleSystem.particles[0].getGravity();
        int damping = particleSystem.springs.front().getDamping();
        int stiffness = particleSystem.springs.front().getStiffness();
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
    return 0;
}