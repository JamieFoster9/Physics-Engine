#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

struct Particle {
    sf::CircleShape shape; //Shape representing circle
    sf::Vector2f position; //2D vector of type float includes both x and y coordinates for position and velocity
    sf::Vector2f velocity;
    bool fixedParticle = false;

    //position is the initial position of the particle but will update if the particle moves so we need a reference to it
    //: shape(radius), velocity(0.f, 0.f) this is the member initialiser list and this sets the radius and initial velocity
    Particle(float radius, const sf::Vector2f& position, const sf::Vector2f& velocity,const sf::Color& color, bool fixedParticle)
        : shape(radius), position(position), velocity(velocity), fixedParticle(fixedParticle) {
        shape.setPosition(position);
        shape.setFillColor(color);
    }

    //Gets position of the particle
    sf::Vector2f getPosition();

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
    void checkBoundaryCollision(const sf::Vector2u& windowSize);

    /*
    void checkBoundaryCollision(const sf::Vector2u& windowSize, Particle& particle) {
        sf::Vector2f position = shape.getPosition();
        float radius = shape.getRadius();

        // Check collision with left and right edges
        if (!particle.colliding) {
            if (position.x <= 0 || position.x + (2 * radius) >= windowSize.x) { //Check collision with left and right edges
                velocity.x = -velocity.x;  // Invert the x component of velocity
                particle.colliding = true;
            }

            if (position.y <= 0 || position.y + (2 * radius) >= windowSize.y) { // Check collision with top and bottom edges
                velocity.y = -velocity.y;  // Invert the y component of velocity
                particle.colliding = true;
            }
        }
        else {
            if (position.x > 0 && position.x + (2 * radius) > windowSize.x) { //Check collision with left and right edges
                particle.colliding = false;
            }
            if (position.y > 0 && position.y + (2 * radius) > windowSize.y) { // Check collision with top and bottom edges
                particle.colliding = false;
            }
        }
    }
    */
};