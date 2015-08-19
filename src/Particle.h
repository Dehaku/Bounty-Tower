#pragma once

#ifndef PARTICLE_H
#define PARTICLE_H

#include <list>
#include <memory>
#include <random>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>

/* Enum for particle distribution type */
namespace Shape { enum { CIRCLE, SQUARE }; }

/* Particle Structure */
struct Particle : public sf::Drawable
{
  /* Data Members */

  sf::Vertex drawVertex; /*< To replace pos */
  sf::Vector2f vel; // Velocity

  /* Member Functions */

  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const
    { target.draw(&drawVertex, 1, sf::Points, states); }

};

/* Typedefs */
typedef std::uniform_real_distribution<> UniRealDist;
typedef std::uniform_int_distribution<> UniIntDist;
typedef std::shared_ptr<Particle> ParticlePtr;

class ParticleSystem : public sf::Drawable
{
public:

  /* Constructors/Destructor */

  ParticleSystem(sf::Vector2u canvasSize);
  ~ParticleSystem(void);

  /* Getters and Setters */

  const int getDissolutionRate(void) const { return m_dissolutionRate; }
  const int getNumberOfParticles(void) const { return m_particles.size(); }
  const float getParticleSpeed(void) const { return m_particleSpeed; }
  const std::string getNumberOfParticlesString(void) const;

  void setCanvasSize(sf::Vector2u newSize) { m_canvasSize = newSize; }
  void setDissolutionRate(sf::Uint8 rate) { m_dissolutionRate = rate; }
  void setDissolve(void) { m_dissolve = !m_dissolve; }
  void setDistribution(void) { m_shape = (m_shape + 1) % 2; }
  void setGravity(float x, float y) { m_gravity.x = x; m_gravity.y = y; }
  void setGravity(sf::Vector2f gravity) { m_gravity = gravity; }
  void setParticleSpeed(float speed) { m_particleSpeed = speed; }
  void setPosition(float x, float y) { m_startPos.x = x; m_startPos.y = y; }
  void setPosition(sf::Vector2f position) { m_startPos = position; }
  void setShape(sf::Uint8 shape) { m_shape = shape; }

  /* Member Functions */

  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  void fuel(int particles);     /*< Adds new particles */
  void update(float deltaTime); /*< Updates particles */

private:

  /* Data Members */

  bool        m_dissolve;       /*< Dissolution enabled? */
  float       m_particleSpeed;  /*< Pixels per second (at most) */

  sf::Color   m_transparent;    /*< sf::Color(0, 0, 0, 0) */

  sf::Uint8   m_dissolutionRate;/*< Rate particles disolve */
  sf::Uint8   m_shape;          /*< Shape of distribution */

  sf::Vector2f    m_gravity;    /*< Influences particle velocities */
  sf::Vector2f    m_startPos;   /*< Particle origin */
  sf::Vector2u    m_canvasSize;/*< Limits of particle travel */

  /* Container */
  std::vector<ParticlePtr> m_particles;

};

#endif