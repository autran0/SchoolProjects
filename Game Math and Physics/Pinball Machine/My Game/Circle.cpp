/// \file Circle.cpp
/// \brief Code for the circle class CCircle.
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

#include "Circle.h"
#include "LineSeg.h"
#include "Line.h"
#include "AABB.h"

#include "debug.h"

/// Construct a circle, given the location of its center and its radius.
/// \param p Position of center.
/// \param e Elasticity.
/// \param r Radius.

CCircle::CCircle(const Vector2& p, float r, float e): 
  CShape(CIRCLE_SHAPE), m_vPos(p), m_fRadius(r), m_fRadiusSq(r*r)
{
  m_fElasticity = e;
  m_cAABB = CAabb2D(-r, r, -r, r);
} //constructor

/// Check whether point is in circle.
/// \param p A point.
/// \return TRUE if p is in the circle.

BOOL CCircle::PtInCircle(const Vector2& p){
  return (m_vPos - p).LengthSquared() < m_fRadiusSq;
} //PtInCircle