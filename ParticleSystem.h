using namespace std;

class ParticleSystem {
public:
    vector<Particle> particles;
    vector<Spring> springs;

    void addParticle(float radius, const sf::Vector2f& position, const sf::Vector2f& velocity, const sf::Color& color) {
        //inserts particle into particles vector
        particles.emplace_back(radius, position, velocity , color);
    }


    void addSpring(Particle& particle1, Particle& particle2, float stiffness, float damping, float length) {
        //inserts particle into particles vector
        Spring spring = Spring(particle1, particle2, stiffness, damping, length);
        springs.emplace_back(spring);
    }

    //Update all of the Particles in the system

    //for each particle
    //update particle based on current speed and gravity
    //check if there's a boundary


    void update(float dt, const sf::Vector2u& windowSize) {
        for (auto& spring : springs) {
            spring.applyForce(dt);
        }
        for (auto& particle : particles) {
            particle.update(dt);
            particle.checkBoundaryCollision(windowSize);
        }
    }
    //Renders the window and particles inside it
    void draw(sf::RenderWindow& window) {
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

    //Adds 2 particle
    //Adds spring to connect particle then repeat
    //choose a starting ppsition
    void addRectangle(sf::Vector2f startPosition, int width, int height) {
        sf::Vector2f velocity = sf::Vector2f(0.f, 0.f); //particles have 0 velocity to start

        sf::Vector2f particlePosition = startPosition; //determines where new particle is going to be created
        sf::Vector2f rowPosition = startPosition;
        int particleNumber = 0;

        for (int i = 0; i < height; i++) { //for each row in the height

            for (int j = 0; j < width; j++) { //this creates rows of particles
                addParticle(5.f, particlePosition, velocity, sf::Color::Blue); //next particle in width
                particlePosition += sf::Vector2f(20.f, 0.f); //adds 20 to x position
            }

            rowPosition += sf::Vector2f(0.f, 20.f); //resets position and adds 20 to y position
            particlePosition = rowPosition; //Set first particle to new row position
        }

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width - 1; j++) { //this adds springs between particles
                addSpring(particles[particleNumber], particles[particleNumber + 1], 100, 0, 20); //join particle to previous particle
                particleNumber += 1;
            }
            particleNumber += 1;
        }

        particleNumber = 0;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width - 1; j++) { //this adds springs between particles
                addSpring(particles[particleNumber], particles[particleNumber + width], 10000, 0, 20); //join particle to previous particle
                particleNumber += 1;
            }
        }

        /*
        for (int j = 0; j < height - 1; j++) {
            sf::Vector2f rowStartPos = newPosition - sf::Vector2f(width * 20.f, 0.f) + sf::Vector2f(0.f, 20.f * (j + 1));
            for (int i = 0; i < width; i++) {
                addParticle(5.f, rowStartPos, velocity, sf::Color::Blue)); // Add particle
                rowStartPos += sf::Vector2f(10.f, 0.f); // Move to the next position in the row
            }
            for (int i = j * width; i < (j + 1) * width - 1; i++) {
                addSpring(particles[i], particles[i + 1], 1000, 0, 100); // Join particle to next particle in the row
            }
        }
        */
    }
};