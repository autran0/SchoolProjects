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
#include "Timer.h"
#include "debug.h"

extern CTimer g_cTimer;

/// \param t Type of ball.
/// \param v Initial position of ball.
/// \param s Diameter of ball.

CObject::CObject(ObjectType t, const Vector2& v, int s){ 
  m_nObjectType = t; 
  m_nSize = s;
  m_vPos = v;
  m_vVel = Vector2(0.0f);
  m_bInPocket = FALSE;
} //constructor

/// Move the ball, apply collision and response with pockets and rails,
/// playing the appropriate sound if necessary.

void CObject::move(){ 
  if(m_bInPocket)return; //bail if in pocket already

  const float SCALE = 20.0f; //to scale back motion
  const float FRICTION = 1.0f/1500.0f; //coefficient of friction
  const float MINSPEEDSQ = 0.5f; //minimum speed squared

  float dt = (float)g_cTimer.frametime(); //time since last move
  m_vPos += m_vVel*dt/SCALE; //move
  m_vVel *= 1.0f - dt*FRICTION; //friction

  if(m_vVel.LengthSquared() < MINSPEEDSQ) //if moving too slowly
    m_vVel = Vector2(0.0f); //stop
} //move

/// Deliver an impulse of a given angle and magnitude.
/// \param angle Angle at which the impulse is to be applied.
/// \param magnitude Magnitude of the impulse.

void CObject::DeliverImpulse(float angle, float magnitude){ 
  m_vVel = magnitude * Vector2(cos(angle), sin(angle));
} //DeliverImpulse
