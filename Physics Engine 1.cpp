#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
using namespace std;
using namespace glm;

struct Particle {
    //Shape representing circle
    sf::CircleShape shape;
    //2D vector includes both x and y coordinates for position and velocity
    sf::Vector2f position;
    sf::Vector2f velocity;
    //float mass = 1;

    //position is the initial position of the particle
    //: shape(radius), velocity(0.f, 0.f) this is the member initialiser list and this sets the radius and initial velocity
    Particle(float radius, const sf::Vector2f& position, const sf::Color& color)
        : shape(radius), velocity(0.f, 0.f) {
        shape.setPosition(position);
        shape.setFillColor(color);
    }

    sf::Vector2f getPosition() { return position; }

    //force on particle from spring
    void applyForce(sf::Vector2f force, float dt) {
        float mass = 1000;
        sf::Vector2f acceleration = force / mass;
        sf::Vector2f velocityChange = acceleration * dt;
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
        position.x += dPositionx;
        //need to update velocity as it's changing with the acceleration of gravity with v2 = v1 + h(F1/m) or v2 = v1 + h*a1 as F/m = a which is constant for masses
        //velocity.y += gravity * dt;
        float dPositiony = (velocity.y * dt);
        position.y += dPositiony;
        shape.move(sf::Vector2f(dPositionx, dPositiony));
    }

    //reverses particles velocity when they hot the window boundary
    void checkBoundaryCollision(const sf::Vector2u& windowSize) {
        sf::Vector2f position = shape.getPosition();
        float radius = shape.getRadius();
        // Check collision with left and right edges
        if (position.x < 0 || position.x + (2 * radius) > windowSize.x) {
            velocity.x = -velocity.x;  // Invert the x component of velocity
        }
        // Check collision with top and bottom edges
        if (position.y < 0 || position.y + (2 * radius) > windowSize.y) {
            velocity.y = -velocity.y;  // Invert the y component of velocity
        }
    }
};

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
        sf::Vector2f displacement = particle2.getPosition() - particle1.getPosition(); //difference in distance between the x and y coordinates of each particle
        float distance = calculateMagnitude(displacement); //gets the magnitude
        float force = stiffness * (distance - length); //Force exerted by spring in direction of spring F = kx
        sf::Vector2f normalisedDisplacement = normalise(displacement);
        particle1.applyForce(normalisedDisplacement * force, dt);
        particle2.applyForce(normalisedDisplacement * (-force), dt);
    }
};

class ParticleSystem {
public:
    vector<Particle> particles;
    vector<Spring> springs;

