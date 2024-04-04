#include "ParticleSystem.h"
using namespace std;

void ParticleSystem::addParticle(float radius, sf::Vector2f position, sf::Vector2f velocity, const sf::Color& color, bool fixedParticle, float mass) {
    particles.emplace_back(radius, position, velocity, color, fixedParticle, mass); 
    //newParticles.emplace_back(radius, position, velocity, color, fixedParticle, mass; //adds particles to a duplicate vector
}

void ParticleSystem::addSpring(Particle& particle1, Particle& particle2, float length) {
    springs.emplace_back(particle1, particle2, length);
    //newSprings.emplace_back(particle1, particle2, length); //adds springs to a duplicate vector
}

void ParticleSystem::update(float dt, const sf::Vector2u& windowSize) {

    for (auto& particle : particles) {
        particle.update(dt);
        particle.checkBoundaryCollision(windowSize, dt);
    }

    checkParticleCollision(windowSize, dt);

    vector<sf::Vector2f> forces;

    for (auto it = springs.begin(); it != springs.end(); ++it) {
        forces.push_back(it->calculateForce());
    }

    int index = 0;
    for (auto it = springs.begin(); it != springs.end(); ++it) {
        it->particle1.applyForce(forces[index], dt);
        it->particle2.applyForce(-forces[index], dt);
        ++index;
    }

    if (!springs.empty()) {
        for (auto it = springs.begin(); it != springs.end();) {
            if (it->shouldRemoveSpring()) {
                it = springs.erase(it); // Erase the current element and get the iterator to the next element
            }
            else {
                ++it; // Move to the next element
            }
        }
    }
    checkParticleCollision(windowSize, dt);
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
        line[0].position = spring.particle1.shape.getPosition(), spring.particle1.shape.getRadius();
        line[1].position = spring.particle2.shape.getPosition(), spring.particle2.shape.getRadius();
        line[0].color = sf::Color::Black; // Example color (change as needed)
        line[1].color = sf::Color::Black; // Example color (change as needed)
        window.draw(line);
    }
}

void ParticleSystem::addRectangle(sf::Vector2f startPosition, int width, int height, float spacing, float radius) {
    float mass = 1;
    sf::Vector2f velocity = sf::Vector2f(0.f, 0.f); //particles have 0 velocity to start
    sf::Vector2f particlePosition = startPosition; //determines where new particle is going to be created
    sf::Vector2f rowPosition = startPosition;
    int particleNumber = 0;

    for (int i = 0; i < height; i++) { //for each row in the height
        for (int j = 0; j < width; j++) { //this creates rows of particles
            if (j == 0 || j == width - 1) {
                addParticle(radius, particlePosition, velocity, sf::Color::Red, true, mass); //Adds a fixed particle to the first and last particle in a row
            }
            else {
                addParticle(radius, particlePosition, velocity, sf::Color::Blue, false, mass); //Adds un fixed particles to the rest of the row
            }
            particlePosition += sf::Vector2f(spacing, 0.f); //adds 20 to x position
        }
        rowPosition += sf::Vector2f(0.f, spacing); //resets position and adds 20 to y position
        particlePosition = rowPosition; //Set first particle to new row position
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width - 1; j++) { //this adds springs between particles in each row
            addSpring(particles[particleNumber], particles[particleNumber + 1], spacing); //join particle to previous particle
            particleNumber += 1;
        }
        particleNumber += 1;
    }
    particleNumber = 0;
    for (int i = 0; i < height-1; i++) {
        for (int j = 0; j < width; j++) { //this adds springs between particles in each column
            addSpring(particles[particleNumber], particles[particleNumber + width], spacing); //join particle to particle in next row
            particleNumber += 1;
        }
    }
    particleNumber = 0;
    for (int i = 0; i < height - 1; i++) { //for rows 0,1,2
        for (int j = 0; j < width - 1; j++) { //for particles 0,1,2,3
            addSpring(particles[particleNumber], particles[particleNumber + width + 1], sqrt(2 * spacing * spacing));
            addSpring(particles[particleNumber + 1], particles[particleNumber + width], sqrt(2 * spacing * spacing));
            particleNumber += 1;
            if (j == width - 2) {
                particleNumber += 1;
            }
        }
    }
}

void ParticleSystem::checkParticleCollision(const sf::Vector2u& windowSize, float dt) {
    
    int repulsionFactor = -2000;
    float dampingPercentage = 0.9999; //To dampen particles touching each other
    
    for (Particle& p1 : particles) { //for a particle
        for (Particle& p2 : particles) { //compare particle to every other particle
            if (p1.getPosition() == p2.getPosition()) {
                continue;
            }

            sf::Vector2f displacement = p2.getPosition() - p1.getPosition(); //difference in distance between the x and y coordinates of each particle
            float distance = calculateMagnitude(displacement); //gets the magnitude
            if (distance < p1.shape.getRadius() + p2.shape.getRadius()) { //if the distnce between to cicircle is less than their combined radius then it is colliding
                //Project velocities into components along normal
                //normalVelcoities is the particles velocity in direction of the normal vector

                sf::Vector2f repulsionForce = (p1.shape.getRadius() + p2.shape.getRadius() - distance) * repulsionFactor * (displacement / distance); //closer the particles get to each other the larger the repulsion force
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
                //float finalNormalVelocityP1 = normalVelocityP2;
                //float finalNormalVelocityP2 = normalVelocityP1;

                float m1 = p1.getMass();
                float m2 = p2.getMass();
                float totalMass = m1 + m2;

                float finalNormalVelocityP1 = ((m1 - m2) * normalVelocityP1 + 2 * m2 * normalVelocityP2) / totalMass;
                float finalNormalVelocityP2 = ((m2 - m1) * normalVelocityP2 + 2 * m1 * normalVelocityP1) / totalMass;

                // Update velocities
                p1.velocity = (finalNormalVelocityP1 * normal + tangentVelocityP1 * tangent) * dampingPercentage;
                p2.velocity = (finalNormalVelocityP2 * normal + tangentVelocityP2 * tangent) * dampingPercentage;
            }
        }
    }
}