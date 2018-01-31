/// \file Point.h
/// \brief Interface for point class CPoint.
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

#include "Shape.h"
#include "GameDefines.h"

/// \brief The point shape.
///
/// A point has no dimensions, only a position.

class CPoint: public CShape{
  friend class CLineSeg;
  friend class CMovingCircle;

  private:
    Vector2 m_vPos; ///< Position.

  public:
    CPoint(): CPoint(Vector2(0.0f)){}; ///< Default constructor.
    CPoint(const Vector2& p, const float e=1.0f): CShape(POINT_SHAPE, e), m_vPos(p){}; ///< Constructor.
}; //CPoint
