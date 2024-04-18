#include "Spring.h"

float Spring::getStiffness() { return stiffness; }

float Spring::getDamping() { return damping; }

Particle Spring::getParticle1() { return particle1; }

Particle Spring::getParticle2() { return particle2; }

sf::Vector2f Spring::getDisplacement() {
    return particle2.getPosition() - particle1.getPosition();
}

float Spring::getDistance() {
    return calculateMagnitude(getDisplacement());
}

float Spring::getChangeInLength() { return getDistance() - defaultLength; }

bool Spring::shouldRemoveSpring() {
    return getChangeInLength() >= breakLength || getChangeInLength() <= -breakLength;
}

float Spring::calculateMagnitude(sf::Vector2f vector) {
    return sqrt((vector.x * vector.x) + (vector.y * vector.y));
}

sf::Vector2f Spring::normalise(sf::Vector2f vector) {
    float magnitude = calculateMagnitude(vector);
    if (magnitude != 0) {
        return sf::Vector2f(vector.x / magnitude, vector.y / magnitude);
    } else {
        //In case of zero values
        return sf::Vector2f(0, 0);
    }
}

float Spring::dotProduct(sf::Vector2f velocity, sf::Vector2f normal) {
    return velocity.x * normal.x + velocity.y * normal.y;
}

sf::Vector2f Spring::calculateForce() {
    sf::Vector2f displacement = getDisplacement();
    float distance = calculateMagnitude(displacement);
    //Force exerted by spring in direction of spring F = kx
    float force = stiffness * getChangeInLength();
    //Gets the unit direction of the diplacement
    sf::Vector2f direction = normalise(displacement);
    sf::Vector2f relativeVelocity = particle2.velocity - particle1.velocity;
    //F = c * v; damping force equals damping coefficient * velocity
    float dampingForceMagnitude = damping * dotProduct(relativeVelocity, direction);
    sf::Vector2f dampingForce = dampingForceMagnitude * direction;
    return (direction * force) - dampingForce;
}