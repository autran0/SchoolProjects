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
#include "Sndlist.h"

extern b2World g_b2dPhysicsWorld; ///< Box2D Physics World.
extern CSoundManager* g_pSoundManager; ///< The sound manager.

/// Constructor for a game object.
/// \param objecttype Object type.

CObject::CObject(ObjectType objecttype){ 
  m_nObjectType = objecttype; 
  m_pBody = nullptr;
  m_b2vOldV = b2Vec2(0, 0);
} //constructor

/// Destructor for a game object. It takes
/// care of destroying the object's body in Physics World.

CObject::~CObject(){
  if(m_pBody)
    g_b2dPhysicsWorld.DestroyBody(m_pBody);
} //destructor

/// Set the physics body pointer (from Physics World) of the game object.
/// \param b Pointer to the physics body.

void CObject::SetPhysicsBody(b2Body* b){
  m_pBody = b;
} //SetPhysicsBody

/// Make collision sound if a collision has occurred. This function
/// fakes collision detection by querying Physics World
/// to see whether the object's velocity has changed. If the
/// length squared of the change in velocity vector is larger
/// than some arbitrarily chosen value, then this function concludes
/// that a collision must have occurred, so it makes a sound.
/// The exact sound made will depend on the object type.

void CObject::MakeSound(){ 
  if(!m_pBody)return; //safety

  b2Vec2 vNewV = m_pBody->GetLinearVelocityFromWorldPoint(b2Vec2(0, 0));
  b2Vec2 vDelta = m_b2vOldV - vNewV; //delta vee

  if(vDelta.LengthSquared() > 200.0f) //big delta vee
    switch(m_nObjectType){ //make correct sound
      case BALL_OBJECT:
      case WHEEL_OBJECT:
        g_pSoundManager->play(THUMP_SOUND);
        break;

      case CRATE_OBJECT:
      case MOUNT_OBJECT:
        g_pSoundManager->play(THUMP2_SOUND);
        break;

      case BARREL_OBJECT:
        g_pSoundManager->play(CLANG_SOUND);
        break;
    } //switch

  m_b2vOldV = vNewV;
} //MakeSound