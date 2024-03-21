#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Particle.h"

struct Spring {
    Particle& particle1;
    Particle& particle2;
    float stiffness; //k = F/x
    float damping = 0; //F = c * v; damping force equals the coefficient * change in velocithy
    float length; //default length between particles

    //Constructor to initialise properties of the spring
    //spring has to be connected to 2 particles
    Spring(Particle& particle1, Particle& particle2, float stiffness, float damping, float length)
        : particle1(particle1), particle2(particle2), stiffness(stiffness), damping(damping), length(length) {}

    float calculateMagnitude(sf::Vector2f& vector);

    //Normalise vectors
    sf::Vector2f normalise(const sf::Vector2f& vector);

    //Applies force from spring to particles
    void applyForce(float dt);
};