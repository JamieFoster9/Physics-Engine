#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

struct Particle {
    float mass;
    float gravity = 500;
    sf::CircleShape circle;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool fixedParticle = false;
    //position is the initial position of the particle but will update if the particle moves so we need a reference to it
    Particle(float radius, sf::Vector2f position, sf::Vector2f velocity, sf::Color color, bool fixedParticle, float mass)
        : circle(radius), position(position), velocity(velocity), fixedParticle(fixedParticle), mass(mass) {
        circle.setPosition(position);
        circle.setFillColor(color);
        circle.setOrigin(circle.getRadius(), circle.getRadius()); // set the circles position to the centre
    }

    sf::Vector2f getPosition();

    float getMass();

    float getGravity();

    //Applies a force to a particle and updates its position and velocity
    void applyForce(sf::Vector2f force, float dt);

    /*
    -Updates a particles position every dt in time passed.
    -Shifts the particle by velocity * dt = change in position.
    -v2 = v1 + dt * (F1/m) velocity is extrapolated based on the current force and previous velocity.
    -x2 = x1 + dt * v2 postion is extrapolated based on the previous position plus the change in 
    position from the new velocity.
    */
    void update(float dt);

    //Reverses a particles velocity when they are outside the window boundaries
    void checkBoundaryCollision(const sf::Vector2u& windowSize, float dt);
};