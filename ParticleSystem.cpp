#include "ParticleSystem.h"
using namespace std;

void ParticleSystem::addParticle(float radius, sf::Vector2f position, sf::Vector2f velocity, sf::Color color, bool fixedParticle, float mass) {
    particles.emplace_back(radius, position, velocity, color, fixedParticle, mass); 
}

void ParticleSystem::addSpring(Particle& particle1, Particle& particle2, float defaultLength) {
    springs.emplace_back(particle1, particle2, defaultLength);
}

void ParticleSystem::update(float dt, const sf::Vector2u& windowSize) {

    //Updates particle on it's normal course without gravity
    for (auto& particle : particles) {
        particle.update(dt);
        particle.checkBoundaryCollision(windowSize, dt);
    }

    checkParticleCollision(windowSize, dt);

    //Stores temporary calculated spring forces
    vector<sf::Vector2f> forces;

    //Calculates the current forces across every spring
    for (auto it = springs.begin(); it != springs.end(); ++it) {
        forces.push_back(it->calculateForce());
    }

    //Applies the forces from the forces vector to every particle
    int index = 0;
    for (auto it = springs.begin(); it != springs.end(); ++it) {
        it->particle1.applyForce(forces[index], dt);
        it->particle2.applyForce(-forces[index], dt);
        ++index;
    }

    //Removes spring if it exceeds the breaklength
    if (!springs.empty()) {
        for (auto it = springs.begin(); it != springs.end();) {
            if (it->shouldRemoveSpring()) {
                //Erase the current element and get the iterator to the next element
                it = springs.erase(it);
            } else {
                ++it;
            }
        }
    }

    checkParticleCollision(windowSize, dt);
}

float ParticleSystem::calculateMagnitude(sf::Vector2f vector) {
    return sqrt((vector.x * vector.x) + (vector.y * vector.y));
}

float ParticleSystem::dotProduct(sf::Vector2f velocity, sf::Vector2f normal) {
    return velocity.x * normal.x + velocity.y * normal.y;
}

void ParticleSystem::draw(sf::RenderWindow& window) {
    for (auto particle : particles) {
        window.draw(particle.circle);
    }
    for (auto spring : springs) {
        //Code adapted from SFML, n.d-d:
        sf::VertexArray line(sf::Lines, 2);
        //Set the end points of the lines to be the centres of the particles
        line[0].position = spring.particle1.circle.getPosition(), spring.particle1.circle.getRadius();
        line[1].position = spring.particle2.circle.getPosition(), spring.particle2.circle.getRadius();
        line[0].color = sf::Color::Black;
        line[1].color = sf::Color::Black;
        window.draw(line);
        //End of adapted code
    }
}

void ParticleSystem::addRectangle(sf::Vector2f startPosition, int width, int height, float spacing, float radius) {
    float mass = 1;
    sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
    sf::Vector2f particlePosition = startPosition;
    sf::Vector2f rowPosition = startPosition;
    int particleNumber = 0;

    //Adds particles in rows and columns with equal spacing
    //for each row in the height
    for (int i = 0; i < height; i++) {
        //for each particle in row
        for (int j = 0; j < width; j++) {
            //Adds fixed particles to first and last particle in each row
            if (j == 0 || j == width - 1) {
                addParticle(radius, particlePosition, velocity, sf::Color::Red, true, mass);
            } else {
                addParticle(radius, particlePosition, velocity, sf::Color::Blue, false, mass);
            }
            //adds spacing to the particles x position to add the next particle
            particlePosition += sf::Vector2f(spacing, 0.f);
        }
        //resets x position and adds spacing to y position to begin the next row
        rowPosition += sf::Vector2f(0.f, spacing); 
        particlePosition = rowPosition;
    }

    //Horizontal springs between each row
    for (int i = 0; i < height; i++) {
        //springs in row = width - 1, as 1 spring is needed to join two particles
        for (int j = 0; j < width - 1; j++) {
            //join particle to next particle
            addSpring(particles[particleNumber], particles[particleNumber + 1], spacing);
            particleNumber += 1;
        }
        //skips the last particle to the particle in the next row
        particleNumber += 1;
    }

    //Verticle springs between each row
    particleNumber = 0;
    for (int i = 0; i < height-1; i++) {
        for (int j = 0; j < width; j++) {
            //join particle to particle in the next row
            addSpring(particles[particleNumber], particles[particleNumber + width], spacing);
            particleNumber += 1;
        }
    }

    //Diagonal spring between each row
    particleNumber = 0;
    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width - 1; j++) {
            addSpring(particles[particleNumber], particles[particleNumber + width + 1], sqrt(2 * spacing * spacing));
            addSpring(particles[particleNumber + 1], particles[particleNumber + width], sqrt(2 * spacing * spacing));
            particleNumber += 1;
            //skip last particle in row
            if (j == width - 2) {
                particleNumber += 1;
            }
        }
    }
}

