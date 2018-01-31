/// \file Particle.h
/// \brief Interface for the particle class CParticle.
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

#include "GameDefines.h"

/// \brief The particle. 
///
/// The particle class is the representation of an abstract particle
/// in the sense of a point moving in space, not to be confused with the
/// game graphics concept of a particle engine. This will include the real
/// actual particles at the end of springs and sticks, and faux imaginary particles
/// located at the middle of springs and sticks whose sole purpose
/// is to carry the sprite representing the spring or stick in Render
/// World. Hey, I believe in rafactoring code (also known as
/// "when all you have is a hammer, everything starts to look like a nail").

class CParticle{ 
  friend class CSpring; 
  friend class CObjectWorld; 
  friend class CBody;

  private:
    SpriteType m_nSpriteType; ///< Sprite type.

    Vector2 m_vPos; ///< Current position.
    Vector2 m_vOldPos; ///< Previous position, needed for Verlet.

    float m_fRadius; ///< Radius of bounding circle.
    float m_fAngle; ///< Orientation angle.
    float m_fXScale; ///< Scale along x-axis.
    float m_fYScale; ///< Scale along y-axis.

    float EdgeCollision(const Vector2& s); ///< Collision detection and response for edges of screen.

  public:
    CParticle(SpriteType s, const Vector2& p); ///< Constructor.
    CParticle(); ///< Default constructor.

    void move(const Vector2& s); ///< Change position depending on time and velocity.
    void DeliverImpulse(float a, float m); ///< Deliver an impulse.
}; //CParticle