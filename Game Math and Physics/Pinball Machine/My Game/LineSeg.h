/// \file LineSeg.h
/// \brief Interface for the line segment class CLineSeg.
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

#include "Line.h"

/// \brief The line segment shape.
///
/// A line segment consists of a line between two end points, that is,
/// it is finite in both directions.

class CLineSeg: public CLine{
  friend class CMovingCircle;
  friend class CCircle;

  private:
    CPoint* m_pPoint0; ///< Point at end of line segment.
    CPoint* m_pPoint1; ///< Point at other end of line segment.

    BOOL m_bOneWay; ///< TRUE if line segment can be crossed in one direction.
    Vector2 m_vCanCross; ///< Vector pointing in direction in which line segment can be crossed.
    BOOL m_bOpen; ///< TRUE if the line segment is open, ie. the end points cannot be collided with.
    
    BOOL Intersect(CLine* L, Vector2& p); ///< Test and return intersection point with line.
    BOOL Intersect(CCircle* C); ///< Test for intersection with circle.
    BOOL Intersect(const Vector2& p); ///< Test whether point is on line segment.

  public:
    CLineSeg(const Vector2& p0, const Vector2& p1, const float e=1.0f); ///< Constructor.
    CLineSeg(CPoint* p0, CPoint* p1, const float e=1.0f); ///< Constructor for open line segment.
    CLineSeg(CPoint* p0, CPoint* p1, const Vector2& n, const float e=1.0f); ///< Constructor for one-way line segment.

    ~CLineSeg(); ///< Destructor.
}; //CLineSeg
