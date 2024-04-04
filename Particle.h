#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

struct Particle {

    static float commonMass; //mass for all particle
    static float commonGravity; //gravity for all particles
    float mass;
    float gravity = 500;
    sf::CircleShape shape; //Shape representing circle
    sf::Vector2f position; //2D vector of type float includes both x and y coordinates for position and velocity
    sf::Vector2f velocity;
    bool fixedParticle = false;
    //position is the initial position of the particle but will update if the particle moves so we need a reference to it
    //: shape(radius), velocity(0.f, 0.f) this is the member initialiser list and this sets the radius and initial velocity
    Particle(float radius, sf::Vector2f position, sf::Vector2f velocity, sf::Color color, bool fixedParticle, float mass)
        : shape(radius), position(position), velocity(velocity), fixedParticle(fixedParticle), mass(mass) {
        shape.setPosition(position);
        shape.setFillColor(color);
        shape.setOrigin(shape.getRadius(), shape.getRadius()); // set the circles position to the centre
    }

    sf::Vector2f getPosition();

    float getMass();

    float getGravity();

    void setMass(float massValue);

    void setGravity(float gravityValue);

    //Force on particle from spring
    void applyForce(sf::Vector2f force, float dt);

    /*
    Updates a particles position every dt in time passed, this function shifts the particle by velocity * dt = change in position
    we want to update the new position based on the previous velocity
    v2 = v1 + h(F1/m) velocity is extrapolated based on the current force so it would be current velocity plus the change in velocity from acceration
    x2 = x1 + h*v2 postion is extrapolated based on the previous position plus the change in position from the new velocity
    h = dt
    */
    void update(float dt);

    //BUG:: particle keeps reversing velocity while it's in the boundary
    //reverses particles velocity when they are within the window boundary
    void checkBoundaryCollision(const sf::Vector2u& windowSize, float dt);
};