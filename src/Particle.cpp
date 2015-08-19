#include "particle.h"

/************************************************************/
ParticleSystem::ParticleSystem(sf::Vector2u canvasSize) :
  m_dissolve(false),
  m_particleSpeed(100.0f),
  m_transparent(sf::Color(0,0,0,0)),
  m_dissolutionRate(4),
  m_shape(Shape::CIRCLE),
  m_gravity(sf::Vector2f(0.0f,0.0f)),
  m_canvasSize(canvasSize)
{
  m_startPos =
    sf::Vector2f(
      static_cast<float>(canvasSize.x) / 2,
      static_cast<float>(canvasSize.y) / 2);
}

/************************************************************/
ParticleSystem::~ParticleSystem(void)
{
  /* Clear vector */
  m_particles.clear();
}

/************************************************************/
void ParticleSystem::draw(
  sf::RenderTarget& target, sf::RenderStates states) const
{
  for(const auto& item : m_particles)
    target.draw(&item.get()->drawVertex, 1, sf::Points);
  return;
}

/************************************************************/
void ParticleSystem::fuel(int particles)
{
  for(int i = 0; i < particles; i++)
  {
    /* Generate a new particle and put it at the generation point */
    Particle* particle;
    particle = new Particle();
    particle->drawVertex.position.x = m_startPos.x;
    particle->drawVertex.position.y = m_startPos.y;

    /* Randomizer initialization */
    std::random_device rd;
    std::mt19937 gen(rd());

    switch(m_shape)
    {
      case Shape::CIRCLE:
      {
        /* Generate a random angle */
        UniRealDist randomAngle(0.0f, (2.0f * 3.14159265));
        float angle = randomAngle(gen);

        /* Use the random angle as a thrust vector for the particle */
        UniRealDist randomAngleCos(0.0f, cos(angle));
        UniRealDist randomAngleSin(0.0f, sin(angle));
        particle->vel.x = randomAngleCos(gen);
        particle->vel.y = randomAngleSin(gen);

        break;
      }
      case Shape::SQUARE:
      {
        /* Square generation */
        UniRealDist randomSide(-1.0f, 1.0f);
        particle->vel.x = randomSide(gen);
        particle->vel.y = randomSide(gen);

        break;
      }
      default:
      {
        particle->vel.x = 0.5f; // Easily detected
        particle->vel.y = 0.5f; // Easily detected
      }
    }

    /* We don't want lame particles. Reject, start over. */
    if(particle->vel.x == 0.0f && particle->vel.y == 0.0f )
    {
      delete particle;
      continue;
    }

    /* Randomly change the colors of the particles */
    UniIntDist randomColor(0, 255);
    particle->drawVertex.color.r = randomColor(gen);
    particle->drawVertex.color.g = randomColor(gen);
    particle->drawVertex.color.b = randomColor(gen);
    particle->drawVertex.color.a = 255;

    m_particles.push_back(ParticlePtr(particle));
  }

  return;
}

/************************************************************/
const std::string ParticleSystem::getNumberOfParticlesString(void) const
{
    std::ostringstream oss;
    oss << m_particles.size();
    return oss.str();
}

/************************************************************/
void ParticleSystem::update(float deltaTime)
{
  /* Run through each particle and apply our system to it */
  for(auto it = m_particles.begin(); it != m_particles.end(); it++)
  {
    /* Apply Gravity */
    (*it)->vel.x += m_gravity.x * deltaTime;
    (*it)->vel.y += m_gravity.y * deltaTime;

    /* Apply thrust */
    (*it)->drawVertex.position.x += (*it)->vel.x * deltaTime * m_particleSpeed;
    (*it)->drawVertex.position.y += (*it)->vel.y * deltaTime * m_particleSpeed;

    /* If they are set to disolve, disolve */
    if(m_dissolve) (*it)->drawVertex.color.a -= m_dissolutionRate;

    if((*it)->drawVertex.position.x > m_canvasSize.x
       || (*it)->drawVertex.position.x < 0
       || (*it)->drawVertex.position.y > m_canvasSize.y
       || (*it)->drawVertex.position.y < 0
       || (*it)->drawVertex.color.a < 10)
    {
      it = m_particles.erase(it);
      if(it == m_particles.end()) return;
    }
  }

  return;
}