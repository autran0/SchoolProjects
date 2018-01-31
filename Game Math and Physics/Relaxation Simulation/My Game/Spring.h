/// \file Spring.h
/// \brief Interface for the spring class CSpring.
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

#include "Particle.h"

/// \brief The spring.
///
/// The spring class is the representation of an abstract spring
/// including, if the springiness is very small, a stick.
/// Springs have faux imaginary particles located at their centers whose sole purpose
/// is to carry the sprite representing the spring or stick in Render World.

class CSpring{
  friend class CObjectWorld; 
  friend class CBody;

  private:
    CParticle* m_pV0; ///< Particle at one end.
    CParticle* m_pV1; ///< Particle at the other end.
    CParticle* m_pCenter; ///< Virtual particle at the center.

    float m_fRestLength; ///< Rest length.
    float m_fRestitution; ///< Springiness, between 0.0f and 0.5f. 
    float m_fAngle; ///< Orientation.

  public:
    CSpring(); ///< Constructor.

    void Relax();  ///< Gauss-Seidel relaxation.
    void ComputeCenter(); ///< Compute spring center.
}; //CSpring