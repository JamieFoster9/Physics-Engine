#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Particle.h"

struct Spring {
    Particle& particle1;
    Particle& particle2;
    float stiffness = 3000; //3000
    float damping = 20; //20
    float defaultLength;
    float breakLength = 15; //15

    //Spring is initialised with a reference to two particles
    Spring(Particle& particle1, Particle& particle2, float length)
        : particle1(particle1), particle2(particle2), defaultLength(length) {}

    float getStiffness();

    float getDamping();

    //Returns a reference to particle 1 in the spring
    Particle getParticle1();

    //Returns a reference to particle 2 in the spring
    Particle getParticle2();

    //Gets the displacement vector between two particles
    sf::Vector2f getDisplacement();

    //Gets the distance magnitude between two particles
    float getDistance();

    //Returns the change in length of the spring from the default length
    float getChangeInLength();

    //Returns true if a spring exceeds the break length
    bool shouldRemoveSpring();

    //Returns the magnitude of a vector
    float calculateMagnitude(sf::Vector2f vector);

    //Returns the unit normal vector of a vector
    sf::Vector2f normalise(sf::Vector2f vector);

    //Return the magnitude of the velocity along the normal
    float dotProduct(sf::Vector2f velocity, sf::Vector2f normal);

    //Calculates the force that a spring exerts on a particle
    sf::Vector2f calculateForce();
};