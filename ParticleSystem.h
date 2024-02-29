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
};