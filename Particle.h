struct Particle {
    sf::CircleShape shape; //Shape representing circle
    sf::Vector2f position; //2D vector of type float includes both x and y coordinates for position and velocity
    sf::Vector2f velocity;

    //position is the initial position of the particle but will update if the particle moves so we need a reference to it
    //: shape(radius), velocity(0.f, 0.f) this is the member initialiser list and this sets the radius and initial velocity
    Particle(float radius, const sf::Vector2f& position, const sf::Vector2f& velocity,const sf::Color& color)
        : shape(radius), position(position), velocity(velocity) {
        shape.setPosition(position);
        shape.setFillColor(color);
    }

    sf::Vector2f getPosition() { return position; }

    //force on particle from spring
    void applyForce(sf::Vector2f force, float dt) {
        float mass = 10;
        sf::Vector2f acceleration = force / mass; //(30000, 0)/10 = (3000,0)
        sf::Vector2f velocityChange = acceleration * dt; //(3000, 0) * 1/60 = (50, 0)
        velocity += velocityChange;
        sf::Vector2f positionChange = velocityChange * dt;
        position += positionChange;
        shape.move(sf::Vector2f(positionChange));
    }

    //Updates particle position every dt in time passed, this function shifts the particle by velocity * dt = change in position
    //we want to update the new position based on the previous 
    //v2 = v1 + h(F1/m) velocity is extrapolated based on the current force so it would be current velocity plus the change in velocity from acceration
    //x2 = x1 + h*v2 postion is extrapolated based on the previous position plus the change in position from the new velocity
    //h = dt
    void update(float dt) {
        //To do: should be 9.81 m/s2 but scale it for the frame scale
        float gravity = 1000;
        float dPositionx = velocity.x * dt;
        float dPositiony = velocity.y * dt;
        //velocity.y += gravity * dt; //need to update velocity as it's changing with the acceleration of gravity with v2 = v1 + h(F1/m) or v2 = v1 + h*a1 as F/m = a which is constant for masses
        position.x += dPositionx;
        position.y += dPositiony;
        shape.move(sf::Vector2f(dPositionx, dPositiony));
    }

    //To do particle keeps reversing velocity while it's in the boundary
    //reverses particles velocity when they hot the window boundary
    void checkBoundaryCollision(const sf::Vector2u& windowSize) {
        sf::Vector2f position = shape.getPosition();
        float radius = shape.getRadius();
        // Check collision with left and right edges
        if (position.x <= 0 || position.x + (2 * radius) >= windowSize.x) {
            velocity.x = -velocity.x;  // Invert the x component of velocity
        }
        // Check collision with top and bottom edges
        if (position.y <= 0 || position.y + (2 * radius) >= windowSize.y) {
            velocity.y = -velocity.y;  // Invert the y component of velocity
        }
    }
};