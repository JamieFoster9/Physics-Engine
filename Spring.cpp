#include "Spring.h"

Spring& Spring::operator=(const Spring& other) {
    if (this != &other) {
        // Copy data members from 'other' to 'this'
        // Don't forget to properly handle any dynamically allocated resources if applicable
    }
    return *this;
}

float Spring::getStiffness() { return stiffness; }

float Spring::getDamping() { return damping; }

float Spring::getDistance() {
    sf::Vector2f displacement = particle2.getPosition() - particle1.getPosition(); //difference in distance between the x and y coordinates of each particle //400
    float distance = calculateMagnitude(displacement); //gets the magnitude
    return distance;
}

Particle Spring::getParticle1() { return particle1; }

Particle Spring::getParticle2() { return particle2; }

//If the extension or compression of the spring is more than the breaklength return ture
bool Spring::shouldRemoveSpring() {
    float changeInLength = getDistance() - defaultLength;
    return changeInLength >= breakLength || changeInLength <= -breakLength;
}

void Spring::setStiffness(float stiffnessValue) {
    stiffness = stiffnessValue;
}

void Spring::setDamping(float dampingValue) {
    damping = dampingValue;
}

float Spring::calculateMagnitude(sf::Vector2f& vector) {
    return sqrt((vector.x * vector.x) + (vector.y * vector.y));
}

sf::Vector2f Spring::normalise(const sf::Vector2f& vector) {
    float magnitude = sqrt(vector.x * vector.x + vector.y * vector.y);
    if (magnitude != 0) {
        return sf::Vector2f(vector.x / magnitude, vector.y / magnitude);
    }
    else {
        // Handle division by zero or zero-length vectors
        return sf::Vector2f(0, 0);
    }
}

sf::Vector2f Spring::calculateForce() {
    sf::Vector2f displacement = particle2.getPosition() - particle1.getPosition(); //difference in distance between the x and y coordinates of each particle //400
    float distance = calculateMagnitude(displacement); //gets the magnitude
    float force = stiffness * (distance - defaultLength); //Force exerted by spring in direction of spring F = kx //100 * (400 - 100) = 300000
    sf::Vector2f direction = normalise(displacement); //(1,0) unit vector
    sf::Vector2f relativeVelocity = particle2.velocity - particle1.velocity;
    float dampingForceMagnitude = damping * dotProduct(relativeVelocity, direction);
    sf::Vector2f dampingForce = dampingForceMagnitude * direction;
    return (direction * force) - dampingForce;
}