    void addParticle(float radius, const sf::Vector2f& position, const sf::Color& color, const sf::Vector2f& velocity) {
        //inserts particle into particles vector
        particles.emplace_back(radius, position, color);
        particles.back().velocity = velocity;
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
        for (auto& particle : particles) {
            particle.update(dt);
            particle.checkBoundaryCollision(windowSize);
        }
        for (auto& spring : springs) {
            spring.applyForce(dt);
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

int main() {
    sf::RenderWindow window(sf::VideoMode(1400, 800), "SFML Particle System");
    sf::Clock clock; //starts clock
    sf::Time micro = sf::microseconds(10000);
    sf::Time milli = sf::milliseconds(10);
    sf::Time second = sf::seconds(0.01f);
    const float Speed = 50.f;
    float Left = 0.f;
    float Top = 0.f;
    sf::Text text;
    sf::Font font;
    font.loadFromFile("Fonts/Montserrat.otf");
    text.setFont(font);
    text.setString("0");
    text.setCharacterSize(50);
    text.setPosition(20.f, 10.f);
    text.setFillColor(sf::Color::Black);

    ParticleSystem particleSystem;
    particleSystem.addParticle(10.f, sf::Vector2f(100.f, 400.f), sf::Color::Blue, sf::Vector2f(0.f, 0.f)); //1
    particleSystem.addParticle(10.f, sf::Vector2f(150.f, 400.f), sf::Color::Blue, sf::Vector2f(0.f, 0.f)); //2
    particleSystem.addParticle(10.f, sf::Vector2f(200.f, 400.f), sf::Color::Blue, sf::Vector2f(0.f, 0.f)); //3
    particleSystem.addParticle(10.f, sf::Vector2f(250.f, 400.f), sf::Color::Blue, sf::Vector2f(0.f, 0.f)); //4
    particleSystem.addParticle(10.f, sf::Vector2f(300.f, 400.f), sf::Color::Blue, sf::Vector2f(0.f, 0.f)); //5
    particleSystem.addParticle(10.f, sf::Vector2f(300.f, 350.f), sf::Color::Blue, sf::Vector2f(0.f, 0.f)); //6
    particleSystem.addParticle(10.f, sf::Vector2f(250.f, 350.f), sf::Color::Blue, sf::Vector2f(0.f, 0.f)); //7
    particleSystem.addParticle(10.f, sf::Vector2f(200.f, 350.f), sf::Color::Blue, sf::Vector2f(0.f, 0.f)); //8
    particleSystem.addParticle(10.f, sf::Vector2f(150.f, 350.f), sf::Color::Blue, sf::Vector2f(0.f, 0.f)); //9
    particleSystem.addParticle(10.f, sf::Vector2f(100.f, 350.f), sf::Color::Blue, sf::Vector2f(0.f, 0.f)); //10
    particleSystem.addSpring(particleSystem.particles[0], particleSystem.particles[1], 10000, 0, 10); //created a spring between particle 1 and 2
    particleSystem.addSpring(particleSystem.particles[1], particleSystem.particles[2], 10000, 0, 10); //created a spring between particle 2 and 3
    particleSystem.addSpring(particleSystem.particles[2], particleSystem.particles[3], 10000, 0, 10); //created a spring between particle 3 and 1
    particleSystem.addSpring(particleSystem.particles[3], particleSystem.particles[4], 10000, 0, 10); //created a spring between particle 1 and 2
    particleSystem.addSpring(particleSystem.particles[4], particleSystem.particles[5], 10000, 0, 10); //created a spring between particle 2 and 3
    particleSystem.addSpring(particleSystem.particles[5], particleSystem.particles[6], 10000, 0, 10); //created a spring between particle 3 and 1
    particleSystem.addSpring(particleSystem.particles[6], particleSystem.particles[7], 10000, 0, 10); //created a spring between particle 1 and 2
    particleSystem.addSpring(particleSystem.particles[7], particleSystem.particles[8], 10000, 0, 10); //created a spring between particle 2 and 3
    particleSystem.addSpring(particleSystem.particles[8], particleSystem.particles[9], 10000, 0, 10); //created a spring between particle 3 and 1
    particleSystem.addSpring(particleSystem.particles[9], particleSystem.particles[0], 10000, 0, 10); //created a spring between particle 3 and 1

    //run the program as long as the window is open
    while (window.isOpen()) {
        //check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            //"close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.getElapsedTime().asSeconds();

        clock.restart();

        //if (window.GetInput().IsKeyDown(sf::Key::Left))  Left -= Speed * dt;
        //if (window.GetInput().IsKeyDown(sf::Key::Right)) Left += Speed * dt;
        //if (window.GetInput().IsKeyDown(sf::Key::Up))    Top -= Speed * dt;
        //if (window.GetInput().IsKeyDown(sf::Key::Down))  Top += Speed * dt;

        float framerate = 1 / dt; //Framerate in Hz
        string framesText = to_string(framerate);
        text.setString(framesText + " fps");
        window.draw(text);
        particleSystem.update(dt, window.getSize());
        window.clear(sf::Color::White);
        particleSystem.draw(window);
        window.draw(text);
        window.display();;
    }

    return 0;
}