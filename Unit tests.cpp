#include <iostream>
#include <cassert>
#include "Particle.h"

void testApplyForce() {
    Particle particle = Particle(50.f, sf::Vector2f(700.f, 100.f), sf::Vector2f(0.f, 500.f), sf::Color::Blue, false, 1);
    particle.applyForce(sf::Vector2f(10.0f, 5.0f), 0.1f); // Apply force

    // Expected velocity change based on force and mass
    sf::Vector2f expectedVelocityChange(1.0f, 0.5f);
    // Expected position change based on velocity change and time step
    sf::Vector2f expectedPositionChange(0.1f, 0.05f);

    assert(particle.velocity == expectedVelocityChange);
    //assert(particle.position() == expectedPositionChange);
}

/*
void testUpdate() {
    Particle particle;
    particle.setFixedParticle(false); // Make the particle movable
    particle.setGravity(9.81f); // Set gravity
    particle.setVelocity(sf::Vector2f(5.0f, 10.0f)); // Set initial velocity

    // Update particle with a time step of 0.1 seconds
    particle.update(0.1f);

    // Expected position change based on velocity, gravity, and time step
    sf::Vector2f expectedPositionChange(0.5f, 1.0f);

    assert(particle.getPosition() == expectedPositionChange);
}

void testBoundaryCollision() {
    Particle particle;
    sf::Vector2u windowSize(800, 600); // Set window size
    particle.setPosition(sf::Vector2f(10.0f, 10.0f)); // Set initial position
    particle.setVelocity(sf::Vector2f(-2.0f, -3.0f)); // Set initial velocity

    // Set particle radius
    float radius = 5.0f;

    // Perform boundary collision check
    particle.checkBoundaryCollision(windowSize, 0.1f);

    // Expected position after collision with left edge
    sf::Vector2f expectedPosition(15.0f, 10.0f);
    assert(particle.getPosition() == expectedPosition);

    // Check collision with top edge
    particle.setPosition(sf::Vector2f(400.0f, 5.0f)); // Reset position
    particle.setVelocity(sf::Vector2f(2.0f, -3.0f)); // Reset velocity
    particle.checkBoundaryCollision(windowSize, 0.1f);
    // Expected position after collision with top edge
    expectedPosition = sf::Vector2f(400.0f, 15.0f);
    assert(particle.getPosition() == expectedPosition);

    // Add more tests for other edges if needed
}
*/

int main() {
    testApplyForce();
    //testUpdate();
    //testBoundaryCollision();

    std::cout << "All tests passed!\n";
    return 0;
}