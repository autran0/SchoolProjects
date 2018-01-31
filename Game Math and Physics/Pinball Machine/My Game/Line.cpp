/// \file Line.cpp
/// \brief Code for the line class CLine.
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

#include "Line.h"
#include "Circle.h"
#include "debug.h"

/// Given a point and a gradient, construct the unique line
/// through that point with that gradient.
/// \param p Point.
/// \param m Gradient.
/// \param e Elasticity.

CLine::CLine(const Vector2& p, const float m, const float e): 
  CShape(LINE_SHAPE, e), 
  m_fGradient(m), m_fInverseGradient(1.0f/m), 
  m_fYIntercept(p.y - m*p.x), m_fXIntercept(p.x){
} //constructor

/// Given a vector and a point, construct the unique line
/// through that point in the direction of the vector.
/// \param v Vector indicating the line's direction.
/// \param p Point on the line.
/// \param e Elasticity.

CLine::CLine(const Vector2& p, const Vector2& v, const float e): 
  CLine(p, v.y/v.x, e){
} //constructor

/// Given another line, find the unique point that is on both lines.
/// We have to be careful to use inverse gradient whenever the
/// gradient is close to zero.
/// \param L Pointer to line to intersect with.
/// \param p [out] Point of intersection of this line with that one, if there is one.
/// \return TRUE if the two lines intersect.

BOOL CLine::Intersect(CLine* L, Vector2& p){
  //some handy shorthands to make this more readable
  const float m1 = m_fGradient;
  const float m1inv = m_fInverseGradient;
  const float c1 = m_fYIntercept;

  const float m2 = L->m_fGradient;
  const float m2inv = L->m_fInverseGradient;
  const float c2 = L->m_fYIntercept;

  if(m1 == m2) //parallel lines don't meet
    return FALSE;

  //neither line vertical
  if(isfinite(m1) && isfinite(m2)){ 
    p.x = (c2 - c1)/(m1 - m2);
    p.y = m1*p.x + c1;
  } //if

  //only the first line is vertical
  else if(isinf(m1) && isfinite(m2)){
    p.x = m_fXIntercept;
    p.y = m2*p.x + c2;
  } //else if

  //only the second line is vertical
  else if(isfinite(m1) && isinf(m2)){
    p.x = L->m_fXIntercept;
    p.y = m1*p.x + c1;
  } //else if

  return TRUE;
} //Intersect

/// Given a point, find the point on this line that is closest to it.
/// Intersect with a line through p perpendicular to this line.
/// \param p Point to check.
/// \return Point on this line that is closest to it.

Vector2 CLine::ClosestPoint(const Vector2& p){
  Vector2 p0; //the result
  if(m_fInverseGradient == 0.0f || isinf(m_fInverseGradient))
    Intersect(&CLine(p, m_fInverseGradient), p0); 
  else 
    Intersect(&CLine(p, -m_fInverseGradient), p0); 
  return p0;
} //ClosestPoint

/// Given a circle, find the unique point on this line that is
/// closest to its center. Also check whether the circle hits the line.
/// \param C Pointer to circle to check.
/// \param p [out] Closest point on this line to the circle's center.
/// \return TRUE if the circle overlaps this line. 

BOOL CLine::Intersect(CCircle* C, Vector2& p){
  p = ClosestPoint(C->m_vPos); //point on this line closest to the circle's center
  return C->PtInCircle(p); //the circle overlaps this line if p is inside the circle
} //Intersect

/// Does a given circle overlap this line?
/// \param C Circle to check.
/// \return TRUE if the circle overlaps this line. 

BOOL CLine::Intersect(CCircle* C){
  Vector2 p = ClosestPoint(C->m_vPos);
  return C->PtInCircle(p);
} //Intersect

/// Does a point intersect this line? That is, is the point on the line?
/// \param p A point.
/// \return TRUE if p is on this line.

BOOL CLine::Intersect(const Vector2& p){
  //shorthands
  const float m = m_fGradient;
  const float c = m_fYIntercept;
  const float b = m_fXIntercept;

  return isfinite(m)? fabs(p.y - m*p.x - c) < 0.001: fabs(p.x - b) < 0.001;
} //Intersect

/// Get a normal to the line in the direction of a vector.
/// If v is parallel to this line, then report failure.
/// \param v A vector.
/// \param nhat The normal in the direction of v.
/// \return FALSE if v is parallel to this line.

BOOL CLine::GetNormal(const Vector2& v, Vector2& nhat){
  Vector2 p0;
  if(isfinite(m_fGradient))
    p0 = Vector2(0.0f, m_fYIntercept);
  else
    p0 = Vector2(m_fXIntercept, 0.0f);

  Vector2 p1 = p0 + 100.0f*v;
  if(Intersect(p1))return FALSE; //p shouldn't be on the line, dummy

  nhat = p1 - ClosestPoint(p1); //nhat is a vector orthogonal to this line
  nhat.Normalize(); //normalize it
  return TRUE;
} //GetNormal
