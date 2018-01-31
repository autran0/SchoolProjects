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

/// \brief Types of object that can appear in the game. 

enum ObjectType{
  UNKNOWN_OBJECT, BALL_OBJECT, CRATE_OBJECT, BARREL_OBJECT, MOUNT_OBJECT, WHEEL_OBJECT,
  TEMPGUAGE_OBJECT, TEMPNEEDLE_OBJECT, TEMPMAXNEEDLE_OBJECT, CLOCKFACE_OBJECT, CLOCKNEEDLE_OBJECT,
  PIRATE_OBJECT  //easter egg
}; //ObjectType

/// \brief State of game play, including whether the player has won or lost.

enum GameStateType{
  PLAYING_GAMESTATE, WON_GAMESTATE, LOST_GAMESTATE
}; //GameStateType

const int g_nMaxPlayingTime = 60; ///< Maximum allowable playing time per level.

//Translate units between Render World and Physics World
const float fPRV = 10.0f; ///< Physics World to Render World rescale value.

/// \brief Physics World to Render World units.
inline float PW2RW(float x){return x*fPRV;}; 

/// \brief Render World to Physics World units for a float.
inline float RW2PW(float x){return x/fPRV;}; 

/// \brief Render World to Physics World units for an int.
inline float RW2PW(int x){return (float)x/fPRV;}; 