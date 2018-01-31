/// \file AABB.h
/// \brief Interface for the AABB class CAabb2D.
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

#include "GameDefines.h"

/// \brief The 2D Axially Aligned Bounding Box.
///
/// An AABB, or Axially Aligned Bounding Box, is a rectangle enclosing
/// an object and whose axes are aligned with the cardinal axes.


class CAabb2D{
  friend class CObjectWorld;
  friend class CMovingCircle;

  private:
    float left; ///< X coordinate of left side.
    float right; ///< X coordinate of right side.
    float top; ///< Y coordinate of top.
    float bottom; ///< Y coordinate of bottom.

  public:
    CAabb2D(float l, float r, float b, float t); ///< Constructor.
    CAabb2D(); ///< Default constructor.

    BOOL Intersect(const CAabb2D& aabb, const Vector2& u, const Vector2& v); ///< AABB intersection test.
    BOOL Intersect(const CAabb2D& aabb, const Vector2& u); ///< AABB intersection test.
}; //CAabb2D
