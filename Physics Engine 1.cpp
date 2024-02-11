#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace glm;

/*
int main()
{
    //Creates the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    sf::CircleShape shape(10.f);
    shape.setFillColor(sf::Color::Black);

    sf::Font font;
    font.loadFromFile("Fonts/Bodycount.otf");

    sf::Text text;

    // select the font
    text.setFont(font); // font is a sf::Font

    // set the string to display
    text.setString("It'll be ogre soon!");

    // set the character size
    text.setCharacterSize(50); // in pixels, not points!

    // set the color
    text.setFillColor(sf::Color::Green);

    // set the text style
    //text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    text.move(sf::Vector2f(100.f, 0.f));

    sf::Texture texture;
    texture.loadFromFile("Images/Shrek.png");
    texture.setSmooth(true);

    sf::Sprite sprite;
    sprite.setTexture(texture);

    // position
    sprite.setPosition(sf::Vector2f(10.f, 50.f)); // absolute position
    sprite.move(sf::Vector2f(5.f, 10.f)); // offset relative to the current position

    // rotation
    //sprite.setRotation(90.f); // absolute angle
    //sprite.rotate(15.f); // offset relative to the current angle

    // scale
    //sprite.setScale(sf::Vector2f(0.5f, 2.f)); // absolute scale factor
    //sprite.scale(sf::Vector2f(1.5f, 3.f)); // factor relative to the current scale

    //run the program as long as the window is open
    while (window.isOpen())
    {
        //check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            //"close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //clear the window with white colour
        window.clear(sf::Color::White);

        //draw everything here
        window.draw(shape);

        // inside the main loop, between window.clear() and window.display()
        window.draw(text);

        window.draw(sprite);

        //end the current frame
        window.display();
    }

    return 0;
}


struct point_mass {
    vec2 position;
    vec2 velocity;
};

struct engine {
    vector<point_mass> points;
    vec2 gravity;

    void update(float dt) {
        for (auto& p : points)
            p.velocity += gravity * dt;

        for (auto& p : points)
            p.position += p.velocity * dt;
    }
};
*/

struct Spring {
    //member variables
    //Particle A
    //Particle B
    //Stiffness
    //Default length

    //Constructor to initialise properties of the spring
    /*
    Spring(Particle particleA, Particle particleB, float restLength, float stiffness)
        : particleA(particleA), particleB(particleB), length(restLength), stiffness(stiffness) {
    }
    */

    //need a method to update particles A position depending on particles B position
    /*
    void update(float dt) {
        float dPositionx = particleA velocity.x * dt;
        //need to update velocity as it's changing with the acceleration of gravity with v = u + at
        velocity.y += gravity * dt;
        float dPositiony = (velocity.y * dt);
        shape.move(sf::Vector2f(dPositionx, dPositiony));
    }
    */
};

struct Particle {
    //Shape representing circle
    sf::CircleShape shape;
    //2D vector includes both x and y coordinates for position and velocity
    sf::Vector2f position;
    sf::Vector2f velocity;
    float mass;

    //position is the initial position of the particle
    //: shape(radius), velocity(0.f, 0.f) this is the member initialiser list and this sets the radius and initial velocity
    Particle(float radius, const sf::Vector2f& position, const sf::Color& color)
        : shape(radius), velocity(0.f, 0.f), mass(mass) {
        shape.setPosition(position);
        shape.setFillColor(color);
    }

    //force on particle from external forces
    /*
    void applyForce(sf::Vector2f force) {
        sf::Vector2f acceleration = force / mass;
        velocity += acceleration
    }
    */

    //Updates particle position every dt in time passed, this function shifts the particle by velocity * dt = change in position
    //we want to update the new position based on the previous 
    //v2 = v1 + h(F1/m) velocity is extrapolated based on the current force so it would be current velocity plus the change in velocity from acceration
    //x2 = x1 + h*v2 postion is extrapolated based on the previous position plus the change in position from the new velocity
    //h = dt
    void update(float dt) {
        //To do: should be 9.81 m/s2 but scale it for the frame scale
        float gravity = 1000;
        float dPositionx = velocity.x * dt;
        //need to update velocity as it's changing with the acceleration of gravity with v2 = v1 + h(F1/m) or v2 = v1 + h*a1 as F/m = a which is constant for masses
        velocity.y += gravity * dt;
        float dPositiony = (velocity.y * dt);
        shape.move(sf::Vector2f(dPositionx, dPositiony));
    }

    //reverses particles velocity when they hot the window boundary
    void checkBoundaryCollision(const sf::Vector2u& windowSize) {
        sf::Vector2f position = shape.getPosition();
        float radius = shape.getRadius();
        // Check collision with left and right edges
        if (position.x < 0 || position.x + (2*radius) > windowSize.x) {
            velocity.x = -velocity.x;  // Invert the x component of velocity
        }
        // Check collision with top and bottom edges
        if (position.y < 0 || position.y + (2*radius) > windowSize.y) {
            velocity.y = -velocity.y;  // Invert the y component of velocity
        }
    }
};

class ParticleSystem {
public:
    vector<Particle> particles;

    void addParticle(float radius, const sf::Vector2f& position, const sf::Color& color, const sf::Vector2f& velocity) {
        //inserts particle into particles vector
        particles.emplace_back(radius, position, color);
        particles.back().velocity = velocity;
    }

    //Update all of the Particles in the system
    void update(float dt, const sf::Vector2u& windowSize) {
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
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1400, 800), "SFML Particle System");
    sf::Clock clock; //starts clock
    sf::Text text;
    sf::Font font;
    font.loadFromFile("Fonts/Montserrat.otf");
    text.setFont(font);
    text.setString("0");
    text.setCharacterSize(50);
    text.setPosition(20.f, 10.f);
    text.setFillColor(sf::Color::Black);

    ParticleSystem particleSystem;
    particleSystem.addParticle(10.f, sf::Vector2f(300.f, 100.f), sf::Color::Red, sf::Vector2f(300.f, -100.f));
    particleSystem.addParticle(10.f, sf::Vector2f(600.f, 100.f), sf::Color::Blue, sf::Vector2f(-500.f, 0.f));
    particleSystem.addParticle(10.f, sf::Vector2f(20.f, 100.f), sf::Color::Yellow, sf::Vector2f(-500.f, 0.f));

    //run the program as long as the window is open
    while (window.isOpen()) {
        //check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            //"close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds(); //restarts clock and returns time elapsed in seconds
        float framerate = 1 / dt; //Framerate in Hz
        string framesText = to_string(framerate);
        text.setString(framesText + " Hz");
        window.draw(text);
        particleSystem.update(dt, window.getSize());
        window.clear(sf::Color::White);
        particleSystem.draw(window);
        window.draw(text);
        window.display();;
    }

    return 0;
}