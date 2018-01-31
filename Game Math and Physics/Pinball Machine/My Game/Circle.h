/// \file Circle.h
/// \brief Interface for the circle class CCircle.
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

/// \brief The circle shape.
///
/// A circle consists of the position of its center point and its radius.

class CCircle: public CShape{
  friend class CLineSeg;
  friend class CLine;
  friend class CMovingCircle;

  protected:
    float m_fRadius; ///< Radius.
    float m_fRadiusSq; ///< Radius squared, used for faster distance calculations.
    Vector2 m_vPos; ///< Position of center.
  
    BOOL PtInCircle(const Vector2& p); ///< Point in circle test.

  public:
    CCircle(const Vector2& p, float r, float e=1.0f); ///< Constructor.
}; //CCircle