#include "Particle.h"

sf::Vector2f Particle::getPosition() { return position; }

//Force applied on particle from the spring
void Particle::applyForce(sf::Vector2f force, float dt) {
    float mass = 10;
    if (!fixedParticle) {
        sf::Vector2f acceleration = force / mass; //(30000, 0)/10 = (3000,0) F_1/m
        sf::Vector2f velocityChange = acceleration * dt; //(3000, 0) * 1/60 = (50, 0) h * F_1 / m
        velocity += velocityChange; //v_2 = v_1 + h * F_1 / m
        sf::Vector2f positionChange = velocityChange * dt; //h * v_2
        position += positionChange; //x_2 = x_1 + h * v_2
        shape.move(sf::Vector2f(positionChange));
    }
}

void Particle::update(float dt) {
    if (!fixedParticle) {
        //To do: should be 9.81 m/s2 but scale it for the frame scale
        float gravity = 1000;
        float dPositionx = velocity.x * dt;
        float dPositiony = velocity.y * dt;
        velocity.y += gravity * dt; //need to update velocity as it's changing with the acceleration of gravity with v2 = v1 + h(F1/m) or v2 = v1 + h*a1 as F/m = a which is constant for masses
        position.x += dPositionx;
        position.y += dPositiony;
        shape.move(sf::Vector2f(dPositionx, dPositiony));
    }
}

void Particle::checkBoundaryCollision(const sf::Vector2u& windowSize) {
    sf::Vector2f position = shape.getPosition();
    float radius = shape.getRadius();
    // Check collision with left and right edges
    if (round(position.x) <= 0 || round(position.x + (2 * radius)) >= windowSize.x) {
        velocity.x = -velocity.x;  // Invert the x component of velocity
    }
    // Check collision with top and bottom edges
    if (round(position.y <= 0) || round(position.y + (2 * radius)) >= windowSize.y) {
        velocity.y = -velocity.y;  // Invert the y component of velocity
    }
}