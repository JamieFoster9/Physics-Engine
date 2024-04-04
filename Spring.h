#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Particle.h"

struct Spring {
    Particle& particle1;
    Particle& particle2;
    float stiffness = 4000; //k = F/x
    float damping = 30; //F = c * v; damping force equals the coefficient * change in velocity
    float defaultLength; //default length between particles
    float breakLength = 15;
    Spring& operator=(const Spring& other);

    //Constructor to initialise properties of the spring
    //spring has to be connected to 2 particles
    Spring(Particle& particle1, Particle& particle2, float length)
        : particle1(particle1), particle2(particle2), defaultLength(length) {}

    float getStiffness();

    float getDamping();

    //Gets the distance that the spring is compressed or extended from it default length
    float getDistance();

    Particle getParticle1();

    Particle getParticle2();

    void setStiffness(float stiffnessValue);

    void setDamping(float dampingValue);

    bool shouldRemoveSpring();

    float calculateMagnitude(sf::Vector2f& vector);

    //Normalise vectors
    sf::Vector2f normalise(const sf::Vector2f& vector);

    float dotProduct(const sf::Vector2f& velocity, const sf::Vector2f& normal) {
        return velocity.x * normal.x + velocity.y * normal.y;
    }

    bool exceededLength();

    //Applies force from spring to particles
    sf::Vector2f calculateForce();
};