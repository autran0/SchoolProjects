/// \file GameDefines.h
/// \brief Game specific defines.
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

/// \brief Object type.
/// Types of object that can appear in the game. 

enum ObjectType{
  UNKNOWN_OBJECT, ARROW_OBJECT, 

  GRAYBALL_OBJECT, REDBALL_OBJECT, BLUEBALL_OBJECT, YELLOWBALL_OBJECT,  GREENBALL_OBJECT, 
  MAGENTABALL_OBJECT, CYANBALL_OBJECT, ORANGEBALL_OBJECT, PURPLEBALL_OBJECT,

  GRAYCIRCLE_OBJECT, REDCIRCLE_OBJECT, BLUECIRCLE_OBJECT, YELLOWCIRCLE_OBJECT, GREENCIRCLE_OBJECT, 
  MAGENTACIRCLE_OBJECT, CYANCIRCLE_OBJECT, ORANGECIRCLE_OBJECT, PURPLECIRCLE_OBJECT,

  GRAYDOT_OBJECT, REDDOT_OBJECT, BLUEDOT_OBJECT, YELLOWDOT_OBJECT, GREENDOT_OBJECT, 
  MAGENTADOT_OBJECT, CYANDOT_OBJECT, ORANGEDOT_OBJECT, PURPLEDOT_OBJECT,

  GRAYRETICLE_OBJECT, REDRETICLE_OBJECT, BLUERETICLE_OBJECT, YELLOWRETICLE_OBJECT, GREENRETICLE_OBJECT, 
  MAGENTARETICLE_OBJECT, CYANRETICLE_OBJECT, ORANGERETICLE_OBJECT, PURPLERETICLE_OBJECT,

  BIGREDRETICLE_OBJECT
}; //ObjectType

const float MAXSPEED = 12.0f; ///< Upper speed limit.
const float MAXSPEEDSQ = MAXSPEED*MAXSPEED; ///< Upper speed limit squared.

