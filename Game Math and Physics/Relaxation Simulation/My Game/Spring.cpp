/// \file Spring.cpp
/// \brief Code for the spring class CSpring.
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

#include "Spring.h"

#include "RenderWorld.h"
#include "debug.h"

extern CRenderWorld g_cRenderWorld;

CSpring::CSpring(){ 
  m_pV0 = m_pV1 = m_pCenter = nullptr;
  m_fRestLength = 100.0f; //fair-sized
  m_fRestitution = 0.5f; //stick
} //constructor

/// Perform a single iteration of Gauss-Seidel relaxation to the spring.

void CSpring::Relax(){ 
  Vector2 vDelta = m_pV0->m_vPos - m_pV1->m_vPos;
  float fLength = vDelta.Length(); //stick current length

  if(fabs(fLength - m_fRestLength) > 0.5f){ //if different enough
    vDelta *= 1.0f - m_fRestLength/fLength; //amount to change by
    vDelta *= m_fRestitution; //springiness
    m_pV0->m_vPos -= vDelta; //some from one end
    m_pV1->m_vPos += vDelta; //some from the other
  } //if
} //Relax

/// The particles at the ends of the spring are moved and relaxed like any
/// sensible object should be. The center of the spring just gets dragged around.
/// This is where it catches up.

void CSpring::ComputeCenter(){
  if(!m_pCenter)return; //fail and bail

  Vector2 p0 = m_pV0->m_vPos; //p0 is the position of one end
  Vector2 p1 = m_pV1->m_vPos; //p1 is the other
  Vector2 v = p0 - p1; //vector from p1 to p0
  m_fAngle = m_pCenter->m_fAngle = atan2(v.y, v.x); //the angle between them

  m_pCenter->m_vPos = (p0 + p1)/2.0f; //center position
  m_pCenter->m_vOldPos = m_pCenter->m_vPos; //stopped
  int sw = g_cRenderWorld.GetWidth(m_pCenter->m_nSpriteType); //sprite width
  m_pCenter->m_fXScale = v.Length()/sw; //scale needed for sprite
} //ComputeCenter