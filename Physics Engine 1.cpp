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
struct Particle {
    sf::CircleShape shape;
    vec2 velocity;

    Particle(float radius, const sf::Vector2f& position, const sf::Color& color)
        : shape(radius), velocity(0.f, 0.f) {
        shape.setPosition(position);
        shape.setFillColor(color);
    }

    void update(float dt) {
        shape.move(sf::Vector2f(velocity.x, velocity.y) * dt);
    }

    void checkBoundaryCollision(const sf::Vector2u& windowSize) {
        sf::Vector2f position = shape.getPosition();
        float radius = shape.getRadius();

        // Check collision with left and right edges
        if (position.x - radius < 0 || position.x + radius > windowSize.x) {
            velocity.x = -velocity.x;  // Invert the x component of velocity
        }

        // Check collision with top and bottom edges
        if (position.y - radius < 0 || position.y + radius > windowSize.y) {
            velocity.y = -velocity.y;  // Invert the y component of velocity
        }
    }
};

class ParticleSystem {
public:
    vector<Particle> particles;

    void addParticle(float radius, const sf::Vector2f& position, const sf::Color& color, const vec2& velocity) {
        particles.emplace_back(radius, position, color);
        particles.back().velocity = velocity;
    }

    void update(float dt, const sf::Vector2u& windowSize) {
        for (auto& particle : particles) {
            particle.update(dt);
            particle.checkBoundaryCollision(windowSize);
        }
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& particle : particles) {
            window.draw(particle.shape);
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Particle System");
    sf::Clock clock;

    ParticleSystem particleSystem;
    particleSystem.addParticle(50.f, sf::Vector2f(300.f, 300.f), sf::Color::Red, vec2(200.f, 50.f));
    particleSystem.addParticle(50.f, sf::Vector2f(300.f, 300.f), sf::Color::Blue, vec2(-100.f, 200.f));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        particleSystem.update(dt, window.getSize());

        window.clear(sf::Color::White);
        particleSystem.draw(window);
        window.display();
    }

    return 0;
}