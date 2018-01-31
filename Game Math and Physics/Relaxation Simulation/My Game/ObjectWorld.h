/// \file ObjectWorld.h
/// \brief Interface for the object world class CObjectWorld.
///
/// Created by Ian Parberry to accompany his book
/// "Introduction to Game Physics with Box2D", published by CRC Press in 2013.
/// Copyright Ian Parberry, Laboratory for Recreational Computing,
/// Department of Computer Science & Engineering, University of North
/// Texas, Denton, TX, USA. URL: http://larc.unt.edu/ian.
///
/// This file is made available under the GNU All-Permissive License.
/// Copying and distribution of this file, with or without
/// modification, are permitted in any medium without royalty
/// provided the copyright notice and this notice are preserved.
/// This file is offered as-is, without any warranty.

#pragma once

#include <vector>

using namespace std;

#include "Particle.h"
#include "Spring.h"

/// \brief The object world.
///
/// The Object World is an abstract representation of all of the objects
/// in the game.

class CObjectWorld{
  friend class CBody;

  private:
    vector<CParticle*> m_stdPts; ///< Particle list.
    vector<CSpring*> m_stdSpr; ///< Spring list.

    CBody* m_pCurrentBody; ///< The first body.
    CBody* m_pCurrentBody2; ///< The second body.

    Vector2 m_vSize; ///< Width and height of Object World.

  public:
    CObjectWorld(); ///< Constructor.
    ~CObjectWorld(); ///< Destructor.

    void SetWorldSize(const Vector2& s); ///< Set Object World size.
    void clear(); ///< Reset to initial conditions.
    void move(); ///< Move everything.
    void draw(); ///< Draw everything.
    
    void DeliverImpulse(); ///< Apply impulse to the current bodies.
    void CreateBody(BodyType b); ///< Create a pair of bodies.

    CParticle* create(SpriteType sprite, Vector2 position); ///< Create a particle.

    CSpring* create(CParticle* v0, CParticle* v1, CParticle* c, float r); ///< Create a spring.
    void Relax(int iterations);  ///< Gauss-Seidel relaxation.
}; //CObjectWorld