/// \file Line.h
/// \brief Interface for the line class CLine.
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

#include "Point.h"

/// \brief The line shape.
///
/// A line is infinite in both directions. It consists of
/// its gradient m and y-incercept c, that is, it has the
/// equation y = mx + c. The gradient can be infinite, in
/// that case it has the equation x = b, where b is its x-intercept.

class CLine: public CShape{
  friend class CLineSeg;
  friend class CCircle;
  friend class CMovingCircle;

  protected:
    float m_fGradient; ///< Gradient.
    float m_fInverseGradient; ///< Inverse gradient.
    float m_fYIntercept; ///< Intercept with Y axis.
    float m_fXIntercept; ///< Intercept with X axis.
    
    Vector2 ClosestPoint(const Vector2& p); ///< Get closest point on line.
    BOOL Intersect(CLine* L, Vector2& p); ///< Get intersection point with line.
    BOOL Intersect(CCircle* C, Vector2& p); ///< Get intersection point with circle.
    BOOL Intersect(CCircle* C); ///< Test intersection point with circle.
    BOOL Intersect(const Vector2& p); ///< Test point on line.

    BOOL GetNormal(const Vector2& v, Vector2& nhat); ///< Get normal to line.

  public:
    CLine(const Vector2& p, const float m, const float e=1.0f); ///< Constructor.
    CLine(const Vector2& p, const Vector2& v, const float e=1.0f); ///< Constructor.
}; //CLine
