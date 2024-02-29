struct Spring {
    //member variables
    Particle& particle1;
    Particle& particle2;
    float stiffness; //k = F/x
    float damping = 0; //F = c * v; damping force equals the coefficient * change in velocithy
    float length; //default length between particles

    //Constructor to initialise properties of the spring
    //spring has to be connected to 2 particles
    Spring(Particle& particle1, Particle& particle2, float stiffness, float damping, float length)
        : particle1(particle1), particle2(particle2), stiffness(stiffness), damping(damping), length(length) {}

    float calculateMagnitude(sf::Vector2f& vector) {
        return sqrt((vector.x * vector.x) + (vector.y * vector.y));
    }

    sf::Vector2f normalise(const sf::Vector2f& vector) {
        float magnitude = sqrt(vector.x * vector.x + vector.y * vector.y);
        if (magnitude != 0) {
            return sf::Vector2f(vector.x / magnitude, vector.y / magnitude);
        }
        else {
            // Handle division by zero or zero-length vectors
            return sf::Vector2f(0, 0);
        }
    }

    //Applies force from spring to particles
    void applyForce(float dt) {
        sf::Vector2f displacement = particle2.getPosition() - particle1.getPosition(); //difference in distance between the x and y coordinates of each particle //400
        float distance = calculateMagnitude(displacement); //gets the magnitude //400
        float force = stiffness * (distance - length); //Force exerted by spring in direction of spring F = kx //100 * (400 - 100) = 300000
        sf::Vector2f direction = normalise(displacement); //(1,0)
        particle1.applyForce(direction * force, dt); //(30000, 0)
        particle2.applyForce(direction * (-force), dt);//(-30000, 0)
    }
};