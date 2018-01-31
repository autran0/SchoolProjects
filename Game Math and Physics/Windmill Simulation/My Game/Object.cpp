/// \file Object.cpp
/// \brief Code for the game object class CObject.
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

#include "Object.h"
#include "debug.h"

extern b2World g_b2dPhysicsWorld;

/// Constructor for a game object.
/// \param objecttype Object type.

CObject::CObject(ObjectType objecttype){
  m_nObjectType = objecttype; 
  m_pBody = nullptr;
} //constructor

/// Destructor for a game object. It takes
/// care of destroying the object's body in Physics World.

CObject::~CObject(){ //destructor
  if(m_pBody)
    g_b2dPhysicsWorld.DestroyBody(m_pBody);
} //destructor

/// Set the physics body pointer (from Physics World) of the game object.
/// \param b Pointer to the physics body.

void CObject::SetPhysicsBody(b2Body* b){
  m_pBody = b;
} //SetPhysicsBody