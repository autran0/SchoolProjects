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
#include "Box2D\Box2D.h"

/// \brief The game object.
///
/// Game objects are responsible for remembering information about themselves,
/// in particular, their representations in Render World and Physics World.

class CObject{ 
  friend class CHeadsUpDisplay; 
  friend class CObjectWorld; 
  friend class CRenderWorld;

  protected:
    ObjectType m_nObjectType; ///< Object type.
    b2Body* m_pBody; ///< Physics World body.
    b2Vec2 m_b2vOldV; ///< Old velocity vector, needed to infer collisions.

  public:
    CObject(ObjectType objecttype); ///< Constructor.
    ~CObject(); ///< Destructor.

    void SetPhysicsBody(b2Body* b); ///< Set pointer to physics world body.
    void MakeSound(); ///< Make the appropriate sounds
}; //CObject