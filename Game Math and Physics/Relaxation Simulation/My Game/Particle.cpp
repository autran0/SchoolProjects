/// \file Particle.cpp
/// \brief Code for the particle class CParticle.
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

#include "Particle.h"

#include "SndList.h"
#include "RenderWorld.h"
#include "debug.h"

extern CRenderWorld g_cRenderWorld;
extern CSoundManager* g_pSoundManager; 
extern BodyType g_nCurrentBody;
extern float g_fSizeMult;

/// \param sprite Sprite type.
/// \param position Initial position.

CParticle::CParticle(SpriteType s, const Vector2& p){ 
  m_nSpriteType = s; 
  m_fAngle = 0.0f; m_fXScale = m_fYScale = 1.0f;
  m_fRadius = g_cRenderWorld.GetWidth(s)/2.0f * g_fSizeMult; 
  m_vPos = m_vOldPos = p;
} //constructor

CParticle::CParticle(){ 
  m_nSpriteType = INVISIBLE_SPRITE; 
  m_vPos = m_vOldPos = Vector2(0.0f);
  m_fAngle = 0.0f; m_fXScale = m_fYScale = 1.0f;
  m_fRadius = 32.0f * g_fSizeMult;
} //constructor

/// Collision and response for particle hitting an edge of the screen. 
/// Checks for a collision, and does the necessary housework for reflecting
/// a particle if it hits an edge. This function ignores all of the stuff
/// in Section 2.2 about computing where the particle should be at the
/// end of the frame and simply backs off the particle so that it does not
/// appear to go partially off the screen. This gives the screen edges
/// a certain amount of stickiness. Making this right is left as an exercise
/// for the reader.
/// \param s Screen width and height.
/// \return Collision speed, if particle hits an edge.

float CParticle::EdgeCollision(const Vector2& s){ 
  const float fRestitution = 0.6f; //how bouncy the edges are 
  Vector2 vDelta = m_vPos - m_vOldPos; //vector from old to curren position

  const float left  = m_fRadius;
  const float bottom = m_fRadius;
  const float right = s.x - m_fRadius;
  const float top = s.y - m_fRadius;

  float cspeed = 0.0f; //collision speed

  //left and right edges
  if(m_vPos.x < left || m_vPos.x > right){ 
    m_vPos.x = m_vPos.x < left? left: right; 
    vDelta.y = -vDelta.y;
    m_vOldPos = m_vPos + fRestitution * vDelta; 
    cspeed = fabs(vDelta.x);
  } //if

  //top and bottom edges
  if(m_vPos.y < bottom || m_vPos.y > top){
    m_vPos.y = m_vPos.y < bottom? bottom: top; 
    vDelta.x = -vDelta.x;
    m_vOldPos = m_vPos + fRestitution * vDelta; 
    cspeed = fabs(vDelta.y);
  } //if

  return cspeed;
} //EdgeCollision

/// Move the particle using Verlet integration, apply collision and response.
/// \param s Screen width and height.

void CParticle::move(const Vector2& s){ 
  Vector2 vTemp = m_vPos;
  m_vPos += m_vPos - m_vOldPos; 
  m_vOldPos = vTemp;
  m_vPos.y -= 0.2f * g_fSizeMult; //gravity

  float cspeed = EdgeCollision(s); //collision speed
  if(cspeed > 0.25f*g_fSizeMult){ //edge collision
    if(g_nCurrentBody == RAGDOLL_BODY){
      if(cspeed > 5.0f*g_fSizeMult)
        g_pSoundManager->play(OW_SOUND);
    } //if
    else if(m_nSpriteType == BALL_SPRITE){
      g_pSoundManager->play(BOING_SOUND);
      g_pSoundManager->volume(cspeed/10.0f);
    } //else if
    else if(m_nSpriteType == WOODCIRCLE_SPRITE){
      g_pSoundManager->play(THUMP_SOUND);
      g_pSoundManager->volume(cspeed/10.0f);
    } //else if
  } //if
} //move

/// Deliver an impulse to the particle, given the angle and magnitude.
/// \param a Angle at which the impulse is to be applied.
/// \param m Magnitude of the impulse to apply.

void CParticle::DeliverImpulse(float a, float m){ 
  m_vOldPos = m_vPos - m*Vector2(cos(a), sin(a)); //Verlet again
} //DeliverImpulse