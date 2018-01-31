/// \file AABB.cpp
/// \brief Code for the AABB class CAabb2D.
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

#include "AABB.h"
#include "debug.h"

CAabb2D::CAabb2D(float l, float r, float b, float t):
  left(l), right(r), top(t), bottom(b){} //constructor
  
CAabb2D::CAabb2D(): CAabb2D(0.0f, 0.0f, 0.0f, 0.0f){} //default constructor

/// Determine whether this AABB whose position is offset by a certain amount
/// intersects with another AABB which is also offset by a different amount.
/// \param aabb The zero-origin AABB to collide with
/// \param u Position offset of this AABB.
/// \param v Position offset of parameter aabb.
/// \return TRUE if they intersect.

BOOL CAabb2D::Intersect(const CAabb2D& aabb, const Vector2& u, const Vector2& v){
  return 
    (left + u.x) <= (aabb.right + v.x) && //my left side is to the left of its right side
    (right + u.x) >= (aabb.left + v.x) && //my right side is to the right of its left side
    (top + u.y) >= (aabb.bottom + v.y) && //my top is above its bottom
    (bottom + u.y) <= (aabb.top + v.y); //my bottom is below its top
} //Intersect

/// Determine whether this AABB whose position is offset by a certain amount
/// intersects with another AABB that is not offset.
/// \param aabb The zero-origin AABB to collide with
/// \param u Position offset of this AABB.
/// \return TRUE if they intersect.

BOOL CAabb2D::Intersect(const CAabb2D& aabb, const Vector2& u){
  return Intersect(aabb, u, Vector2(0.0f));
} //Intersect
