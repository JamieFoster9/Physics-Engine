//#include "Particle.h"
#include "Spring.h"
using namespace std;

class ParticleSystem {
public:
    vector<Particle> particles;
    vector<Spring> springs;

    //Creates particle and inserts it into particles vector
    void addParticle(float radius, const sf::Vector2f& position, const sf::Vector2f& velocity, const sf::Color& color, bool fixedParticle);

    //inserts particle into particles vector
    void addSpring(Particle& particle1, Particle& particle2, float length);

    //Updates all of the Particles and springs in the system
    void update(float dt, const sf::Vector2u& windowSize);

    //Renders the window and particles inside it
    void draw(sf::RenderWindow& window);

    //Creates a grid of particles and springs
    void addRectangle(sf::Vector2f startPosition, int width, int height);

    //Calculates the magnitude
    float calculateMagnitude(sf::Vector2f& vector);

    //Calculates the dot product 
    float dotProduct(const sf::Vector2f& velocity, const sf::Vector2f& normal);

    //for every particle we want to compare it to every other particle to check if it is in its boundary
    void checkParticleCollision(const sf::Vector2u& windowSize, float dt);
};