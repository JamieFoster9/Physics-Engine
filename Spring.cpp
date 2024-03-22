#include "Spring.h"

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

void Spring::applyForce(float dt) {
    sf::Vector2f displacement = particle2.getPosition() - particle1.getPosition(); //difference in distance between the x and y coordinates of each particle //400
    float distance = calculateMagnitude(displacement); //gets the magnitude //400
    float force = stiffness * (distance - length); //Force exerted by spring in direction of spring F = kx //100 * (400 - 100) = 300000
    sf::Vector2f direction = normalise(displacement); //(1,0) unit vector
    sf::Vector2f relativeVelocity = particle2.velocity - particle1.velocity;
    float dampingForceMagnitude = damping * dotProduct(relativeVelocity, direction);
    sf::Vector2f dampingForce = dampingForceMagnitude * direction;
    particle1.applyForce((direction * force) + dampingForce, dt); //(30000, 0)
    particle2.applyForce((direction * (-force)) - dampingForce, dt);//(-30000, 0)
}