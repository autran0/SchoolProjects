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
#include "MovingCircle.h"

const int NUMCRUMBS = 128; ///< Number of breadcrumbs.

/// \brief The game object. 
///
/// The game object represents an object in Object World.

class CObject{
  friend class CObjectWorld;

  private:
    ObjectType m_nObjectType; ///< Object type.
    CMovingCircle m_cShape; ///< Moving circle shape.
    
    Vector2 m_vCrumbPos[NUMCRUMBS]; ///< Breadcrumb trails.
    int m_nCurCrumb; ///< Current crumb index.
    ObjectType m_eCrumbType; ///< Crumb type.

  public:
    CObject(ObjectType t, ObjectType c, const Vector2& s, const Vector2& v, float r, float e, float m); ///< Constructor.
    void move(float dt); ///< Change position depending on time and velocity.
    void DeliverImpulse(float angle, float magnitude); ///< Deliver an impulse.
}; //CObject