#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Particle.h"

struct Spring {
    Particle& particle1;
    Particle& particle2;
    float stiffness = 10000; //k = F/x
    float damping = 20; //F = c * v; damping force equals the coefficient * change in velocithy
    float length; //default length between particles

    //Constructor to initialise properties of the spring
    //spring has to be connected to 2 particles
    Spring(Particle& particle1, Particle& particle2, float length)
        : particle1(particle1), particle2(particle2), length(length) {}

    float calculateMagnitude(sf::Vector2f& vector);

    //Normalise vectors
    sf::Vector2f normalise(const sf::Vector2f& vector);

    float dotProduct(const sf::Vector2f& velocity, const sf::Vector2f& normal) {
        return velocity.x * normal.x + velocity.y * normal.y;
    }

    //Applies force from spring to particles
    void applyForce(float dt);
};