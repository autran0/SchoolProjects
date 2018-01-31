/// \file GameDefines.h
/// \brief Game defines.
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

#include "Defines.h"

/// \brief Sprite type. 

enum SpriteType{
  INVISIBLE_SPRITE, BALL_SPRITE, WOODCIRCLE_SPRITE, SPRING_SPRITE, STICK_SPRITE
}; //SpriteType

/// \brief Body type.

enum BodyType{
  CHAIN2_BODY, CHAIN3_BODY, CHAIN4_BODY, TRIANGLE_BODY,
  SQUARE_BODY, WHEEL5_BODY, WHEEL6_BODY,
  RAGDOLL_BODY, 
  NUM_BODIES //must be last
}; //BodyType

/// \brief Size type

enum SizeType {
	NORMAL_SIZE, HALF_SIZE, QUARTER_SIZE
};