void ParticleSystem::addFibres(sf::Vector2f startPosition, int width, int height, float spacing, float radius) {
    float mass = 1;
    sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
    sf::Vector2f particlePosition = startPosition;
    sf::Vector2f rowPosition = startPosition;
    int particleNumber = 0;

    //Adds particles in rows and columns with equal spacing
    //for each row in the height
    for (int i = 0; i < height; i++) {
        //for each particle in row
        for (int j = 0; j < width; j++) {
            //Adds fixed particles to first and last particle in each row
            if (j == 0 || j == width - 1) {
                addParticle(radius, particlePosition, velocity, sf::Color::Red, true, mass);
            }
            else {
                addParticle(radius, particlePosition, velocity, sf::Color::Blue, false, mass);
            }
            //adds spacing to the particles x position to add the next particle
            particlePosition += sf::Vector2f(spacing, 0.f);
        }
        //resets x position and adds spacing to y position to begin the next row
        rowPosition += sf::Vector2f(0.f, spacing);
        particlePosition = rowPosition;
    }

    //Horizontal springs between each row
    for (int i = 0; i < height; i++) {
        //springs in row = width - 1, as 1 spring is needed to join two particles
        for (int j = 0; j < width - 1; j++) {
            //join particle to next particle
            addSpring(particles[particleNumber], particles[particleNumber + 1], spacing);
            particleNumber += 1;
        }
        //skips the last particle to the particle in the next row
        particleNumber += 1;
    }

    //Verticle springs between each row
    particleNumber = 0;
    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width; j++) {
            //join particle to particle in the next row
            addSpring(particles[particleNumber], particles[particleNumber + width], spacing);
            particleNumber += 1;
        }
    }

    //Diagonal spring between each row
    particleNumber = 0;
    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width - 1; j++) {
            addSpring(particles[particleNumber], particles[particleNumber + width + 1], sqrt(2 * spacing * spacing));
            addSpring(particles[particleNumber + 1], particles[particleNumber + width], sqrt(2 * spacing * spacing));
            particleNumber += 1;
            //skip last particle in row
            if (j == width - 2) {
                particleNumber += 1;
            }
        }
        ++i;
        particleNumber += width;
    }
}

void ParticleSystem::checkParticleCollision(const sf::Vector2u& windowSize, float dt) {
    int repulsionFactor = -2000;
    float dampingPercentage = 0.9999;
    
    for (Particle& p1 : particles) {
        for (Particle& p2 : particles) {
            if (p1.getPosition() == p2.getPosition()) {
                continue;
            }
            sf::Vector2f displacement = p2.getPosition() - p1.getPosition();
            float distance = calculateMagnitude(displacement);
            float combinedRadius = p1.circle.getRadius() + p2.circle.getRadius();
            if (distance < combinedRadius) {
                //Code adapted from Moussa, 2023
                //The more the particles overlap the larger the repulsion force to escape the boundary
                sf::Vector2f repulsionForce = (combinedRadius - distance) * repulsionFactor * (displacement / distance);
                p1.applyForce(repulsionForce,dt);
                p2.applyForce(-repulsionForce,dt);
                //end of adapted code

                //Code adapted from Fox, n.d. 
                //unit normal vector
                sf::Vector2f normal = displacement / distance;
                //Unit tangent vector
                sf::Vector2f tangent(-displacement.y / distance, displacement.x / distance);

                //Break the particles velocities into components of the normal and tangent
                float normalVelocityP1 = dotProduct(p1.velocity, normal);
                float tangentVelocityP1 = dotProduct(p1.velocity, tangent);
                float normalVelocityP2 = dotProduct(p2.velocity, normal);
                float tangentVelocityP2 = dotProduct(p2.velocity, tangent);

                float m1 = p1.getMass();
                float m2 = p2.getMass();
                float totalMass = m1 + m2;

                //Final normal velocities according to the law of conservation of momentum and energy equations
                float finalNormalVelocityP1 = ((m1 - m2) * normalVelocityP1 + 2 * m2 * normalVelocityP2) / totalMass;
                float finalNormalVelocityP2 = ((m2 - m1) * normalVelocityP2 + 2 * m1 * normalVelocityP1) / totalMass;

                //Resolves normal and tangent velocities back to components of x and y
                p1.velocity = (finalNormalVelocityP1 * normal + tangentVelocityP1 * tangent) * dampingPercentage;
                p2.velocity = (finalNormalVelocityP2 * normal + tangentVelocityP2 * tangent) * dampingPercentage;
                //end of adaped code
            }
        }
    }
}