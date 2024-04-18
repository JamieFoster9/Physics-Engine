#include "Particle.h"

sf::Vector2f Particle::getPosition() { return position; }

float Particle::getMass() { return mass; }

float Particle::getGravity() { return gravity; }

void Particle::applyForce(sf::Vector2f force, float dt) {
    float dampingPercent = 1;
    if (!fixedParticle) {
        sf::Vector2f acceleration = (force / mass) * dampingPercent;
        sf::Vector2f velocityChange = acceleration * dt;
        velocity += velocityChange;
        sf::Vector2f positionChange = velocityChange * dt;
        position += positionChange;
        circle.move(sf::Vector2f(positionChange));
    }
}

void Particle::update(float dt) {
    if (!fixedParticle) {
        float dampingPercent = 0.5;
        float dPositionx = velocity.x * dt * dampingPercent;
        float dPositiony = velocity.y * dt * dampingPercent;
        //Need to update velocity as it's changing with the acceleration of gravity with 
        //v2 = v1 + h(F1/m) or v2 = v1 + h * a1 as (F/m = a) which is constant for masses
        velocity.y += gravity * dt;
        position.x += dPositionx;
        position.y += dPositiony;
        circle.move(sf::Vector2f(dPositionx, dPositiony));
    }
}

void Particle::checkBoundaryCollision(const sf::Vector2u& windowSize, float dt) {
    sf::Vector2f position = circle.getPosition();
    float radius = circle.getRadius();
    int repulsionFactor = 100;
    float dampingPercent = 0.5;
    //Check collision with left edge
    if (position.x <= radius) {
        //Invert the x component of velocity
        velocity.x = -velocity.x * dampingPercent;
        //the further the particles get outside the boundary the larger the repulsion force
        float repulsionForce = (-position.x) * repulsionFactor;
        applyForce(sf::Vector2f(repulsionForce, 0.f), dt);
    }
    // Check collision with right edge
    if (position.x >= windowSize.x - radius) {
        velocity.x = -velocity.x * dampingPercent;
        float repulsionForce = (position.x + (2 * radius) - windowSize.x) * repulsionFactor;
        applyForce(sf::Vector2f(-repulsionForce, 0.f), dt);
    }
    // Check collision with top edge
    if (position.y <= radius) {
        //Invert the y component of velocity
        velocity.y = -velocity.y * dampingPercent;
        float repulsionForce = (-position.y) * repulsionFactor;
        applyForce(sf::Vector2f(0.f, repulsionForce), dt);
    }
    // Check collision with bottom edge
    if (position.y >= windowSize.y - radius) {
        velocity.y = -velocity.y * dampingPercent;
        float repulsionForce = (position.y + (2 * radius) - windowSize.y) * repulsionFactor;
        applyForce(sf::Vector2f(0.f, -repulsionForce), dt);
    }
}