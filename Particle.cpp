#include "Particle.h"

sf::Vector2f Particle::getPosition() {
    return position; 
}

float Particle::getMass() { return mass; }

float Particle::getGravity() { return gravity; }

void Particle::setMass(float massValue) {
    mass = massValue;
}

void Particle::setGravity(float gravityValue) {
    gravity = gravityValue;
}

//Force applied on particle from the spring
void Particle::applyForce(sf::Vector2f force, float dt) {
    float dampingPercent = 1;

    if (!fixedParticle) {
        sf::Vector2f acceleration = (force / mass) * dampingPercent;
        sf::Vector2f velocityChange = acceleration * dt;
        velocity += velocityChange;
        sf::Vector2f positionChange = velocityChange * dt;
        position += positionChange;
        shape.move(sf::Vector2f(positionChange));
    }
}

void Particle::update(float dt) {
    if (!fixedParticle) {
        float dampingPercent = 0.5;
        float dPositionx = velocity.x * dt * dampingPercent;
        float dPositiony = velocity.y * dt * dampingPercent;
        velocity.y += gravity * dt; //need to update velocity as it's changing with the acceleration of gravity with v2 = v1 + h(F1/m) or v2 = v1 + h*a1 as F/m = a which is constant for masses
        position.x += dPositionx;
        position.y += dPositiony;
        shape.move(sf::Vector2f(dPositionx, dPositiony));
    }
}

void Particle::checkBoundaryCollision(const sf::Vector2u& windowSize, float dt) {
    sf::Vector2f position = shape.getPosition();
    float radius = shape.getRadius();
    int repulsionFactor = 500;
    float dampingPercent = 0.4;
    // Check collision with left edge
    if (position.x <= radius) {
        velocity.x = -velocity.x * dampingPercent;  // Invert the x component of velocity
        float repulsionForce = (-position.x) * repulsionFactor; //further the particles get outside the boundary the larger the repulsion force
        applyForce(sf::Vector2f(-repulsionForce, 0.f), dt);
    }
    // Check collision with right edge
    if (position.x >= windowSize.x - radius) {
        velocity.x = -velocity.x * dampingPercent;  // Invert the x component of velocity
        float repulsionForce = (position.x + (2 * radius) - windowSize.x) * repulsionFactor; //further the particles get outside the boundary the larger the repulsion force
        applyForce(sf::Vector2f(-repulsionForce, 0.f), dt);
    }
    // Check collision with top edge
    if (position.y <= radius) {
        velocity.y = -velocity.y * dampingPercent;  // Invert the y component of velocity
        float repulsionForce = (-position.y) * repulsionFactor; //further the particles get outside the boundary the larger the repulsion force
        applyForce(sf::Vector2f(0.f, -repulsionForce), dt);
    }
    // Check collision with bottom edge
    if (position.y >= windowSize.y - radius) {
        velocity.y = -velocity.y * dampingPercent;  // Invert the y component of velocity
        float repulsionForce = (position.y + (2 * radius) - windowSize.y) * repulsionFactor; //further the particles get outside the boundary the larger the repulsion force
        applyForce(sf::Vector2f(0.f, -repulsionForce), dt);
    }
}