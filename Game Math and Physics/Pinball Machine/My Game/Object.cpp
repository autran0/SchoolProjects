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
#include "RenderWorld.h"
#include "debug.h"

extern CTimer g_cTimer;
extern CRenderWorld g_cRenderWorld;

/// \param t Type of object.
/// \param c Type of breadcrumb.
/// \param s Initial position.
/// \param v Initial velocity.
/// \param r Radius.
/// \param e Elasticity.
/// \param m Size multiplier.

CObject::CObject(ObjectType t, ObjectType c, const Vector2& s, const Vector2& v, float r, float e, float m){ 
  m_nObjectType = t; 

  m_cShape = CMovingCircle(s, v, r*m, e);
  m_cShape.m_cAABB = CAabb2D(-r*m, r*m, -r*m, r*m);
  m_cShape.m_fScale = m;

  m_nCurCrumb = 0;
  m_eCrumbType = c;
} //constructor

/// Move the object.

void CObject::move(float dt){ 
  const float SCALE = 20.0f; //to scale back motion
  m_cShape.m_vPos += m_cShape.m_vVel*dt/SCALE; //move
  m_cShape.m_vVel *= 0.9999f; //friction
} //move

/// Deliver an impulse of a given angle and magnitude.
/// \param angle Angle at which the impulse is to be applied.
/// \param magnitude Magnitude of the impulse.

void CObject::DeliverImpulse(float angle, float magnitude){ 
  m_cShape.m_vVel = magnitude * Vector2(cos(angle), sin(angle));
} //DeliverImpulse
