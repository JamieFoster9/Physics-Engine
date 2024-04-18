#include <vector>
#include <list>
#include "Spring.h"
using namespace std;

class ParticleSystem {
public:
    vector<Particle> particles;
    list<Spring> springs;

    //Creates particle and inserts it into particles vector
    void addParticle(float radius, sf::Vector2f position, sf::Vector2f velocity, sf::Color color, bool fixedParticle, float mass);

    //Creates a spring and adds it into a springs vector
    void addSpring(Particle& particle1, Particle& particle2, float defaultLength);

    //Updates all of the Particles and springs in the system
    void update(float dt, const sf::Vector2u& windowSize);

    //Renders the window and particles inside it
    void draw(sf::RenderWindow& window);

    //Creates a grid of particles and springs
    void addRectangle(sf::Vector2f startPosition, int width, int height, float spacing, float radius);

    //Creates a grid of particles and springs in a fibre pattern
    void addFibres(sf::Vector2f startPosition, int width, int height, float spacing, float radius);

    //Calculates the magnitude
    float calculateMagnitude(sf::Vector2f vector);

    //Calculates the dot product 
    float dotProduct(sf::Vector2f velocity, sf::Vector2f normal);

    //for every particle we want to compare it to every other particle to check if it is in its boundary
    void checkParticleCollision(const sf::Vector2u& windowSize, float dt);
};