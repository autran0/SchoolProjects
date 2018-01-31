/// \file Object.h
/// \brief Interface for the game object class CObject.
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

/// \brief The game object. 
///
/// The game object represents an object in Object World.

class CObject{
  friend class CObjectWorld;

  private:
    ObjectType m_nObjectType; ///< Object type.
    Vector2 m_vPos; ///< Current position.
    Vector2 m_vVel; ///< Current velocity.
    int m_nSize; ///< Ball size.
    BOOL m_bInPocket; ///< Whether ball is currently in a pocket.

  public:
    CObject(ObjectType t, const Vector2& v, int s); ///< Constructor.
    void move(); ///< Change position depending on time and velocity.
    void DeliverImpulse(float angle, float magnitude); ///< Deliver an impulse.
}; //CObject