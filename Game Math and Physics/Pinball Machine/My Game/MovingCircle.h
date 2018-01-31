/// \file MovingCircle.h
/// \brief Interface for the moving circle class CMovingCircle.
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

#include "Circle.h"
#include "LineSeg.h"

/// \brief The moving circle.
///
/// A moving circle is a circle shape that moves, obviously.
/// Since it moves, it also can collide with other shapes.

class CMovingCircle: public CCircle{
  friend class CObjectWorld;
  friend class CObject;

  private:
    Vector2 m_vVel; ///< Velocity vector.

    float m_fMass; ///< Mass.
    float m_fScale; ///< Scale factor.

    void ReflectVelocity(Vector2 n, const float e=1.0f); ///< Reflect velocity vector in n.
    
    BOOL Collide(CPoint* P, Vector2& poi); ///< Collide with a point.
    BOOL Collide(CLine* L, Vector2& poi); ///< Collide with a line.
    BOOL Collide(CLineSeg* L, Vector2& poi); ///< Collide with a line segment.

    BOOL GetDistToTOI(CCircle* C, const Vector2& v, float& d); ///< Get setback distance to TOI.
    
    BOOL Collide(CCircle* C, Vector2& poi);  ///< Collide with a circle.
    BOOL Collide(CMovingCircle* C, Vector2& poi);  ///< Collide with a circle.

  public:
    CMovingCircle(); ///< Default constructor.
    CMovingCircle(const Vector2& p, float r); ///< Constructor.
    CMovingCircle(const Vector2& p, const Vector2& v, float r, float e=1.0f, float m=1.0f); ///< Constructor.
}; //CMovingCircle
