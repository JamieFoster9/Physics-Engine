#include <vector>
#include "ParticleSystem.h"
using namespace std;

void ParticleSystem::addParticle(float radius, const sf::Vector2f& position, const sf::Vector2f& velocity, const sf::Color& color, bool fixedParticle) {
    particles.emplace_back(radius, position, velocity, color, fixedParticle);
}

void ParticleSystem::addSpring(Particle& particle1, Particle& particle2, float length) {
    Spring spring = Spring(particle1, particle2, length);
    springs.emplace_back(spring);
}

void ParticleSystem::update(float dt, const sf::Vector2u& windowSize) {
    checkParticleCollision(windowSize, dt);
    for (auto& spring : springs) {
        spring.applyForce(dt);
    }
    for (auto& particle : particles) {
        particle.update(dt);
        particle.checkBoundaryCollision(windowSize);
    }
}

float ParticleSystem::calculateMagnitude(sf::Vector2f& vector) {
    return sqrt((vector.x * vector.x) + (vector.y * vector.y));
}

float ParticleSystem::dotProduct(const sf::Vector2f& velocity, const sf::Vector2f& normal) {
    return velocity.x * normal.x + velocity.y * normal.y;
}

void ParticleSystem::draw(sf::RenderWindow& window) {
    for (const auto& particle : particles) {
        window.draw(particle.shape);
    }
    for (const auto& spring : springs) {
        sf::VertexArray line(sf::Lines, 2);
        // Set the positions of the line vertices to the positions of the connected particles
        line[0].position = spring.particle1.shape.getPosition() + sf::Vector2f(spring.particle1.shape.getRadius(), spring.particle1.shape.getRadius());
        line[1].position = spring.particle2.shape.getPosition() + sf::Vector2f(spring.particle2.shape.getRadius(), spring.particle2.shape.getRadius());
        line[0].color = sf::Color::Black; // Example color (change as needed)
        line[1].color = sf::Color::Black; // Example color (change as needed)
        window.draw(line);
    }
}

void ParticleSystem::addRectangle(sf::Vector2f startPosition, int width, int height) {
    sf::Vector2f velocity = sf::Vector2f(0.f, 0.f); //particles have 0 velocity to start
    sf::Vector2f particlePosition = startPosition; //determines where new particle is going to be created
    sf::Vector2f rowPosition = startPosition;
    int particleNumber = 0;

    for (int i = 0; i < height; i++) { //for each row in the height
        for (int j = 0; j < width; j++) { //this creates rows of particles
            addParticle(5.f, particlePosition, velocity, sf::Color::Blue, false); //next particle in width
            particlePosition += sf::Vector2f(20.f, 0.f); //adds 20 to x position
        }
        rowPosition += sf::Vector2f(0.f, 20.f); //resets position and adds 20 to y position
        particlePosition = rowPosition; //Set first particle to new row position
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width - 1; j++) { //this adds springs between particles in each row
            addSpring(particles[particleNumber], particles[particleNumber + 1], 20); //join particle to previous particle
            particleNumber += 1;
        }
        particleNumber += 1;
    }
    particleNumber = 0;
    for (int i = 0; i < height-1; i++) {
        for (int j = 0; j < width; j++) { //this adds springs between particles in each column
            addSpring(particles[particleNumber], particles[particleNumber + width], 20); //join particle to previous particle
            particleNumber += 1;
        }
    }
}

void ParticleSystem::checkParticleCollision(const sf::Vector2u& windowSize, float dt) {
    
    int repulsionFactor = 10;

    for (auto iterator1 = particles.begin(); iterator1 != particles.end(); ++iterator1) { //for a particle
        for (auto iterator2 = next(iterator1); iterator2 != particles.end(); ++iterator2) { //compare particle to every other particle
            Particle& p1 = *iterator1;
            Particle& p2 = *iterator2;
    /*
    for (Particle& p1 : particles) { //for a particle
        for (Particle& p2 : particles) { //compare particle to every other particle
            if (p1.getPosition() == p2.getPosition()) {
                continue;
            }
            */

            sf::Vector2f displacement = p2.getPosition() - p1.getPosition(); //difference in distance between the x and y coordinates of each particle
            float distance = calculateMagnitude(displacement); //gets the magnitude
            if (round(distance) < p1.shape.getRadius() * 2) { //if the distnce between to cicircle is less than their combined radius then it is colliding
                //Project velocities into components along normal
                //normalVelcoities is the particles velocity in direction of the normal vector

                sf::Vector2f repulsionForce = (p1.shape.getRadius() * 2 - distance) * repulsionFactor * (displacement / distance);
                // Apply repulsion force to both particles
                p1.applyForce(repulsionForce,dt);
                p2.applyForce(-repulsionForce,dt);
                
                sf::Vector2f normal = displacement / distance; //unit normal vector
                sf::Vector2f tangent(-displacement.y / distance, displacement.x / distance); // Unit tangent vector
                float normalVelocityP1 = dotProduct(p1.velocity, normal);
                float tangentVelocityP1 = dotProduct(p1.velocity, tangent);
                float normalVelocityP2 = dotProduct(p2.velocity, normal);
                float tangentVelocityP2 = dotProduct(p2.velocity, tangent);

                //if we assume momentum is conserved and paricles have the same mass the exit velocity of particle 2 is the entry velocity of particle 1
                float finalNormalVelocityP1 = normalVelocityP2;
                float finalNormalVelocityP2 = normalVelocityP1;

                // Update velocities
                p1.velocity.x = finalNormalVelocityP1 * normal.x + tangentVelocityP1 * tangent.x;
                p1.velocity.y = finalNormalVelocityP1 * normal.y + tangentVelocityP1 * tangent.y;
                p2.velocity.x = finalNormalVelocityP2 * normal.x + tangentVelocityP2 * tangent.x;
                p2.velocity.y = finalNormalVelocityP2 * normal.y + tangentVelocityP2 * tangent.y;
            }
        }
    }
}