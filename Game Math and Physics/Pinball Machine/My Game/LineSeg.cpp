/// \file LineSeg.cpp
/// \brief Code for the line segment class CLineSeg.
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

#include "LineSeg.h"
#include "Circle.h"
#include "debug.h"

/// Construct the line segment between two points.
/// \param p0 First end point.
/// \param p1 Second end point.
/// \param e Elasticity.

CLineSeg::CLineSeg(const Vector2& p0, const Vector2& p1, const float e):
  CLine(p0, p1 - p0, e), 
  m_bOneWay(FALSE), m_bOpen(FALSE)
{
  m_eShape = LINESEG_SHAPE;
  m_pPoint0 = new CPoint(p0, e);
  m_pPoint1 = new CPoint(p1, e);
} //constructor

/// Construct the open line segment between two points.
/// Open line segments are intended to share end points with other
/// open line segments, and therefore need to pointer to their end points
/// rather than a location.
/// \param p0 Pointer to first end point.
/// \param p1 Pointer to second end point.
/// \param e Elasticity.

CLineSeg::CLineSeg(CPoint* p0, CPoint* p1, const float e):
  CLine(p0->m_vPos, p1->m_vPos - p0->m_vPos, e), 
  m_bOneWay(FALSE), m_bOpen(TRUE)
{
  m_eShape = LINESEG_SHAPE;
  m_pPoint0 = p0;
  m_pPoint1 = p1;
} //constructor

/// Construct the one-way line segment between two points.
/// One-way line segments are also open.
/// \param p0 Pointer to first end point.
/// \param p1 Pointer to second end point.
/// \param e Elasticity.

CLineSeg::CLineSeg(CPoint* p0, CPoint* p1, const Vector2& n, const float e):
  CLineSeg(p0, p1, e)
{
  m_bOneWay = TRUE;
  m_vCanCross = n;
} //constructor

CLineSeg::~CLineSeg(){
  if(!m_bOpen){
    SAFE_DELETE(m_pPoint0);
    SAFE_DELETE(m_pPoint1);
  } //if
} //destructor

/// Check whether a point is on this line segment. 
/// \param p A point.
/// \return TRUE if point p is on this line segment.

BOOL CLineSeg::Intersect(const Vector2& p){
  const Vector2 p0 = m_pPoint0->m_vPos;
  const Vector2 p1 = m_pPoint1->m_vPos;
  return CLine::Intersect(p) && p != p0 && p != p1 && ((p0 - p).Dot(p1 - p) < 0.0f); 
} //PtInLineSeg

/// Check whether a line hits the line segment, and compute the point of
/// intersection with the infinite line that goes through the line segment.
/// \param L Pointer to a line (infinite in both directions).
/// \param p [out] Point of intersection with L.
/// \return TRUE if the point of intersection with L is on this line segment.

BOOL CLineSeg::Intersect(CLine* L, Vector2& p){
 if(CLine::Intersect(L, p)) //if the point is on the infinite line through this line segment
   return Intersect(p); //check that it is on the line segment
 else return FALSE; //otherwise just say "no"
} //Intersect

/// Check whether a circle hits this line segment.
/// That includes checking the end points.
/// \param C Pointer to circle.

BOOL CLineSeg::Intersect(CCircle* C){
  if(C->PtInCircle(m_pPoint0->m_vPos) || C->PtInCircle(m_pPoint1->m_vPos))return TRUE; //end point is in circle

  Vector2 p; //point on line closest to circle center
  return CLine::Intersect(C, p) && Intersect(p); //circle intersects line and closest point is on line segment
} //Intersect