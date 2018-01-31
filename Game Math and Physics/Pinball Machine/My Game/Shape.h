/// \file Shape.h
/// \brief Interface for the shape class CShape.
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

#include "AABB.h"

/// \brief The shape.
///
/// This is the base class for all shapes. It contains shape type information
/// and has a couple of handy constructors.

class CShape{
  friend class CObjectWorld;

  protected:
    /// \brief Shape type.
    enum eShapeType{
      UNKNOWN_SHAPE, POINT_SHAPE, LINE_SHAPE, LINESEG_SHAPE, CIRCLE_SHAPE, MOVINGCIRCLE_SHAPE
    }; //eShapeType

    eShapeType m_eShape; ///< Type of shape.

    float m_fElasticity; ///< Elasticity, aka restitution, bounciness.
    CAabb2D m_cAABB; ///< Axially aligned bounding box enclosing shape.

  public:
    CShape(): CShape(UNKNOWN_SHAPE){}; ///< Default constructor.
    CShape(eShapeType s): CShape(s, 1.0f){}; ///< Constructor.
    CShape(eShapeType s, float e): m_eShape(s), m_fElasticity(e){}; ///< Constructor.
};//CShape
